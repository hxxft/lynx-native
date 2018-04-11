// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_SWIPER_LYNX_SWIPER_VIEW_H_
#define UI_WIDGET_SWIPER_LYNX_SWIPER_VIEW_H_

#import <UIKit/UIKit.h>

@protocol LxSwiperViewDataSource, LxSwiperViewDelegate;

@interface LxSwiperView : UIScrollView

@property(nonatomic, weak) id<LxSwiperViewDataSource> dataSource;
@property(nonatomic, weak) id<LxSwiperViewDelegate> swiperDelegate;

- (void)reloadData;
- (void)scrollToIndex:(NSInteger)index;
- (void)scrollToIndex:(NSInteger)index duration:(NSTimeInterval)duration;
- (UIView *)viewAtIndex:(NSInteger)index;
- (NSInteger)indexOfView:(UIView *)view;

@end


@protocol LxSwiperViewDataSource <NSObject>

- (NSInteger)numberOfViewsInSwiperView:(LxSwiperView *)swiperView;
- (UIView *)swiperView:(LxSwiperView *)swiperView viewAtIndex:(NSInteger)index;

@end

@protocol LxSwiperViewDelegate <NSObject>
@optional

- (CGSize)tableView:(LxSwiperView *)swiperView sizeAtIndex:(NSInteger)index;
- (void)swiperViewDidScroll:(LxSwiperView *)swiperView;
- (void)swiperViewCurrentItemIndexDidChange:(LxSwiperView *)swiperView;
- (void)swiperViewWillBeginDragging:(LxSwiperView *)swiperView;
- (void)swiperViewDidEndDragging:(LxSwiperView *)swiperView willDecelerate:(BOOL)decelerate;
- (void)swiperViewWillBeginDecelerating:(LxSwiperView *)swiperView;
- (void)swiperViewDidEndDecelerating:(LxSwiperView *)swiperView;
- (void)swiperViewDidEndScrollingAnimation:(LxSwiperView *)swiperView;
- (BOOL)swiperView:(LxSwiperView *)swiperView shouldSelectItemAtIndex:(NSInteger)index;
- (void)swiperView:(LxSwiperView *)swiperView didSelectItemAtIndex:(NSInteger)index;

@end

#endif  // UI_WIDGET_SWIPER_LYNX_SWIPER_VIEW_H_
