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

#import "RFduino.h"

#import "RFduinoManager.h"

static const int max_data = 12;

// default NULL (NULL = previous fixed RFduino uuid)
NSString *customUUID = NULL;

static CBUUID *service_uuid;
static CBUUID *send_uuid;
static CBUUID *receive_uuid;
static CBUUID *disconnect_uuid;

char data(NSData *data)
{
    return (char)dataByte(data);
}

uint8_t dataByte(NSData *data)
{
    uint8_t *p = (uint8_t*)[data bytes];
    NSUInteger len = [data length];
    return (len ? *p : 0);
}

int dataInt(NSData *data)
{
    uint8_t *p = (uint8_t*)[data bytes];
    NSUInteger len = [data length];
    return (sizeof(int) <= len ? *(int*)p : 0);
}

float dataFloat(NSData *data)
{
    uint8_t *p = (uint8_t*)[data bytes];
    NSUInteger len = [data length];
    return (sizeof(float) <= len ? *(float*)p : 0);
}

// increment the 16-bit uuid inside a 128-bit uuid
static void incrementUuid16(CBUUID *uuid, unsigned char amount)
{
    NSData *data = uuid.data;
    unsigned char *bytes = (unsigned char *)[data bytes];
    unsigned char result = bytes[3] + amount;
    if (result < bytes[3])
        bytes[2]++;
    bytes[3] += amount;
}

@interface RFduino()
{
    CBCharacteristic *send_characteristic;
    CBCharacteristic *disconnect_characteristic;
    bool loadedService;
}
@end

@implementation RFduino

@synthesize delegate;
@synthesize rfduinoManager;
@synthesize peripheral;

@synthesize name;
@synthesize UUID;
@synthesize advertisementData;
@synthesize advertisementRSSI;
@synthesize advertisementPackets;
@synthesize outOfRange;

- (id)init
{
    NSLog(@"rfduino init");
    
    self = [super init];
    if (self) {
    }
    
    return self;
}

- (void)connected
{
    NSLog(@"rfduino connected");
    
    service_uuid = [CBUUID UUIDWithString:(customUUID ? customUUID : @"2220")];
    receive_uuid = [CBUUID UUIDWithString:(customUUID ? customUUID : @"2221")];
    if (customUUID)
        incrementUuid16(receive_uuid, 1);
    send_uuid = [CBUUID UUIDWithString:(customUUID ? customUUID : @"2222")];
    if (customUUID)
        incrementUuid16(send_uuid, 2);
    disconnect_uuid = [CBUUID UUIDWithString:(customUUID ? customUUID : @"2223")];
    if (customUUID)
        incrementUuid16(disconnect_uuid, 3);
    peripheral.delegate = self;
    
    [peripheral discoverServices:[NSArray arrayWithObject:service_uuid]];
}

#pragma mark - CBPeripheralDelegate methods

- (void)peripheral:(CBPeripheral *)_peripheral didDiscoverServices:(NSError *)error
{
    NSLog(@"didDiscoverServices");
    
    for (CBService *service in peripheral.services) {
        if ([service.UUID isEqual:service_uuid])
        {
            NSArray *characteristics = [NSArray arrayWithObjects:receive_uuid, send_uuid, disconnect_uuid, nil];
            [peripheral discoverCharacteristics:characteristics forService:service];
        }
    }
}

- (void)peripheral:(CBPeripheral *)_peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error
{
    NSLog(@"didDiscoverCharacteristicsForService");
    for (CBService *service in peripheral.services) {
        if ([service.UUID isEqual:service_uuid]) {
            for (CBCharacteristic *characteristic in service.characteristics) {
                if ([characteristic.UUID isEqual:receive_uuid]) {
                    [peripheral setNotifyValue:YES forCharacteristic:characteristic];
                } else if ([characteristic.UUID isEqual:send_uuid]) {
                    send_characteristic = characteristic;
                } else if ([characteristic.UUID isEqual:disconnect_uuid]) {
                    disconnect_characteristic = characteristic;
                }
            }
            
            loadedService = true;
            [rfduinoManager loadedServiceRFduino:self];
        }
    }
}

- (void)peripheral:(CBPeripheral *)aPeripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    NSLog(@"didUpdateValueForCharacteristic");
    if ([characteristic.UUID isEqual:receive_uuid]) {
        SEL didReceive = @selector(didReceive:);
        if ([delegate respondsToSelector:didReceive]) {
            [delegate didReceive:characteristic.value];
        }
    }
}

#pragma mark - RFduino methods

- (void)send:(NSData *)data
{
    if (! loadedService) {
        @throw [NSException exceptionWithName:@"sendData" reason:@"please wait for ready callback" userInfo:nil];
    }
    
    if ([data length] > max_data) {
        @throw [NSException exceptionWithName:@"sendData" reason:@"max data size exceeded" userInfo:nil];
    }
    
    [peripheral writeValue:data forCharacteristic:send_characteristic type:CBCharacteristicWriteWithoutResponse];
}

- (void)disconnect
{
    NSLog(@"rfduino disconnect");
    
    if (loadedService) {
        NSLog(@"writing to disconnect characteristic");
        // fix for iOS SDK 7.0 - at least one byte must now be transferred
        uint8_t flag = 1;
        NSData *data = [NSData dataWithBytes:(void*)&flag length:1];
        [peripheral writeValue:data forCharacteristic:disconnect_characteristic type:CBCharacteristicWriteWithoutResponse];
    }
    
    [rfduinoManager disconnectRFduino:self];
}

@end
