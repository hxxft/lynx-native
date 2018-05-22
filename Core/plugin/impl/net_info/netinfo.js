var NetInfo = {}
NetInfo.getConnectInfo = function(callback) {
    exec('NetInfo', 'getConnectInfo', [], callback, null)
}

NetInfo.isConnected = function(callback) {
    exec('NetInfo', 'isConnected', [], callback, null)
}

NetInfo.addEventListener = function(event, callback) {
    exec('NetInfo', 'addEventListener', [event, callback], null, null)
}

NetInfo.removeEventListener = function(event, callback) {
    exec('NetInfo', 'removeEventListener', [event, callback], null, null)
}
