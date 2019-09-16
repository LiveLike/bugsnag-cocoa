//
//  LLBugsnagSessionTracker.m
//  LLBugsnag
//
//  Created by Jamie Lynch on 24/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import "BugsnagSessionTracker.h"
#import "BugsnagSessionFileStore.h"
#import "BSG_KSLogger.h"
#import "BugsnagSessionTrackingPayload.h"
#import "BugsnagSessionTrackingApiClient.h"
#import "BugsnagLogger.h"

/**
 Number of seconds in background required to make a new session
 */
NSTimeInterval const BSGNewSessionBackgroundDuration = 60;

NSString *const BSGSessionUpdateNotification = @"BugsnagSessionChanged";

@interface LLBugsnagSessionTracker ()
@property (weak, nonatomic) LLBugsnagConfiguration *config;
@property (strong, nonatomic) LLBugsnagSessionFileStore *sessionStore;
@property (strong, nonatomic) LLBugsnagSessionTrackingApiClient *apiClient;
@property (strong, nonatomic) NSDate *backgroundStartTime;

@property (strong, readwrite) LLBugsnagSession *currentSession;

/**
 * Called when a session is altered
 */
@property (nonatomic, strong, readonly) SessionTrackerCallback callback;
@end

@implementation LLBugsnagSessionTracker

- (instancetype)initWithConfig:(LLBugsnagConfiguration *)config
            postRecordCallback:(void(^)(LLBugsnagSession *))callback {
    if (self = [super init]) {
        _config = config;
        _apiClient = [[LLBugsnagSessionTrackingApiClient alloc] initWithConfig:config queueName:@"Session API queue"];
        _callback = callback;

        NSString *storePath = [LLBugsnagFileStore findReportStorePath:@"Sessions"];
        if (!storePath) {
            BSG_KSLOG_ERROR(@"Failed to initialize session store.");
        }
        _sessionStore = [LLBugsnagSessionFileStore storeWithPath:storePath];
    }
    return self;
}

#pragma mark - Creating and sending a new session

- (void)startNewSession {
    [self startNewSessionWithAutoCaptureValue:NO];
}

- (void)stopSession {
    [[self currentSession] stop];

    if (self.callback) {
        self.callback(nil);
    }
    [self postUpdateNotice];
}

- (BOOL)resumeSession {
    LLBugsnagSession *session = self.currentSession;

    if (session == nil) {
        [self startNewSessionWithAutoCaptureValue:NO];
        return NO;
    } else {
        BOOL stopped = session.isStopped;
        [session resume];
        [self postUpdateNotice];
        return stopped;
    }
}

- (LLBugsnagSession *)runningSession {
    LLBugsnagSession *session = self.currentSession;

    if (session == nil || session.isStopped) {
        return nil;
    }
    return session;
}

- (void)startNewSessionIfAutoCaptureEnabled {
    if (self.config.shouldAutoCaptureSessions  && [self.config shouldSendReports]) {
        [self startNewSessionWithAutoCaptureValue:YES];
    }
}

- (void)startNewSessionWithAutoCaptureValue:(BOOL)isAutoCaptured {
    if (self.config.sessionURL == nil) {
        bsg_log_err(@"The session tracking endpoint has not been set. Session tracking is disabled");
        return;
    }

    self.currentSession = [[LLBugsnagSession alloc] initWithId:[[NSUUID UUID] UUIDString]
                                                   startDate:[NSDate date]
                                                        user:self.config.currentUser
                                                autoCaptured:isAutoCaptured];

    [self.sessionStore write:self.currentSession];

    if (self.callback) {
        self.callback(self.currentSession);
    }
    [self postUpdateNotice];

    [self.apiClient deliverSessionsInStore:self.sessionStore];
}

- (void)registerExistingSession:(NSString *)sessionId
                      startedAt:(NSDate *)startedAt
                           user:(LLBugsnagUser *)user
                   handledCount:(NSUInteger)handledCount
                 unhandledCount:(NSUInteger)unhandledCount {
    if (sessionId == nil || startedAt == nil) {
        self.currentSession = nil;
    } else {
        self.currentSession = [[LLBugsnagSession alloc] initWithId:sessionId
                                                       startDate:startedAt
                                                            user:user
                                                    handledCount:handledCount
                                                  unhandledCount:unhandledCount];
    }
    if (self.callback) {
        self.callback(self.currentSession);
    }
    [self postUpdateNotice];
}

- (void)postUpdateNotice {
    [[NSNotificationCenter defaultCenter] postNotificationName:BSGSessionUpdateNotification
                                                        object:[self.runningSession toDictionary]];
}

#pragma mark - Handling events

- (void)handleAppBackgroundEvent {
    self.backgroundStartTime = [NSDate date];
}

- (void)handleAppForegroundEvent {
    if (self.backgroundStartTime
        && [[NSDate date] timeIntervalSinceDate:self.backgroundStartTime] >= BSGNewSessionBackgroundDuration) {
        [self startNewSessionIfAutoCaptureEnabled];
    }
    self.backgroundStartTime = nil;
}

- (void)handleHandledErrorEvent {
    LLBugsnagSession *session = [self runningSession];

    if (session == nil) {
        return;
    }

    @synchronized (session) {
        session.handledCount++;
        if (self.callback) {
            self.callback(session);
        }
        [self postUpdateNotice];
    }
}

- (void)handleUnhandledErrorEvent {
    LLBugsnagSession *session = [self runningSession];

    if (session == nil) {
        return;
    }

    @synchronized (session) {
        session.unhandledCount++;
        if (self.callback) {
            self.callback(session);
        }
        [self postUpdateNotice];
    }
}

@end
