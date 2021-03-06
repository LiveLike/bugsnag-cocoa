//
//  LLBugsnagConfiguration.h
//
//  Created by Conrad Irwin on 2014-10-01.
//
//  Copyright (c) 2014 LLBugsnag, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall remain in place
// in this source code.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#import <Foundation/Foundation.h>

#import "BSG_KSCrashReportWriter.h"
#import "BugsnagBreadcrumb.h"
#import "BugsnagCrashReport.h"
#import "BugsnagMetaData.h"

@class LLBugsnagBreadcrumbs;
@class LLBugsnagUser;

/**
 *  A configuration block for modifying an error report
 *
 *  @param report The default report
 */
typedef void (^BugsnagNotifyBlock)(LLBugsnagCrashReport *_Nonnull report);

/**
 *  A handler for modifying data before sending it to LLBugsnag.
 *
 * beforeSendBlocks will be invoked on a dedicated
 * background queue, which will be different from the queue where the block was originally added.
 *
 *  @param rawEventData The raw event data written at crash time. This
 *                      includes data added in onCrashHandler.
 *  @param reports      The report generated from the rawEventData
 *
 *  @return YES if the report should be sent
 */
typedef bool (^BugsnagBeforeSendBlock)(NSDictionary *_Nonnull rawEventData,
                                       LLBugsnagCrashReport *_Nonnull reports);

/**
 * A configuration block for modifying a session. Intended for internal usage only.
 *
 * @param sessionPayload The session about to be delivered
 */
typedef void(^BeforeSendSession)(NSMutableDictionary *_Nonnull sessionPayload);

/**
 *  A handler for modifying data before sending it to LLBugsnag
 *
 *  @param rawEventReports The raw event data written at crash time. This
 *                         includes data added in onCrashHandler.
 *  @param report          The default report payload
 *
 *  @return the report payload intended to be sent or nil to cancel sending
 */
typedef NSDictionary *_Nullable (^BugsnagBeforeNotifyHook)(
    NSArray *_Nonnull rawEventReports, NSDictionary *_Nonnull report);

@interface LLBugsnagConfiguration : NSObject
/**
 *  The API key of a LLBugsnag project
 */
@property(readwrite, retain, nullable) NSString *apiKey;
/**
 *  The release stage of the application, such as production, development, beta
 *  et cetera
 */
@property(readwrite, retain, nullable) NSString *releaseStage;
/**
 *  Release stages which are allowed to notify LLBugsnag
 */
@property(readwrite, retain, nullable) NSArray *notifyReleaseStages;
/**
 *  A general summary of what was occuring in the application
 */
@property(readwrite, retain, nullable) NSString *context;
/**
 *  The version of the application
 */
@property(readwrite, retain, nullable) NSString *appVersion;

/**
 *  The URL session used to send requests to LLBugsnag.
 */
@property(readwrite, strong, nonnull) NSURLSession *session;

/**
 * The current user
 */
@property(retain, nullable) LLBugsnagUser *currentUser;

/**
 *  Additional information about the state of the app or environment at the
 *  time the report was generated
 */
@property(readwrite, retain, nullable) LLBugsnagMetaData *metaData;
/**
 *  Meta-information about the state of LLBugsnag
 */
@property(readwrite, retain, nullable) LLBugsnagMetaData *config;
/**
 *  Rolling snapshots of user actions leading up to a crash report
 */
@property(readonly, strong, nullable)
LLBugsnagBreadcrumbs *breadcrumbs;

/**
 *  Whether to allow collection of automatic breadcrumbs for notable events
 */
@property(readwrite) BOOL automaticallyCollectBreadcrumbs;

/**
 *  Hooks for modifying crash reports before it is sent to LLBugsnag
 */
@property(readonly, strong, nullable)
    NSArray<BugsnagBeforeSendBlock> *beforeSendBlocks;

/**
 *  Hooks for modifying sessions before they are sent to LLBugsnag. Intended for internal use only by React Native/Unity.
 */
@property(readonly, strong, nullable)
NSArray<BeforeSendSession> *beforeSendSessionBlocks;

/**
 *  Optional handler invoked when a crash or fatal signal occurs
 */
@property void (*_Nullable onCrashHandler)
    (const BSG_KSCrashReportWriter *_Nonnull writer);

/**
 *  YES if uncaught exceptions should be reported automatically
 */
@property BOOL autoNotify;

/**
 * Determines whether app sessions should be tracked automatically. By default this value is true.
 * If this value is updated after +[LLBugsnag start] is called, only subsequent automatic sessions
 * will be captured.
 */
@property BOOL shouldAutoCaptureSessions;

/**
 * Whether the app should report out of memory events which terminate the app
 * When NO, this setting overrides reportBackgroundOOMs.
 */
@property BOOL reportOOMs;

/**
 * Whether the app should report out of memory events which terminate the app
 * while the app is in the background. This setting has no effect when reportOOMs is NO.
 */
@property BOOL reportBackgroundOOMs;

/**
 * Retrieves the endpoint used to notify LLBugsnag of errors
 *
 * NOTE: If you want to set this value, you should do so via setEndpointsForNotify:sessions: instead.
 *
 * @see setEndpointsForNotify:sessions:
 */
@property(readonly, retain, nullable) NSURL *notifyURL;

/**
 * Retrieves the endpoint used to send tracked sessions to LLBugsnag
 *
 * NOTE: If you want to set this value, you should do so via setEndpointsForNotify:sessions: instead.
 *
 * @see setEndpointsForNotify:sessions:
 */
@property(readonly, retain, nullable) NSURL *sessionURL;

/**
 * Set the endpoints to send data to. By default we'll send error reports to
 * https://notify.bugsnag.com, and sessions to https://sessions.bugsnag.com, but you can
 * override this if you are using LLBugsnag Enterprise to point to your own LLBugsnag endpoint.
 *
 * Please note that it is recommended that you set both endpoints. If the notify endpoint is
 * missing, an assertion will be thrown. If the session endpoint is missing, a warning will be
 * logged and sessions will not be sent automatically.
 *
 * @param notify the notify endpoint
 * @param sessions the sessions endpoint
 *
 * @throws an assertion if the notify endpoint is not a valid URL
 */

- (void)setEndpointsForNotify:(NSString *_Nonnull)notify
                     sessions:(NSString *_Nonnull)sessions NS_SWIFT_NAME(setEndpoints(notify:sessions:));

/**
 *  Set user metadata
 *
 *  @param userId ID of the user
 *  @param name   Name of the user
 *  @param email  Email address of the user
 */
- (void)setUser:(NSString *_Nullable)userId
       withName:(NSString *_Nullable)name
       andEmail:(NSString *_Nullable)email;

/**
 *  Add a callback to be invoked before a report is sent to LLBugsnag, to
 *  change the report contents as needed
 *
 *  @param block A block which returns YES if the report should be sent
 */
- (void)addBeforeSendBlock:(BugsnagBeforeSendBlock _Nonnull)block;

/**
 *  Add a callback to be invoked before a session is sent to LLBugsnag. Intended for internal usage only.
 *
 *  @param block A block which can modify the session
 */
- (void)addBeforeSendSession:(BeforeSendSession _Nonnull)block;

/**
 * Clear all callbacks
 */
- (void)clearBeforeSendBlocks;

/**
 *  Whether reports shoould be sent, based on release stage options
 *
 *  @return YES if reports should be sent based on this configuration
 */
- (BOOL)shouldSendReports;

- (void)addBeforeNotifyHook:(BugsnagBeforeNotifyHook _Nonnull)hook
    __deprecated_msg("Use addBeforeSendBlock: instead.");
/**
 *  Hooks for processing raw report data before it is sent to LLBugsnag
 */
@property(readonly, strong, nullable)
    NSArray *beforeNotifyHooks __deprecated_msg("Use beforeNotify instead.");

- (NSDictionary *_Nonnull)errorApiHeaders;
- (NSDictionary *_Nonnull)sessionApiHeaders;

@property(retain, nullable) NSString *codeBundleId;
@property(retain, nullable) NSString *notifierType;

- (BOOL)hasValidApiKey;

@end
