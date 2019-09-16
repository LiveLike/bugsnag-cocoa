//
// Created by Jamie Lynch on 30/11/2017.
// Copyright (c) 2017 LLBugsnag. All rights reserved.
//

#import "BugsnagSessionTrackingApiClient.h"
#import "BugsnagConfiguration.h"
#import "BugsnagSessionTrackingPayload.h"
#import "BugsnagSessionFileStore.h"
#import "BugsnagLogger.h"
#import "BugsnagSession.h"
#import "BSG_RFC3339DateTool.h"


@implementation LLBugsnagSessionTrackingApiClient

- (NSOperation *)deliveryOperation {
    return [NSOperation new];
}

- (void)deliverSessionsInStore:(LLBugsnagSessionFileStore *)store {
    NSString *apiKey = [self.config.apiKey copy];
    NSURL *sessionURL = [self.config.sessionURL copy];

    [self.sendQueue addOperationWithBlock:^{
        if (!apiKey) {
            bsg_log_err(@"No API key set. Refusing to send sessions.");
            return;
        }

        NSArray *fileIds = [store fileIds];

        if (fileIds.count <= 0) {
            return;
        }

        NSMutableArray *sessions = [NSMutableArray new];

        for (NSDictionary *dict in [store allFiles]) {
            [sessions addObject:[[LLBugsnagSession alloc] initWithDictionary:dict]];
        }
        LLBugsnagSessionTrackingPayload *payload = [[LLBugsnagSessionTrackingPayload alloc] initWithSessions:sessions];
        NSUInteger sessionCount = payload.sessions.count;
        NSMutableDictionary *data = [payload toJson];

        for (BeforeSendSession cb in self.config.beforeSendSessionBlocks) {
            cb(data);
        }

        if (sessionCount > 0) {
            NSDictionary *HTTPHeaders = @{
                                          @"Bugsnag-Payload-Version": @"1.0",
                                          @"Bugsnag-API-Key": apiKey,
                                          @"Bugsnag-Sent-At": [LLBSG_RFC3339DateTool stringFromDate:[NSDate new]]
                                          };
            [self sendItems:sessions.count
                withPayload:data
                      toURL:sessionURL
                    headers:HTTPHeaders
               onCompletion:^(NSUInteger sentCount, BOOL success, NSError *error) {
                  if (success && error == nil) {
                      bsg_log_info(@"Sent %lu sessions to Bugsnag", (unsigned long) sessionCount);

                      for (NSString *fileId in fileIds) {
                          [store deleteFileWithId:fileId];
                      }
                  } else {
                      bsg_log_warn(@"Failed to send sessions to Bugsnag: %@", error);
                  }
              }];
        }
    }];
}

@end
