var exec = require('../exec').exec

var Clipboard = {}
Clipboard.getString = function() {
    return new Promise((resolve, reject)=>{
                       exec('Clipboard', 'getString', [], function(content){ resolve(content) }, null)
                       });
}

Clipboard.setString = function(content) {
    exec('Clipboard', 'setString', [content], null, null)
}

module.exports = Clipboard

