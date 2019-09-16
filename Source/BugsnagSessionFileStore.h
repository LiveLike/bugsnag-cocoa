//
// Created by Jamie Lynch on 30/11/2017.
// Copyright (c) 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BugsnagFileStore.h"
#import "BugsnagSession.h"

@interface LLBugsnagSessionFileStore : LLBugsnagFileStore
+ (LLBugsnagSessionFileStore *)storeWithPath:(NSString *)path;

- (void)write:(LLBugsnagSession *)session;

@end
