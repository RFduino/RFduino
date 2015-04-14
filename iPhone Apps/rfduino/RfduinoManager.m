/*
 Copyright (c) 2013 OpenSourceRF.com.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <objc/message.h>

#import "RFduinoManager.h"

#import "RFduino.h"

static CBUUID *service_uuid;

@interface RFduinoManager()
{
    NSTimer *rangeTimer;
    int rangeTimerCount;
    bool didUpdateDiscoveredRFduinoFlag;
    void (^cancelBlock)(void);
    bool isScanning;
}
@end

@implementation RFduinoManager

@synthesize delegate;
@synthesize rfduinos;

+ (RFduinoManager *)sharedRFduinoManager
{
    static RFduinoManager *rfduinoManager;
    if (! rfduinoManager) {
        rfduinoManager = [[RFduinoManager alloc] init];
    }
    return rfduinoManager;
}

- (id)init
{
    NSLog(@"init");
    
    self = [super init];
    
    if (self) {
        service_uuid = [CBUUID UUIDWithString:(customUUID ? customUUID : @"2220")];
        
        rfduinos = [[NSMutableArray alloc] init];
        
        central = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    }
    
    return self;
}

- (bool)isBluetoothLESupported
{
    if ([central state] == CBCentralManagerStatePoweredOn)
        return YES;
    
    NSString *message;
    
    switch ([central state])
    {
        case CBCentralManagerStateUnsupported:
            message = @"This hardware doesn't support Bluetooth Low Energy.";
            break;
        case CBCentralManagerStateUnauthorized:
            message = @"This app is not authorized to use Bluetooth Low Energy.";
            break;
        case CBCentralManagerStatePoweredOff:
            message = @"Bluetooth is currently powered off.";
            break;
        case CBCentralManagerStateUnknown:
            // fall through
        default:
            message = @"Bluetooth state is unknown.";
            
    }

    if ([delegate respondsToSelector:@selector(shouldDisplayAlertTitled:messageBody:)]) {
        [delegate shouldDisplayAlertTitled:@"Bluetooth LE Support" messageBody:message];
    }

    return NO;
}

- (void)startRangeTimer
{
    rangeTimerCount = 0;
    
    rangeTimer = [NSTimer scheduledTimerWithTimeInterval:1.0
                                                  target:self
                                                selector:@selector(rangeTick:)
                                                userInfo:nil
                                                 repeats:YES];
    
}

- (void)stopRangeTimer
{
    [rangeTimer invalidate];
}

- (void) rangeTick:(NSTimer*)timer
{
    bool update = false;

    rangeTimerCount++;
    if ((rangeTimerCount % 60) == 0) {
        // NSLog(@"restarting scanning");
        
        [central stopScan];
        
        NSDictionary *options = nil;
        options = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES]
                                              forKey:CBCentralManagerScanOptionAllowDuplicatesKey];
        [central scanForPeripheralsWithServices:[NSArray arrayWithObject:service_uuid] options:options];
    }
    
    
    NSDate *date = [NSDate date];
    for (RFduino *rfduino in rfduinos) {
        if (!rfduino.outOfRange
            && rfduino.lastAdvertisement != NULL
            && [date timeIntervalSinceDate:rfduino.lastAdvertisement] > 2)
        {
            rfduino.outOfRange = true;
            update = true;
        }
    }
    
    if (update) {
        if (didUpdateDiscoveredRFduinoFlag) {
            [delegate didUpdateDiscoveredRFduino:nil];
        }
    }
}

#pragma mark - CentralManagerDelegate methods

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    NSLog(@"didConnectPeripheral");

    RFduino *rfduino = [self rfduinoForPeripheral:peripheral];
    if (rfduino) {
        [rfduino connected];
        [delegate didConnectRFduino:rfduino];
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"didDisconnectPeripheral");

    void (^block)(void) = ^{
        if ([delegate respondsToSelector:@selector(didDisconnectRFduino:)]) {
            RFduino *rfduino = [self rfduinoForPeripheral:peripheral];
            if (rfduino) {
                [delegate didDisconnectRFduino:rfduino];
            }
        }
    };
    
    if (error.code) {
        cancelBlock = block;

        if ([delegate respondsToSelector:@selector(shouldDisplayAlertTitled:messageBody:)]) {
            [delegate shouldDisplayAlertTitled:@"Peripheral Disconnected with Error" messageBody:error.description];
        }
        
    }
    else
        block();
    
    if (peripheral) {
        [peripheral setDelegate:nil];
        peripheral = nil;
    }
}

- (RFduino *)rfduinoForPeripheral:(CBPeripheral *)peripheral
{
    for (RFduino *rfduino in rfduinos) {
        if ([peripheral isEqual:rfduino.peripheral]) {
            return rfduino;
        }
    }
    return nil;
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    // NSLog(@"didDiscoverPeripheral");

    bool added = false;

    RFduino *rfduino = [self rfduinoForPeripheral:peripheral];
    if (! rfduino) {
        rfduino = [[RFduino alloc] init];
        
        rfduino.rfduinoManager = self;

        rfduino.name = peripheral.name;
        
        rfduino.peripheral = peripheral;
        
        added = true;
        
        [rfduinos addObject:rfduino];
    }
    
    rfduino.advertisementData = nil;
    
    id manufacturerData = [advertisementData objectForKey:CBAdvertisementDataManufacturerDataKey];
    if (manufacturerData) {
        const uint8_t *bytes = [manufacturerData bytes];
        NSUInteger len = [manufacturerData length];
        // skip manufacturer uuid
        NSData *data = [NSData dataWithBytes:bytes+2 length:len-2];
        rfduino.advertisementData = data;
    }
    
    rfduino.advertisementRSSI = RSSI;
    rfduino.advertisementPackets++;
    rfduino.lastAdvertisement = [NSDate date];
    rfduino.outOfRange = false;
    
    if (added) {
        [delegate didDiscoverRFduino:rfduino];
    } else {
        if (didUpdateDiscoveredRFduinoFlag) {
            [delegate didUpdateDiscoveredRFduino:rfduino];
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"didFailToConnectPeripheral");

    if ([delegate respondsToSelector:@selector(shouldDisplayAlertTitled:messageBody:)]) {
        [delegate shouldDisplayAlertTitled:@"Connect Failed" messageBody:error.description];
    }
}

- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals
{
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)aCentral
{
    NSLog(@"central manager state = %ld", [central state]);
    
    bool success = [self isBluetoothLESupported];
    if (success) {
        [self startScan];
    }
}

#pragma mark - Rfduino methods

- (bool)isScanning
{
    return isScanning;
}

- (void)startScan
{
    NSLog(@"startScan");
    
    isScanning = true;

    NSDictionary *options = nil;
    
    didUpdateDiscoveredRFduinoFlag = [delegate respondsToSelector:@selector(didUpdateDiscoveredRFduino:)];
    
    if (didUpdateDiscoveredRFduinoFlag) {
        options = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES]
        forKey:CBCentralManagerScanOptionAllowDuplicatesKey];
    }

    [rfduinos removeAllObjects];
    
    [central scanForPeripheralsWithServices:[NSArray arrayWithObject:service_uuid] options:options];
    
    if (didUpdateDiscoveredRFduinoFlag) {
        [self startRangeTimer];
    }
}

- (void)stopScan
{
    NSLog(@"stopScan");
    
    if (didUpdateDiscoveredRFduinoFlag) {
        [self stopRangeTimer];
    }
    
    [central stopScan];
    
    isScanning = false;
}

- (void)connectRFduino:(RFduino *)rfduino
{
    NSLog(@"connectRFduino");
    
    [central connectPeripheral:[rfduino peripheral] options:nil];
}

- (void)disconnectRFduino:(RFduino *)rfduino
{
    NSLog(@"rfduinoManager disconnectPeripheral");
    
    [central cancelPeripheralConnection:rfduino.peripheral];
}

- (void)loadedServiceRFduino:(id)rfduino
{
    if ([delegate respondsToSelector:@selector(didLoadServiceRFduino:)]) {
        [delegate didLoadServiceRFduino:rfduino];
    }
}

@end
