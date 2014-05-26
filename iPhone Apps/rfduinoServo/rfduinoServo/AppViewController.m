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
        
        [[self navigationItem] setTitle:@"RFduino Servo"];
    }
    return self;
}

- (void)iPhone5PortraitLayout
{
    OutputLabel.frame = CGRectMake(104,80,113,21);
    Output.frame = CGRectMake(20,119,280,44);
    
    PositionLabel.frame = CGRectMake(40,196,241,21);
    
    ValueLabel.frame = CGRectMake(25,235,58,21);
    Value.frame = CGRectMake(97,231,83,30);
    
    Stepper.frame = CGRectMake(206,232,94,27);
    Slider.frame = CGRectMake(18,272,284,23);
    Segment.frame = CGRectMake(20,308,280,30);
}

- (void)iPhone5LandscapeLayout
{
    OutputLabel.frame = CGRectMake(230,20,113,21);
    Output.frame = CGRectMake(146,44,280,44);
    
    PositionLabel.frame = CGRectMake(167,114,241,21);
    
    ValueLabel.frame = CGRectMake(147,143,58,21);
    Value.frame = CGRectMake(213,140,83,30);
    
    Stepper.frame = CGRectMake(323,140,94,27);
    Slider.frame = CGRectMake(145,178,284,23);
    Segment.frame = CGRectMake(146,208,280,30);
}

- (void)iPhone4SPortraitLayout
{
    OutputLabel.frame = CGRectMake(104,25,113,21);
    Output.frame = CGRectMake(20,59,280,44);

    PositionLabel.frame = CGRectMake(40,136,241,21);

    ValueLabel.frame = CGRectMake(25,175,58,21);
    Value.frame = CGRectMake(97,171,83,30);

    Stepper.frame = CGRectMake(206,172,94,27);
    Slider.frame = CGRectMake(18,212,284,23);
    Segment.frame = CGRectMake(20,248,280,30);
}

- (void)iPhone4SLandscapeLayout
{
    OutputLabel.frame = CGRectMake(190,20,113,21);
    Output.frame = CGRectMake(106,44,280,44);
    
    PositionLabel.frame = CGRectMake(127,114,241,21);
    
    ValueLabel.frame = CGRectMake(107,143,58,21);
    Value.frame = CGRectMake(173,140,83,30);
    
    Stepper.frame = CGRectMake(293,140,94,27);
    Slider.frame = CGRectMake(105,178,284,23);
    Segment.frame = CGRectMake(106,208,280,30);
}

- (void)iPadPortraitLayout
{
    OutputLabel.frame = CGRectMake(328,383,113,21);
    Output.frame = CGRectMake(244,423,280,44);
    
    PositionLabel.frame = CGRectMake(264,499,241,21);
    
    ValueLabel.frame = CGRectMake(249,538,58,21);
    Value.frame = CGRectMake(321,534,83,30);
    
    Stepper.frame = CGRectMake(430,535,94,27);
    Slider.frame = CGRectMake(242,576,284,23);
    Segment.frame = CGRectMake(242,612,280,30);
}

- (void)iPadLandscapeLayout
{
    OutputLabel.frame = CGRectMake(456,255,113,21);
    Output.frame = CGRectMake(372,295,280,44);
    
    PositionLabel.frame = CGRectMake(392,371,241,21);
    
    ValueLabel.frame = CGRectMake(377,410,58,21);
    Value.frame = CGRectMake(449,406,83,30);
    
    Stepper.frame = CGRectMake(558,407,94,27);
    Slider.frame = CGRectMake(370,448,284,23);
    Segment.frame = CGRectMake(372,484,280,30);
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

    for (int i = 0; i < 4; i++)
      value[i] = 90;
    
    Value.delegate = self;

    [rfduino setDelegate:self];
    
    UIColor *start = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.15];
    UIColor *stop = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.45];
    
    CAGradientLayer *gradient = [CAGradientLayer layer];
    //gradient.frame = [self.view bounds];
    gradient.frame = CGRectMake(0, 0, 1024, 1024);
    gradient.colors = [NSArray arrayWithObjects:(id)start.CGColor, (id)stop.CGColor, nil];
    [self.view.layer insertSublayer:gradient atIndex:0];

    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self
                                   action:@selector(dismissKeyboard)];
    
    [tap setCancelsTouchesInView:NO];
    [self.view addGestureRecognizer:tap];
    
    output = 0;
    [self updateUI];
}

- (void)dismissKeyboard {
    [Value endEditing:YES];
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

- (void)updateUI
{
    int val = value[output];
    
    NSLog(@"updateUI - output=%d, value=%d", output, val);

    Output.selectedSegmentIndex = output;
    
    Value.text = [[NSString alloc] initWithFormat:@"%d", val];
    
    Stepper.value = val;

    Slider.value = val;
    //[Slider setValue:val animated:YES];
    
    if (val == 0) {
        Segment.selectedSegmentIndex = 0;
    } else if (val == 90) {
        Segment.selectedSegmentIndex = 1;
    } else if (val == 180) {
        Segment.selectedSegmentIndex = 2;
    } else {
        Segment.selectedSegmentIndex = -1;
    }
}

- (void)updateServo
{
    // the sketch allows multiple servos to be updated at the same time
    int servo = 1 << (1 + output);
    
    int val = value[output];
    
    if (val == 180) {
        val = 179;
    }
    
    NSLog(@"updateServo - output=%d, servo=%d, value=%d", output, servo, val);

    uint8_t bytes[] = { servo, val };
    
    NSData* data = [NSData dataWithBytes:(void*)&bytes length:2];
    [rfduino send:data];
}

- (IBAction)outputSegmentValueChanged:(id)sender {
    output = Output.selectedSegmentIndex;

    [self updateUI];
}

- (IBAction)textFieldEditingDidEnd:(id)sender {
    int val = Value.text.intValue;
    
    if (val < 0) {
        val = 0;
    } else if (val > 180) {
        val = 180;
    }
    
    value[output] = val;
    
    [self updateUI];
    [self updateServo];
}

- (IBAction)stepperValueChanged:(id)sender {
    value[output] = Stepper.value;
    
    [self updateUI];
    [self updateServo];
}

- (IBAction)sliderTouchUpInside:(id)sender {
    value[output] = Slider.value;
    
    [self updateUI];
    [self updateServo];
}

- (IBAction)segmentValueChanged:(id)sender {
    int val = value[output];
    
    if (Segment.selectedSegmentIndex == 0) {
        val = 0;
    } else if (Segment.selectedSegmentIndex == 1) {
        val = 90;
    } else if (Segment.selectedSegmentIndex == 2) {
        val = 180;
    }
    
    value[output] = val;
    
    [self updateUI];
    [self updateServo];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    NSLog(@"textFieldShouldReturn");
    [textField resignFirstResponder];
    return YES;
}

@end
