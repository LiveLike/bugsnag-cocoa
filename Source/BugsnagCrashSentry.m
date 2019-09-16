//
//  LLBugsnagCrashSentry.m
//  Pods
//
//  Created by Jamie Lynch on 11/08/2017.
//
//

#import "BSG_KSCrashAdvanced.h"
#import "BSG_KSCrashC.h"

#import "BugsnagCrashSentry.h"
#import "BugsnagLogger.h"
#import "BugsnagSink.h"

NSUInteger const BSG_MAX_STORED_REPORTS = 12;

@implementation LLBugsnagCrashSentry

- (void)install:(LLBugsnagConfiguration *)config
      apiClient:(LLBugsnagErrorReportApiClient *)apiClient
        onCrash:(BSGReportCallback)onCrash {

    LLBugsnagSink *sink = [[LLBugsnagSink alloc] initWithApiClient:apiClient];
    [LLBSG_KSCrash sharedInstance].sink = sink;
    [LLBSG_KSCrash sharedInstance].introspectMemory = YES;
    [LLBSG_KSCrash sharedInstance].deleteBehaviorAfterSendAll =
        BSG_KSCDeleteOnSucess;
    [LLBSG_KSCrash sharedInstance].onCrash = onCrash;
    [LLBSG_KSCrash sharedInstance].maxStoredReports = BSG_MAX_STORED_REPORTS;

    if (!config.autoNotify) {
        bsg_kscrash_setHandlingCrashTypes(BSG_KSCrashTypeUserReported);
    }
    if (![[LLBSG_KSCrash sharedInstance] install]) {
        bsg_log_err(@"Failed to install crash handler. No exceptions will be "
                    @"reported!");
    }

    [sink.apiClient flushPendingData];
}

- (void)reportUserException:(NSString *)reportName
                     reason:(NSString *)reportMessage
          originalException:(NSException *)ex
               handledState:(NSDictionary *)handledState
                   appState:(NSDictionary *)appState
          callbackOverrides:(NSDictionary *)overrides
                   metadata:(NSDictionary *)metadata
                     config:(NSDictionary *)config
               discardDepth:(int)depth {

    [[LLBSG_KSCrash sharedInstance] reportUserException:reportName
                                               reason:reportMessage
                                    originalException:ex
                                         handledState:handledState
                                             appState:appState
                                    callbackOverrides:overrides
                                             metadata:metadata
                                               config:config
                                         discardDepth:depth
                                     terminateProgram:NO];
}

@end
