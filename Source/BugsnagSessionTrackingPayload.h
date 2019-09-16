//
//  LLBugsnagSessionTrackingPayload.h
//  LLBugsnag
//
//  Created by Jamie Lynch on 27/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BugsnagSession.h"

@interface LLBugsnagSessionTrackingPayload : NSObject

- (instancetype)initWithSessions:(NSArray<LLBugsnagSession *> *)sessions;

- (NSMutableDictionary *)toJson;

@property NSArray<LLBugsnagSession *> *sessions;

@end
