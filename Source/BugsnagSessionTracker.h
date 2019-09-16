//
//  LLBugsnagSessionTracker.h
//  LLBugsnag
//
//  Created by Jamie Lynch on 24/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BugsnagSession.h"
#import "BugsnagConfiguration.h"

@class LLBugsnagSessionTrackingApiClient;

typedef void (^SessionTrackerCallback)(LLBugsnagSession *newSession);

extern NSString *const BSGSessionUpdateNotification;

@interface LLBugsnagSessionTracker : NSObject

/**
 Create a new session tracker

 @param config The LLBugsnag configuration to use
 @param callback A callback invoked each time a new session is started
 @return A new session tracker
 */
- (instancetype)initWithConfig:(LLBugsnagConfiguration *)config
            postRecordCallback:(void(^)(LLBugsnagSession *))callback;

/**
 Record and send a new session
 */
- (void)startNewSession;

- (void)stopSession;
- (BOOL)resumeSession;

/**
 Record a new auto-captured session if neededed. Auto-captured sessions are only
 recorded and sent if -[LLBugsnagConfiguration shouldAutoCaptureSessions] is YES
 */
- (void)startNewSessionIfAutoCaptureEnabled;

/**
 Update the details of the current session to account for externally reported
 session information. Current session details are included in subsequent crash
 reports.
 */
- (void)registerExistingSession:(NSString *)sessionId
                      startedAt:(NSDate *)startedAt
                           user:(LLBugsnagUser *)user
                   handledCount:(NSUInteger)handledCount
                 unhandledCount:(NSUInteger)unhandledCount;

/**
 Handle the app foregrounding event. If more than 30s has elapsed since being
 sent to the background, records a new session if session auto-capture is
 enabled.
 Must be called from the main thread.
 */
- (void)handleAppForegroundEvent;

/**
 Handle the app backgrounding event. Tracks time between foreground and
 background to determine when to automatically record a session.
 Must be called from the main thread.
 */
- (void)handleAppBackgroundEvent;

/**
 Handle some variation of LLBugsnag.notify() being called.
 Increases the number of handled errors recorded for the current session, if
 a session exists.
 */
- (void)handleHandledErrorEvent;

/**
 Handled LLBugsnag.notify() being called with an event with unhandled = YES.
 Increases the number of unhandled errors recorded for the current session, if
 a session exists.
 */
- (void)handleUnhandledErrorEvent;

/**
 * Retrieves the running session, or nil if the session is stopped or has not yet been started/resumed.
 */
@property (nonatomic, strong, readonly) LLBugsnagSession *runningSession;

@end
