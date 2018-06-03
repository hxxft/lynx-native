
function exec(name, method, args, onSuccess, onFail) {
    var callbackId = plugin.callbackId++
    if (onSuccess || onFail) {
        plugin.callbacks[callbackId] = {success:onSuccess, fail:onFail};
    }
    plugin.exec(name, method, callbackId, args)
}

function addEventListener(name, method, callback) {
    plugin.addEventListener(name, method, callback)
}

function removeEventListener(name, method, callback) {
    plugin.removeEventListener(name, method, callback)
}

function callbackFromNative(callbackId, successed, args) {
    
    var callback = plugin.callbacks[callbackId];
    if (callback) {
        if (successed) {
            callback.success.apply(null, args);
        } else {
            callback.fail.apply(null, args);
        }
        delete plugin.callbacks[callbackId];
    }
}

plugin.callbackId = 0
plugin.callbacks = {}
plugin.init(callbackFromNative)

var pluginExec = {}

pluginExec.addEventListener = addEventListener
pluginExec.removeEventListener = removeEventListener
pluginExec.exec = exec
module.exports = pluginExec
