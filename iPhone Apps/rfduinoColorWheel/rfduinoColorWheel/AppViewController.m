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

#include <QuartzCore/QuartzCore.h>

#import "AppViewController.h"
#import "UIImage+Extras.h"

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

        [[self navigationItem] setTitle:@"RFduino ColorWheel"];
    }
    return self;
}

- (void)manualLayout
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    
    if (UIDeviceOrientationIsLandscape(self.interfaceOrientation)) {
        if (rect.size.height >= 1024) {  // iPad
            colorWheel.frame = CGRectMake(20,50,600,600);
            
            colorSwatch.frame = CGRectMake(667,260,330,16);
            
            rLabel.frame = CGRectMake(667,299,21,21);
            gLabel.frame = CGRectMake(667,347,21,21);
            bLabel.frame = CGRectMake(667,395,21,21);
            
            rSlider.frame = CGRectMake(694,300,242,23);
            gSlider.frame = CGRectMake(694,348,242,23);
            bSlider.frame = CGRectMake(694,396,242,23);
            
            rValue.frame = CGRectMake(950,296,45,30);
            gValue.frame = CGRectMake(950,344,45,30);
            bValue.frame = CGRectMake(950,392,45,30);
        } else if (rect.size.height >= 568) {  // iPhone 5
            colorWheel.frame = CGRectMake(7,7,250,250);
            
            colorSwatch.frame = CGRectMake(267,60,280,16);
            
            rLabel.frame = CGRectMake(267,89,21,21);
            gLabel.frame = CGRectMake(267,127,21,21);
            bLabel.frame = CGRectMake(267,165,21,21);
            
            rSlider.frame = CGRectMake(294,90,202,23);
            gSlider.frame = CGRectMake(294,128,202,23);
            bSlider.frame = CGRectMake(294,166,202,23);
            
            rValue.frame = CGRectMake(502,86,45,30);
            gValue.frame = CGRectMake(502,124,45,30);
            bValue.frame = CGRectMake(502,162,45,30);
        } else {  // iPhone 4S
            colorWheel.frame = CGRectMake(0,7,250,250);
            
            colorSwatch.frame = CGRectMake(260,60,213,16);
            
            rLabel.frame = CGRectMake(260,89,21,21);
            gLabel.frame = CGRectMake(260,127,21,21);
            bLabel.frame = CGRectMake(260,165,21,21);
            
            rSlider.frame = CGRectMake(287,90,135,23);
            gSlider.frame = CGRectMake(287,128,135,23);
            bSlider.frame = CGRectMake(287,166,135,23);
            
            rValue.frame = CGRectMake(428,86,45,30);
            gValue.frame = CGRectMake(428,124,45,30);
            bValue.frame = CGRectMake(428,162,45,30);
        }
    } else {  // UIDeviceOrientationIsPortrait(self.interfaceOrientation)
        if (rect.size.height >= 1024) {  // iPad
            colorWheel.frame = CGRectMake(80,50,600,600);
            
            colorSwatch.frame = CGRectMake(40,704,685,16);
            
            rLabel.frame = CGRectMake(40,740,21,21);
            gLabel.frame = CGRectMake(40,788,21,21);
            bLabel.frame = CGRectMake(40,836,21,21);
            
            rSlider.frame = CGRectMake(67,741,602,23);
            gSlider.frame = CGRectMake(67,789,602,23);
            bSlider.frame = CGRectMake(67,837,602,23);
            
            rValue.frame = CGRectMake(680,737,45,30);
            gValue.frame = CGRectMake(680,785,45,30);
            bValue.frame = CGRectMake(680,833,45,30);
        } else if (rect.size.height >= 568) {  // iPhone 5
            colorWheel.frame = CGRectMake(35,20,250,250);
            
            colorSwatch.frame = CGRectMake(20,304,280,16);
            
            rLabel.frame = CGRectMake(20,330,21,21);
            gLabel.frame = CGRectMake(20,368,21,21);
            bLabel.frame = CGRectMake(20,406,21,21);
            
            rSlider.frame = CGRectMake(47,331,202,23);
            gSlider.frame = CGRectMake(47,369,202,23);
            bSlider.frame = CGRectMake(47,407,202,23);
            
            rValue.frame = CGRectMake(255,327,45,30);
            gValue.frame = CGRectMake(255,365,45,30);
            bValue.frame = CGRectMake(255,403,45,30);
        } else {  // iPhone 4S
            colorWheel.frame = CGRectMake(35,13,250,250);
            
            colorSwatch.frame = CGRectMake(20,274,280,16);
            
            rLabel.frame = CGRectMake(20,300,21,21);
            gLabel.frame = CGRectMake(20,338,21,21);
            bLabel.frame = CGRectMake(20,376,21,21);
            
            rSlider.frame = CGRectMake(47,301,202,23);
            gSlider.frame = CGRectMake(47,339,202,23);
            bSlider.frame = CGRectMake(47,377,202,23);
            
            rValue.frame = CGRectMake(255,297,45,30);
            gValue.frame = CGRectMake(255,335,45,30);
            bValue.frame = CGRectMake(255,373,45,30);
        }
    }
    
    if (colorWheel.frame.size.width != colorWheel.image.size.width) {
        UIImage *image1 = [UIImage imageNamed:@"colorWheel1.png"];
        UIImage *image2 = [image1 imageByScalingProportionallyToSize:colorWheel.frame.size];
        [colorWheel setImage:image2];
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Do any additional setup after loading the view from its nib.

    [rfduino setDelegate:self];
    
    // delegate needed so UITextField textFieldShouldReturn can dismiss the keyboard when the return key is pressed
    rValue.delegate = self;
    gValue.delegate = self;
    bValue.delegate = self;
    
    UIColor *start = [UIColor colorWithRed:102/255.0 green:102/255.0 blue:102/255.0 alpha:1.0];
    UIColor *stop = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:1.0];

    CAGradientLayer *gradient = [CAGradientLayer layer];
    //gradient.frame = [self.view bounds];
    gradient.frame = CGRectMake(0, 0, 1024, 1024);
    gradient.colors = [NSArray arrayWithObjects:(id)start.CGColor, (id)stop.CGColor, nil];
    [self.view.layer insertSublayer:gradient atIndex:0];
    
    // self.view.backgroundColor=[UIColor colorWithPatternImage:[UIImage imageNamed:@"background.png"]];
    
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

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
	UITouch* touch = [touches anyObject];
	CGPoint point = [touch locationInView:colorWheel];
    if (CGRectContainsPoint(colorWheel.frame, point)) {
        UIColor *color = [colorWheel.image pixelColorAt:point];
        [self pickedColor:color];
    }
}

- (void)setColor
{
    NSLog(@"colors: RGB %f %f %f", red, green, blue);

    UIColor *color = [UIColor colorWithRed:red green:green blue:blue alpha:1.0];
    
    [colorSwatch setHighlighted:YES];
    [colorSwatch setTintColor:color];
    
    uint8_t tx[3] = { red * 255, green * 255, blue * 255 };
    NSData *data = [NSData dataWithBytes:(void*)&tx length:3];
    
    [rfduino send:data];
}

- (void)pickedColor:(UIColor*)color
{
    NSLog(@"Picked Color");
    
    const CGFloat *components = CGColorGetComponents([color CGColor]);
    
    red = components[0];
    green = components[1];
    blue = components[2];
    
    rSlider.value = red;
    gSlider.value = green;
    bSlider.value = blue;
    
    rValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(red * 255)];
    gValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(green * 255)];
    bValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(blue * 255)];
    
    [self setColor];
}

- (IBAction)rSliderChanged:(id)sender
{
    NSLog(@"rSliderChanged");
    red = [rSlider value];
    rValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(red * 255)];
    [self setColor];
}

- (IBAction)gSliderChanged:(id)sender
{
    NSLog(@"gSliderChanged");
    green = [gSlider value];
    gValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(green * 255)];
    [self setColor];
}

- (IBAction)bSliderChanged:(id)sender
{
    NSLog(@"bSliderChanged");
    blue = [bSlider value];
    bValue.text = [[NSString alloc] initWithFormat:@"%d", (int)(blue * 255)];
    [self setColor];
}

- (IBAction)rEditingDidEnd:(id)sender
{
    NSLog(@"rEditingDidEnd");
    red = rValue.text.intValue / 255.0;
    rSlider.value = red;
    [self setColor];
}

- (IBAction)gEditingDidEnd:(id)sender
{
    NSLog(@"gEditingDidEnd");
    green = gValue.text.intValue / 255.0;
    gSlider.value = green;
    [self setColor];
}

- (IBAction)bEditingDidEnd:(id)sender
{
    NSLog(@"bEditingDidEnd");
    blue = bValue.text.intValue / 255.0;
    bSlider.value = blue;
    [self setColor];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    NSLog(@"textFieldShouldReturn");
    [textField resignFirstResponder];
    return YES;
}

@end
