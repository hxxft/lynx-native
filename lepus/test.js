var b = 2
function onScrollEvent1(scrollTop, scrollLeft){
    var i = 2
    b = i + b + scrollTop
    print(b)
}


// var scaleRate = (850 - 650) / 650

// print(scaleRate)

// var topHeight = (608 - 150 * (1 + scaleRate)) * 2
// var p = 0
// var totalP = 0
// var lastScrollTop = 0
// function onDispatchScrollEvent() {
//     p = (e.target.scrollTop - lastScrollTop) / topHeight
//     print(p)
//     totalP = scrollTop / topHeight
//     if(p>1) p = 1
//     if(totalP > 1) totalP = 1
//     lastScrollTop = e.target.scrollTop
// }

// function onScrollEvent1(scrollTop, scrollLeft){
//     var i = 2
//     print(p)
//     print(750 * (i - 1) * p)
//    // translateY = -300 * (i - 1) * p
// }

// function onTouchEvent(touchX, touchY) {

// }


// function onScrollEvent2(e){
//     var i = 2
//     translateX = 750 * (i - 1) * p
//     translateY = -300 * (i - 1) * p
// }

// function onScrollEvent3(e){
//     var i = 3
//     translateX = 750 * (i - 1) * p
//     translateY = -300 * (i - 1) * p
// }
