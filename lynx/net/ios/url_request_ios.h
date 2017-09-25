// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef url_request_ios_h
#define url_request_ios_h

//typedef void(^bUrlDownload)(BOOL, NSData*, NSURL*);
//
//@interface NetRequest : NSObject
//
//-(BOOL) fetch:(NSURL*)url withBlock:(bUrlDownload)block;
//
//@end

#import <UIKit/UIKit.h>
#import <AFNetworking/AFNetworking.h>
#include <string>
#include "url_request.h"
#include "url_request_delegate.h"

using namespace std;
namespace net {
    class URLRequestIOS : public URLRequest {
    public:
        URLRequestIOS(URLRequestContext *context,
                      const std::string& url,
                      URLRequestDelegate* delegate);
        virtual ~URLRequestIOS();
        virtual void Fetch();
    private:
        std::string url_;
        static AFHTTPSessionManager* g_network_;
    };
}

#endif /* url_request_ios_h */
