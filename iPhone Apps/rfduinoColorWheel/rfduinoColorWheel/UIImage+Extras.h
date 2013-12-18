//
//  UIImage+Extras.h
//  BitmapResearch2
//
//  Created by Steve on 8/6/13.
//  Copyright (c) 2013 Steve. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Extras)

- (UIImage *)imageByScalingProportionallyToSize:(CGSize)targetSize;
- (UIColor *)pixelColorAt:(CGPoint)point;

@end
