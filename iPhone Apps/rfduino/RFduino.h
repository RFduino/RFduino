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

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <CoreBluetooth/CoreBluetooth.h>
#elif TARGET_OS_MAC
#import <IOBluetooth/IOBluetooth.h>
#endif

#import "RFduinoDelegate.h"

@class RFduinoManager;

char data(NSData *data);
uint8_t dataByte(NSData *data);
int dataInt(NSData *data);
float dataFloat(NSData *data);

// default NULL (NULL = previous fixed RFduino uuid)
extern NSString *customUUID;

@interface RFduino : NSObject<CBPeripheralDelegate>
{
}

@property(assign, nonatomic) id<RFduinoDelegate> delegate;

@property(strong, nonatomic) CBPeripheral *peripheral;

@property(strong, nonatomic) RFduinoManager *rfduinoManager;

@property(strong, nonatomic) NSString *name;
@property(strong, nonatomic) NSString *UUID;
@property(strong, nonatomic) NSData *advertisementData;
@property(strong, nonatomic) NSNumber *advertisementRSSI;
@property(assign, nonatomic) NSInteger advertisementPackets;
@property(strong, nonatomic) NSDate *lastAdvertisement;
@property(assign, nonatomic) NSInteger outOfRange;
- (void)connected;
- (void)disconnect;

- (void)send:(NSData *)data;

@end
