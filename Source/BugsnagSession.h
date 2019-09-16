//
//  LLBugsnagSession.h
//  LLBugsnag
//
//  Created by Jamie Lynch on 24/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BugsnagUser.h"

@interface LLBugsnagSession : NSObject

- (_Nonnull instancetype)initWithId:(NSString *_Nonnull)sessionId
                 startDate:(NSDate *_Nonnull)startDate
                      user:(LLBugsnagUser *_Nullable)user
              autoCaptured:(BOOL)autoCaptured;

- (_Nonnull instancetype)initWithDictionary:(NSDictionary *_Nonnull)dict;

- (_Nonnull instancetype)initWithId:(NSString *_Nonnull)sessionId
                          startDate:(NSDate *_Nonnull)startDate
                               user:(LLBugsnagUser *_Nullable)user
                       handledCount:(NSUInteger)handledCount
                     unhandledCount:(NSUInteger)unhandledCount;

/**
 * Representation used in report payloads
 */
- (NSDictionary *_Nonnull)toJson;

/**
 * Full representation of a session suitable for creating an identical session
 * using initWithDictionary
 */
- (NSDictionary *_Nonnull)toDictionary;
- (void)stop;
- (void)resume;

@property(readonly) NSString *_Nonnull sessionId;
@property(readonly) NSDate *_Nonnull startedAt;
@property(readonly) LLBugsnagUser *_Nullable user;
@property(readonly) BOOL autoCaptured;
@property(readonly, getter=isStopped) BOOL stopped;

@property NSUInteger unhandledCount;
@property NSUInteger handledCount;

@end
