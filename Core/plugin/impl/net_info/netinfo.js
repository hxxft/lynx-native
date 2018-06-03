var exec = require('../exec').exec
var addEventListener = require('../exec').addEventListener
var removeEventListener = require('../exec').removeEventListener
var NetInfo = {}

NetInfo.getConnectInfo = function() {
    return new Promise((resolve, reject)=>{
                       exec('NetInfo', 'getConnectInfo', [], function(info){ resolve(info) }, null)
                       });}

NetInfo.isConnected = function(callback) {
    exec('NetInfo', 'isConnected', [], callback, null)
}

NetInfo.addEventListener = function(event, callback) {
    addEventListener('NetInfo', event, callback)
}

NetInfo.removeEventListener = function(event, callback) {
    removeEventListener('NetInfo', event, callback)
}
module.exports = NetInfo
