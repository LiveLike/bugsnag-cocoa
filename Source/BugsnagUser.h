//
//  LLBugsnagUser.h
//  LLBugsnag
//
//  Created by Jamie Lynch on 24/11/2017.
//  Copyright © 2017 LLBugsnag. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LLBugsnagUser : NSObject

- (instancetype)initWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithUserId:(NSString *)userId name:(NSString *)name emailAddress:(NSString *)emailAddress;

- (NSDictionary *)toJson;

@property NSString *userId;
@property NSString *name;
@property NSString *emailAddress;

@end
