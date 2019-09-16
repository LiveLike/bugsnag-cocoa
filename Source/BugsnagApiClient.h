//
// Created by Jamie Lynch on 04/12/2017.
// Copyright (c) 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LLBugsnagConfiguration;

typedef void (^RequestCompletion)(NSUInteger reportCount, BOOL success, NSError *error);

@interface LLBugsnagApiClient : NSObject

- (instancetype)initWithConfig:(LLBugsnagConfiguration *)configuration
                     queueName:(NSString *)queueName;

/**
 * Send outstanding reports
 */
- (void)flushPendingData;

- (NSOperation *)deliveryOperation;

- (void)sendItems:(NSUInteger)count
      withPayload:(NSDictionary *)payload
            toURL:(NSURL *)url
          headers:(NSDictionary *)headers
     onCompletion:(RequestCompletion)onCompletion;

@property(readonly) NSOperationQueue *sendQueue;
@property(readonly) LLBugsnagConfiguration *config;


@end
