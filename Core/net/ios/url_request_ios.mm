// Copyright 2017 The Lynx Authors. All rights reserved.

#include "net/ios/url_request_ios.h"
#include "app/resource_manager.h"
#include "base/file_path_utility.h"
#include "base/string/platform_string_impl.h"


namespace net {
    
    URLRequest* URLRequest::Create(URLRequestContext *context,
                              const std::string &url,
                                   URLRequestDelegate *delegate) {
        return lynx_new URLRequestIOS(context, url, delegate);
    }
    
    AFHTTPSessionManager* URLRequestIOS::g_network_;
    
    URLRequestIOS::URLRequestIOS(URLRequestContext *context,
                                 const std::string& url,
                                 URLRequestDelegate* delegate)
        : URLRequest(context, url, delegate), url_(url) {
        
    }
    
    URLRequestIOS::~URLRequestIOS() {
        
    }
    
    void URLRequestIOS::Fetch() {
       
        NSString *ns_url_string =  [NSString stringWithUTF8String:url_.c_str()];
        NSURL *ns_url = [[LynxResourceManager instance] toResourceURL:ns_url_string];

        if (ns_url.fileURL) {
            [[[NSURLSession sharedSession] downloadTaskWithURL:ns_url completionHandler:^(NSURL* _Nullable location,
                                                                                        NSURLResponse * _Nullable response, NSError * _Nullable error) {
                if (ghost_delegate_ == NULL) return;
                NSData *downloadData = [NSData dataWithContentsOfURL:location];
                NSString *response_ns_str = [[NSString alloc] initWithData:downloadData encoding:NSUTF8StringEncoding];
                base::ScopedPtr<base::PlatformString> scoped_url(new base::PlatformStringImpl(ns_url_string));
                base::ScopedPtr<base::PlatformString> scoped_response(new base::PlatformStringImpl(response_ns_str));
                ghost_delegate_->DeliverSuccess(scoped_url, scoped_response);
            }] resume];
        } else {
            if (nil == g_network_)
            {
                g_network_ = [AFHTTPSessionManager manager];
                g_network_.requestSerializer.timeoutInterval = 30.0f;
                g_network_.responseSerializer = [AFHTTPResponseSerializer serializer];
                [g_network_.requestSerializer setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
                [g_network_.requestSerializer setValue:@"*/*" forHTTPHeaderField:@"Accept"];
            }
            
            [g_network_ GET:ns_url.absoluteString parameters:nil
                    progress:^(NSProgress * _Nonnull downloadProgress) {
                       
                    }
                    success:^(NSURLSessionDataTask* task, id responseObject)
                    {
                        if (ghost_delegate_ != 0) {
                            NSString *response_ns_str = [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding];
                            base::ScopedPtr<base::PlatformString> scoped_url(new base::PlatformStringImpl(ns_url_string));
                            base::ScopedPtr<base::PlatformString> scoped_response(new base::PlatformStringImpl(response_ns_str));
                            ghost_delegate_->DeliverSuccess(scoped_url, scoped_response);
                        }
                    }
                   failure:^(NSURLSessionDataTask* task, NSError* error)
                    {
                        if (ghost_delegate_ != 0) {
                            NSString *error_ns_str = error.localizedFailureReason;
                            base::ScopedPtr<base::PlatformString> scoped_url(new base::PlatformStringImpl(ns_url_string));
                            base::ScopedPtr<base::PlatformString> scoped_error(new base::PlatformStringImpl(error_ns_str));
                            ghost_delegate_->DeliverError(scoped_url, scoped_error);
                        }
                    }
            ];
        }

    }
}
