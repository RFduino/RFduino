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

/*
This is the matching iPhone for the bulk data transfer sketch.  This
application has been included for completeness, and to demonstrate
how you could verify that no data was dropped.  Its really for
educational use only - it's output is to the debug window, and it
doesn't have a UI.

If you would like to test bulk data transfer with one of the existing
apps, you can use the ColorWheel application.  Open the sketch in 
Arduino, compile and open the Serial Monitor.  Open the ColorWheel
application and connect to the sketch.  Once connected, the sketch
will start transferring the data (The ColorWheel application receives
the data, but ignores it).  After the transfer is complete, the
Serial Monitor will display the start time, end time, elapsed time,
and kbps.
*/

#import <QuartzCore/QuartzCore.h>

#import "AppViewController.h"

@implementation AppViewController
{
    int packets;
    char ch;
    int packet;
    double start;
}

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
        
        [[self navigationItem] setTitle:@"RFduino Bulk"];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
 
    [rfduino setDelegate:self];
    
    UIColor *start = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.15];
    UIColor *stop = [UIColor colorWithRed:58/255.0 green:108/255.0 blue:183/255.0 alpha:0.45];
    
    CAGradientLayer *gradient = [CAGradientLayer layer];
    gradient.frame = [self.view bounds];
    gradient.colors = [NSArray arrayWithObjects:(id)start.CGColor, (id)stop.CGColor, nil];
    [self.view.layer insertSublayer:gradient atIndex:0];
    
    packets = 500;
    ch = 'A';
    packet = 0;
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

- (void)didReceive:(NSData *)data
{
    // NSLog(@"RecievedRX");

    /*
    uint8_t *p = (uint8_t*)[data bytes];
    NSUInteger len = [data length];
    
    // NSString *s = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
    // NSLog(@"%d=%@", len, s);
    NSMutableString *s = [[NSMutableString alloc] init];
    
    [s appendFormat:@"%d - ", len];
    
    for (int i = 0; i < len; i++)
      if (isprint(p[i]))
          [s appendFormat:@"%c", p[i]];
      else
          [s appendFormat:@"{%02x}", p[i]];
    
    NSLog(@"%@", s);
    */
    
    if (packet == 0)
    {
        NSLog(@"start");
        start = CACurrentMediaTime();
    }
    
    uint8_t *p = (uint8_t*)[data bytes];
    NSUInteger len = [data length];
    
    if (len != 20)
        NSLog(@"len issue");
    for (int i = 0; i < 20; i++)
    {
        if (p[i] != ch)
            NSLog(@"ch issue");
        ch++;
        if (ch > 'Z')
            ch = 'A';
    }
    packet++;
    if (packet >= packets)
    {
        NSLog(@"end");
        double end = CACurrentMediaTime();
        float secs = (end - start);
        int bps = ((packets * 20) * 8) / secs;
        NSLog(@"start: %f", start);
        NSLog(@"end: %f", end);
        NSLog(@"elapsed: %f", secs);
        NSLog(@"kbps: %f", bps / 1000.0);
    }
}

@end
