//
// Created by Jamie Lynch on 30/11/2017.
// Copyright (c) 2017 LLBugsnag. All rights reserved.
//

#import "BugsnagSessionFileStore.h"
#import "BSG_KSLogger.h"

static NSString *const kSessionStoreSuffix = @"-Session-";

@implementation LLBugsnagSessionFileStore

+ (LLBugsnagSessionFileStore *)storeWithPath:(NSString *)path {
    return [[self alloc] initWithPath:path
                       filenameSuffix:kSessionStoreSuffix];
}

- (void)write:(LLBugsnagSession *)session {
    // serialise session
    NSString *filepath = [self pathToFileWithId:session.sessionId];
    NSDictionary *dict = [session toJson];

    NSError *error;
    NSData *json = [NSJSONSerialization dataWithJSONObject:dict options:0 error:&error];

    if (error != nil || ![json writeToFile:filepath atomically:YES]) {
        BSG_KSLOG_ERROR(@"Failed to write session %@", error);
        return;
    }
}


@end
