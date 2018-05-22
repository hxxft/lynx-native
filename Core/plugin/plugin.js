
function exec(module, method, args, onSuccess, onFail) {
    var callbackId = plugin.callbackId++
    if (onSuccess || onFail) {
        plugin.callbacks[callbackId] = {success:onSuccess, fail:onFail};
    }
    plugin.exec(module, method, callbackId, args)
}

function callbackFromNative(callbackId, status, args) {
    
    var callback = plugin.callbacks[callbackId];
    if (callback) {
        if (status === 0) {
            callback.fail.apply(null, args);
        } else if (status === 1) {
            callback.success.apply(null, args);
        }
        delete plugin.callbacks[callbackId];
    }
}

plugin.callbackId = 0
plugin.callbacks = {}
plugin.init(callbackFromNative)

