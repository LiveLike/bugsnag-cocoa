//
//  LLBugsnagErrorReportApiClient.m
//  Pods
//
//  Created by Jamie Lynch on 11/08/2017.
//
//

#import "BugsnagErrorReportApiClient.h"
#import "Bugsnag.h"
#import "BugsnagLogger.h"
#import "BugsnagNotifier.h"
#import "BugsnagSink.h"
#import "BugsnagKeys.h"

@interface LLBSGDeliveryOperation : NSOperation
@end

@implementation LLBugsnagErrorReportApiClient

- (NSOperation *)deliveryOperation {
    return [LLBSGDeliveryOperation new];
}

@end

@implementation LLBSGDeliveryOperation

- (void)main {
    @autoreleasepool {
        @try {
            [[LLBSG_KSCrash sharedInstance]
                    sendAllReportsWithCompletion:^(NSUInteger sentReportCount,
                            BOOL completed, NSError *error) {
                        if (error) {
                            bsg_log_warn(@"Failed to send reports: %@", error);
                        } else if (sentReportCount > 0) {
                            bsg_log_info(@"Reports sent.");
                        }
                    }];
        } @catch (NSException *e) {
            bsg_log_err(@"Could not send report: %@", e);
        }
    }
}
@end
