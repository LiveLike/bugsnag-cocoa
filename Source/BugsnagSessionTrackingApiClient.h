//
// Created by Jamie Lynch on 30/11/2017.
// Copyright (c) 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BugsnagApiClient.h"

@class LLBugsnagSessionFileStore;


@interface LLBugsnagSessionTrackingApiClient : LLBugsnagApiClient


/**
 Asynchronously delivers sessions written to the store

 @param store The store containing the sessions to deliver
 */
- (void)deliverSessionsInStore:(LLBugsnagSessionFileStore *)store;
@end
