document.body.setStyle({
    backgroundColor: 'white',
    flexDirection: 'column'
});
// 构建测试页面
var view = document.createElement('view');
view.setStyle({
    marginTop: 50,
    marginLeft: 50,
    width: 100,
    height: 100,
    backgroundColor: '#ccc'
})
document.body.appendChild(view);

// 收集信息
var recordResult = function(e, tag) {
    console.log('recordResult');

    TestResultRecorder.record('tag', tag);
    TestResultRecorder.record('type', e.type);
    TestResultRecorder.record('touches.length', e.touches.length + '');
    TestResultRecorder.record('changedTouches.length', e.changedTouches.length + '');
    for (var i = 0; i < e.touches.length; ++i) {
        TestResultRecorder.record('touches[' + i + '].clientX', e.touches[i].clientX + '');
        TestResultRecorder.record('touches[' + i + '].clientY', e.touches[i].clientY + '');
        TestResultRecorder.record('touches[' + i + '].screenX', e.touches[i].screenX + '');
        TestResultRecorder.record('touches[' + i + '].screenY', e.touches[i].screenY + '');
    }
    for (var i = 0; i < e.changedTouches.length; ++i) {
        TestResultRecorder.record('changedTouches[' + i + '].clientX', e.changedTouches[i].clientX + '');
        TestResultRecorder.record('changedTouches[' + i + '].clientY', e.changedTouches[i].clientY + '');
        TestResultRecorder.record('changedTouches[' + i + '].screenX', e.changedTouches[i].screenX + '');
        TestResultRecorder.record('changedTouches[' + i + '].screenY', e.changedTouches[i].screenY + '');
    }

};

var addEventListener1 = function(element, tag, type) {
    element.addEventListener(type, function(e) {
        console.log(tag +' event happern');

        recordResult(e, tag);
    });
}

// body touch info test
addEventListener1(document.body, 'body', 'touchstart');
addEventListener1(document.body, 'body', 'touchmove');
addEventListener1(document.body, 'body', 'touchend');
addEventListener1(document.body, 'body', 'touchcancel');

// view touch info test
addEventListener1(view, 'view', 'touchstart.stop');
addEventListener1(view, 'view', 'touchmove.stop');
addEventListener1(view, 'view', 'touchend.stop');
addEventListener1(view, 'view', 'touchcancel.stop');



