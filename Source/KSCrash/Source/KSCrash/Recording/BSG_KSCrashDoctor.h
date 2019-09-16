//
//  LLBSG_KSCrashDoctor.h
//  LLBSG_KSCrash
//
//  Created by Karl Stenerud on 2012-11-10.
//  Copyright (c) 2012 Karl Stenerud. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LLBSG_KSCrashDoctor : NSObject

+ (LLBSG_KSCrashDoctor *)doctor;

- (NSString *)diagnoseCrash:(NSDictionary *)crashReport;

@end
