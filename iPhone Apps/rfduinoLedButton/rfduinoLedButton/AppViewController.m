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

#import <QuartzCore/QuartzCore.h>

#import "AppViewController.h"

@implementation AppViewController

@synthesize rfduino;

+ (void)load
{
    // customUUID = @"c97433f0-be8f-4dc8-b6f0-5343e6100eb4";
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        UIButton *backButton = [UIButton buttonWithType:101];  // left-pointing shape
        [backButton setTitle:@"Disconnect" forState:UIControlStateNormal];
        [backButton addTarget:self action:@selector(disconnect:) forControlEvents:UIControlEventTouchUpInside];
        
        UIBarButtonItem *backItem = [[UIBarButtonItem alloc] initWithCustomView:backButton];
        [[self navigationItem] setLeftBarButtonItem:backItem];
        
        [[self navigationItem] setTitle:@"RFduino LedBtn"];
    }
    return self;
}

- (void)iPhone5PortraitLayout
{
    label1.frame = CGRectMake(51,20,218,84);
    button1.frame = CGRectMake(124,112,73,44);
    
    label2.frame = CGRectMake(51,279,218,84);
    image1.frame = CGRectMake(123,371,75,75);
}

- (void)iPhone5LandscapeLayout
{
    label1.frame = CGRectMake(20,65,218,84);
    button1.frame = CGRectMake(93,173,73,44);
    
    label2.frame = CGRectMake(330,65,218,84);
    image1.frame = CGRectMake(402,157,75,75);
}

- (void)iPhone4SPortraitLayout
{
    label1.frame = CGRectMake(51,34,218,84);
    button1.frame = CGRectMake(124,126,73,44);
    
    label2.frame = CGRectMake(51,211,218,84);
    image1.frame = CGRectMake(123,303,75,75);
}

- (void)iPhone4SLandscapeLayout
{
    label1.frame = CGRectMake(20,45,218,84);
    button1.frame = CGRectMake(93,153,73,44);
    
    label2.frame = CGRectMake(262,45,218,84);
    image1.frame = CGRectMake(334,137,75,75);
}

- (void)iPadPortraitLayout
{
    label1.frame = CGRectMake(91,198,587,28);
    button1.frame = CGRectMake(343,242,73,44);
    
    label2.frame = CGRectMake(113,648,543,38);
    image1.frame = CGRectMake(347,694,75,75);
}

- (void)iPadLandscapeLayout
{
    label1.frame = CGRectMake(224,144,587,28);
    button1.frame = CGRectMake(476,188,73,44);
    
    label2.frame = CGRectMake(241,462,543,38);
    image1.frame = CGRectMake(475,508,75,75);
}

- (void)manualLayout
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    
    if (UIDeviceOrientationIsLandscape(self.interfaceOrientation)) {
        if (rect.size.height >= 1024) {
            [self iPadLandscapeLayout];
        } else if (rect.size.height >= 568) {
            [self iPhone5LandscapeLayout];
        } else {
            [self iPhone4SLandscapeLayout];
        }
    } else {
        if (rect.size.height >= 1024) {
            [self iPadPortraitLayout];
        } else if (rect.size.height >= 568) {
            [self iPhone5PortraitLayout];
        } else {
            [self iPhone4SPortraitLayout];
        }
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
 
    [rfduino setDelegate:self];

    UIColor *start = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.15];
    UIColor *stop = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.45];
    
    CAGradientLayer *gradient = [CAGradientLayer layer];
    //gradient.frame = [self.view bounds];
    gradient.frame = CGRectMake(0.0, 0.0, 1024.0, 1024.0);
    gradient.colors = [NSArray arrayWithObjects:(id)start.CGColor, (id)stop.CGColor, nil];
    [self.view.layer insertSublayer:gradient atIndex:0];
    
    off = [UIImage imageNamed:@"off.jpg"];
    on = [UIImage imageNamed:@"on.jpg"];
}

- (void)viewDidLayoutSubviews {
    [self manualLayout];
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [self manualLayout];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)disconnect:(id)sender
{
    NSLog(@"disconnect pressed");

    [rfduino disconnect];
}

- (void)sendByte:(uint8_t)byte
{
    uint8_t tx[1] = { byte };
    NSData *data = [NSData dataWithBytes:(void*)&tx length:1];
    [rfduino send:data];
}

- (IBAction)buttonTouchDown:(id)sender
{
    NSLog(@"TouchDown");
    [self sendByte:1];
}

- (IBAction)buttonTouchUpInside:(id)sender
{
    NSLog(@"TouchUpInside");
    [self sendByte:0];
}

- (void)didReceive:(NSData *)data
{
    NSLog(@"RecievedData");
    
    const uint8_t *value = [data bytes];
    // int len = [data length];

    NSLog(@"value = %x", value[0]);
    
    if (value[0])
        [image1 setImage:on];
    else
        [image1 setImage:off];
}

@end
