// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>

@protocol IOSSwiperViewDataSource, IOSSwiperViewDelegate;

@interface IOSSwiperView : UIScrollView

@property(nonatomic, weak) id<IOSSwiperViewDataSource> dataSource;
@property(nonatomic, weak) id<IOSSwiperViewDelegate> swiperDelegate;

- (void)reloadData;
- (void)scrollToIndex:(NSInteger)index;
- (void)scrollToIndex:(NSInteger)index duration:(NSTimeInterval)duration;
- (UIView *)viewAtIndex:(NSInteger)index;
- (NSInteger)indexOfView:(UIView *)view;

@end


@protocol IOSSwiperViewDataSource <NSObject>

- (NSInteger)numberOfViewsInSwiperView:(IOSSwiperView *)swiperView;
- (UIView *)swiperView:(IOSSwiperView *)swiperView viewAtIndex:(NSInteger)index;

@end

@protocol IOSSwiperViewDelegate <NSObject>
@optional

- (CGSize)tableView:(IOSSwiperView *)swiperView sizeAtIndex:(NSInteger)index;
- (void)swiperViewDidScroll:(IOSSwiperView *)swiperView;
- (void)swiperViewCurrentItemIndexDidChange:(IOSSwiperView *)swiperView;
- (void)swiperViewWillBeginDragging:(IOSSwiperView *)swiperView;
- (void)swiperViewDidEndDragging:(IOSSwiperView *)swiperView willDecelerate:(BOOL)decelerate;
- (void)swiperViewWillBeginDecelerating:(IOSSwiperView *)swiperView;
- (void)swiperViewDidEndDecelerating:(IOSSwiperView *)swiperView;
- (void)swiperViewDidEndScrollingAnimation:(IOSSwiperView *)swiperView;
- (BOOL)swiperView:(IOSSwiperView *)swiperView shouldSelectItemAtIndex:(NSInteger)index;
- (void)swiperView:(IOSSwiperView *)swiperView didSelectItemAtIndex:(NSInteger)index;

@end