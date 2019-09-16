//
//  LLBugsnagSessionTrackingPayload.m
//  LLBugsnag
//
//  Created by Jamie Lynch on 27/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import "BugsnagSessionTrackingPayload.h"
#import "BugsnagCollections.h"
#import "BugsnagNotifier.h"
#import "Bugsnag.h"
#import "BugsnagKeys.h"
#import "BSG_KSSystemInfo.h"
#import "BugsnagKSCrashSysInfoParser.h"

@interface LLBugsnag ()
+ (LLBugsnagNotifier *)notifier;
@end

@implementation LLBugsnagSessionTrackingPayload

- (instancetype)initWithSessions:(NSArray<LLBugsnagSession *> *)sessions {
    if (self = [super init]) {
        _sessions = sessions;
    }
    return self;
}


- (NSMutableDictionary *)toJson {
    NSMutableDictionary *dict = [NSMutableDictionary new];
    NSMutableArray *sessionData = [NSMutableArray new];
    
    for (LLBugsnagSession *session in self.sessions) {
        [sessionData addObject:[session toJson]];
    }
    BSGDictInsertIfNotNil(dict, sessionData, @"sessions");
    BSGDictSetSafeObject(dict, [LLBugsnag notifier].details, BSGKeyNotifier);
    
    NSDictionary *systemInfo = [LLBSG_KSSystemInfo systemInfo];
    BSGDictSetSafeObject(dict, BSGParseAppState(systemInfo, [LLBugsnag configuration].appVersion), @"app");
    BSGDictSetSafeObject(dict, BSGParseDeviceState(systemInfo), @"device");
    return dict;
}

@end
