//
//  LLBSGConnectivity.h
//
//  Created by Jamie Lynch on 2017-09-04.
//
//  Copyright (c) 2017 LLBugsnag, Inc. All rights reserved.
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
#import <SystemConfiguration/SystemConfiguration.h>

@class LLBSGConnectivity;

typedef void (^ConnectivityChange)(LLBSGConnectivity *connectivity);

@interface LLBSGConnectivity : NSObject

@property(nonatomic, copy) ConnectivityChange connectivityChangeBlock;

- (instancetype)initWithURL:(NSURL *)url
                changeBlock:(ConnectivityChange)changeBlock;
- (void)startWatchingConnectivity;
- (void)stopWatchingConnectivity;

@end
