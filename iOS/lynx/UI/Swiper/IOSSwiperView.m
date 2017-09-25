// Copyright 2017 The Lynx Authors. All rights reserved.

#import "IOSSwiperView.h"

@interface IOSSwiperView () <UIScrollViewDelegate, UIGestureRecognizerDelegate>

@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) NSMutableDictionary *children;
@property (nonatomic, strong) NSMutableSet *itemViewPool;
@property (nonatomic, assign) NSInteger previousItemIndex;
@property (nonatomic, assign) CGPoint previousContentOffset;
@property (nonatomic, assign) CGSize itemSize;
@property (nonatomic, assign) BOOL suppressScrollEvent;
@property (nonatomic, assign) NSTimeInterval scrollDuration;
@property (nonatomic, assign, getter = isScrolling) BOOL scrolling;
@property (nonatomic, assign) NSTimeInterval startTime;
@property (nonatomic, assign) NSTimeInterval lastTime;
@property (nonatomic, assign) CGFloat startOffset;
@property (nonatomic, assign) CGFloat endOffset;
@property (nonatomic, assign) CGFloat lastUpdateOffset;
@property (nonatomic, strong) NSTimer *timer;

@end

@implementation IOSSwiperView

- (instancetype)init {
    self = [super init];
    if (self) {
        self.bounces = false;
        self.pagingEnabled = true;
        self.scrollEnabled = true;
        self.showsHorizontalScrollIndicator = false;
        self.showsVerticalScrollIndicator = false;
        self.scrollsToTop = false;
        
        _children = [[NSMutableDictionary alloc] init];
        
    }
    return self;
}

- (void)reloadData {
    NSArray *viewsToRemove = [_scrollView subviews];
    for (UIView *v in viewsToRemove) {
        [v removeFromSuperview];
    }
    CGSize size;
    size.width = 0;
    size.height = 0;
    for (NSInteger i = 0; i < [_dataSource numberOfViewsInSwiperView:self]; ++i) {
        CGSize childSize = [_swiperDelegate tableView:self sizeAtIndex:i];
        size.width += childSize.width;
        size.height = MAX(size.height, childSize.height);
        [self addSubview:[_dataSource swiperView:self viewAtIndex:i]];
    }
    self.contentSize = size;
}

- (void)scrollToIndex:(NSInteger)index {
    
}

- (void)scrollToIndex:(NSInteger)index duration:(NSTimeInterval)duration {
    
}

- (UIView *)viewAtIndex:(NSInteger)index {
    return nil;
}

- (NSInteger)indexOfView:(UIView *)view {
    return 0;
}

@end