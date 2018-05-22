var Clipboard = {}
Clipboard.getString = function(callback) {
    exec('NetInfo', 'getString', [], callback, null)
}

Clipboard.setString = function(content) {
    exec('NetInfo', 'setString', [content], null, null)
}

