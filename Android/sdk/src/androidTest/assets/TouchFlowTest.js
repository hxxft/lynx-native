document.body.setStyle({
    backgroundColor: 'white',
    flexDirection: 'column'
});
// common with fix child
var parent = document.createElement('view');
parent.setStyle({
    width: 100,
    height: 100,
    backgroundColor: '#ccc'
});
var child = document.createElement('view');
child.setStyle({
    width: 50,
    height: 50,
    backgroundColor: '#222'
});
var fixedChild = document.createElement('view');
fixedChild.setStyle({
    width: 10,
    height: 10,
    position: 'fixed',
});
fixedChild.addEventListener('touchstart', function() {
    TestResultRecorder.record('fixed', 'touchstart');
});
parent.appendChild(fixedChild);
parent.appendChild(child);
document.body.appendChild(parent);

// scrollview
var scrollview = document.createElement('scrollview');
scrollview.setStyle({
    backgroundColor: '#ccc',
    width: 200,
    height: 100,
});
var childInScrollView = document.createElement('view');
childInScrollView.setStyle({
    backgroundColor: '#222',
    height: 100,
    width: 100
});
scrollview.addEventListener('scroll', function() {
    TestResultRecorder.record('scroll', 'scroll');
});
childInScrollView.addEventListener('touchmove.prevent', function() {
    TestResultRecorder.record('child', 'touchmove');
});
scrollview.appendChild(childInScrollView);
for (var i = 0; i < 4; ++i) {
    var temp = document.createElement('view');
    temp.setStyle({
        height: 100,
        width: 150
    });
    scrollview.appendChild(temp);
}
document.body.appendChild(scrollview);


var index = 0;
var recordResult = function(e, tag) {
    TestResultRecorder.record(tag, index++ + "");
};

var addEventListener1 = function(element, tag, type) {
    element.addEventListener(type, function(e) {
        recordResult(e, tag);
    });
}

var addEventListenerForParent = function(type) {
    // parent touch test
    addEventListener1(parent, 'parent', type);
}

var addEventListenerForChild = function(type) {
    // child touch test
    addEventListener1(child, 'child', type);
}
