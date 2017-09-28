const fs = require('fs');
const path = require('path');
const express = require('express');
const vueServerRenderer = require('../libs/vue-server-renderer');
//const pushCommand = require('../debug/debug-client')
const debugServerStart = require('../debug/debug-server')
const bodyParser = require('body-parser');
const app = express();
const server = require('http').createServer(app);
app.use(bodyParser.json({limit: '1mb'}));  //body-parser 解析json格式数据
app.use(bodyParser.urlencoded({            //此项必须在 bodyParser.json 下面,为参数编码
  extended: true
}));

// Server-Side Bundle File
const serverBundleFilePath = path.join(__dirname, './out/bundle.server.js')
const serverBundleFileCode = fs.readFileSync(serverBundleFilePath, 'utf8');
const bundleRenderer = vueServerRenderer.createBundleRenderer(serverBundleFileCode);

// Client-Side Bundle File
const clientBundleFilePath = path.join(__dirname, '../out/bundle.client.js');
const clientBundleFileUrl = '/bundle.client.js';

// Server-Side Rendering
app.get('/', function (req, res) {
  console.log('get /')
  bundleRenderer.renderToString((err, html) => {
    if (err){
      res.status(500).send(`
        <h1>Error: ${err.message}</h1>
        <pre>${err.stack}</pre>
      `);
    } else {
      res.send(`
          <body>
            ${html}
            <script src="${clientBundleFileUrl}"></script>
          </body>`);
    }
  });
});

app.get('/data', function (req, res) {
  function randNum() {
    return Math.round(Math.random() * 100);
  }
  res.send({
    name: `Hans-${randNum()}`,
    age: randNum(),
  });
});

// Client-Side Bundle File
app.get(clientBundleFileUrl, function (req, res) {
  const clientBundleFileCode = fs.readFileSync(clientBundleFilePath, 'utf8');
  res.send(clientBundleFileCode);
});

app.get('*.png', function(req, res) {
  var imgPath = path.join(__dirname, '../out');
  const img = fs.readFileSync(imgPath + req.url);
  res.send(img);
})

app.post('/log', function(req,res) {
    console.log(req.body.log)
    res.send('')
})

app.post('/', function(req,res) {
  console.log(req.body.log)
  res.send('')
})

const net = require('net')

function push(socket, command) {
  socket.write(command)
  socket.end()
}

function sendCommand(command) {
  var HOST = '127.0.0.1'
  var POST = 8000
  var socket = new net.Socket()
  socket.connect(POST, HOST, function() {
    socket.write('reload')
    socket.end()
  })
}

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, function () {
  sendCommand('reload')
});

