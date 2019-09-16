#ifndef BUGSNAG_PRIVATE_H
#define BUGSNAG_PRIVATE_H

#import "BugsnagBreadcrumb.h"

@interface LLBugsnagBreadcrumbs ()
/**
 * Reads and return breadcrumb data currently stored on disk
 */
- (NSArray *_Nullable)cachedBreadcrumbs;
@end

#endif // BUGSNAG_PRIVATE_H
