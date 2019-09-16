//
//  LLBugsnagCrashSentry.h
//  Pods
//
//  Created by Jamie Lynch on 11/08/2017.
//
//

#import <Foundation/Foundation.h>

#import "BSG_KSCrashReportWriter.h"
#import "BugsnagConfiguration.h"
#import "BugsnagErrorReportApiClient.h"

@interface LLBugsnagCrashSentry : NSObject

- (void)install:(LLBugsnagConfiguration *)config
      apiClient:(LLBugsnagErrorReportApiClient *)apiClient
        onCrash:(BSGReportCallback)onCrash;

- (void)reportUserException:(NSString *)reportName
                     reason:(NSString *)reportMessage
          originalException:(NSException *)ex
               handledState:(NSDictionary *)handledState
                   appState:(NSDictionary *)appState
          callbackOverrides:(NSDictionary *)overrides
                   metadata:(NSDictionary *)metadata
                     config:(NSDictionary *)config
               discardDepth:(int)depth;

@end
