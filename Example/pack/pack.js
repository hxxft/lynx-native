const fs = require('fs');
const path = require('path');

const vueServerRenderer = require('../libs/vue-server-renderer');


// Server-Side Bundle File
const serverBundleFilePath = path.join(__dirname, './out/bundle.server.js')
const serverBundleFileCode = fs.readFileSync(serverBundleFilePath, 'utf8');
const bundleRenderer = vueServerRenderer.createBundleRenderer(serverBundleFileCode);

// Client-Side Bundle File
const clientBundleFilePath = path.join(__dirname, '../out/bundle.client.js');
const clientBundleFileUrl = '/bundle.client.js';

var pagePath = path.join(__dirname, '../out/index.html')
// Server-Side Rendering
bundleRenderer.renderToString((err, html) => {
    var msg
    if (err) {
        msg = `
        <h1>Error: ${err.message}</h1>
        <pre>${err.stack}</pre>
      `;
    } else {
        msg = `
        <!DOCTYPE html>
        <html>
          <head>
            <meta charset="utf-8">
          </head>
          <body>
            ${html}
            <script src="${clientBundleFileUrl}"></script>
          </body>
        </html>`;
    }
    fs.writeFile(pagePath, msg,function(err){
        if(err) {
            console.error('Write failed: '+pagePath);
        }
    });
});



