var WebSocketServer = require('ws').Server,
wss = new WebSocketServer({ port: 9526 });
wss.on('connection', function (ws) {
    ws.on('message', function (message) {
        var msg = JSON.parse(message);
        if(msg["commandType"] === "register") {
            ws.send("{\"fromType\":\"server\",\"commandType\":\"dom\",\"content\":{\"id\":26,\"method\":\"DOM.getDocument\"}}");         
        }
        console.log("收到消息", msg);
    });
    ws.on('close', function () {
    });
});
