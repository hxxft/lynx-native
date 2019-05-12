document.__createElement = document.createElement


document.createElement = function (tag_name) {
    // try {
    //     a()
    // }catch(e) {
    //     console.log(e.stack)
    // }
    var element = document.__createElement(tag_name)
    return element
}