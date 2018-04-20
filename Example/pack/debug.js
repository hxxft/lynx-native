const fs = require('fs');
const path = require('path');
const express = require('express');
const net = require('net');
const vueServerRenderer = require('../libs/vue-server-renderer');

const app = express();

// Server-Side Bundle File
const serverBundleFilePath = path.join(__dirname, './out/bundle.server.js')
const serverBundleFileCode = fs.readFileSync(serverBundleFilePath, 'utf8');
const bundleRenderer = vueServerRenderer.createBundleRenderer(serverBundleFileCode);

// Client-Side Bundle File
const clientBundleFilePath = path.join(__dirname, '../out/bundle.client.js');
const clientBundleFileUrl = '/bundle.client.js';

// Server-Side Rendering
app.get('/', function (req, res) {
  bundleRenderer.renderToString((err, html) => {
    if (err){
      res.status(500).send(`
        <h1>Error: ${err.message}</h1>
        <pre>${err.stack}</pre>
      `);
    } else {
      res.send(`
        <!DOCTYPE html>
        <html>
          <head>
            <meta charset="utf-8">
            <title>Vue 2.0 SSR</title>
          </head>
          <body>
            ${html}
            <script src="${clientBundleFileUrl}"></script>
          </body>
        </html>`);
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

function pushCommand(command) {
  var HOST = '127.0.0.1'
  var POST = 8000
  var socket = new net.Socket()
  socket.connect(POST, HOST, function() {
    socket.write(command)
    socket.end()
  })
}

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, function () {
  pushCommand('reload')
});
