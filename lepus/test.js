// var kMinScale = 0.9
// var kMaxScale = 1
// var kMarginLeft = 40
// var kImgWidth = 450
// var kTotalIndex = 4
// var kScrollWrapTag = 100
// var kTouchSlop = 0

// var curIndex = 0
// var startTouchX = 0
// var startTouchY = 0
// var curTouchX = 0
// var totalMoveX = 0
// var preTotalMoveX = 0


// function init(tag) {
//   if (tag != curIndex && tag != kScrollWrapTag) {
//     setScaleX(kMinScale)
//     setScaleY(kMinScale)
//   }
// }

// function onDispatchTouchEvent(tag, type, touchX, touchY) {
//     curTouchX = touchX
//     switch (type) {
//       case 0:
//         startTouchX = touchX
//         break
//       case 1:
//         preTotalMoveX = totalMoveX
//         break
//       case 2:
//         totalMoveX = curTouchX - startTouchX + preTotalMoveX
//         curIndex = totalMoveX / (kImgWidth + kMarginLeft)
//         curIndex = curIndex % 10
//         break
//       case 3:
//         break
//       default:
//         break
//     }
//     print(1)
//     setConsumed(true)
// }

// function onTouchEventForImg(tag, touchX, touchY) {
//     print(2)
//     setTranslateX(-totalMoveX)
//     if (curIndex + 1 == tag) {
//       var offsetX = totalMoveX - (kImgWidth + kMarginLeft) * curIndex
//       var rate = offsetX / (kImgWidth + kMarginLeft)
//       var scale = rate * (kMaxScale - kMinScale) + kMinScale
//       setScaleX(scale)
//       setScaleY(scale)
//     } else {
//       setScaleX(kMaxScale)
//       setScaleX(kMaxScale)
//     }
// }

// init(0)
// onDispatchTouchEvent(0, 0, 11, 22)
// onDispatchTouchEvent(2, 2, 11, 22)
var i = 0
var n = 100*1000*1000
while(i < n) {
  ++i
}
