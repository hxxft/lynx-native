
#import <Foundation/Foundation.h>


@interface LynxFilePathUtility : NSObject


+ (NSString*)toFilePath:(NSString*) fileUrl;
+ (BOOL)isFilePath:(NSString*) url;

@end
