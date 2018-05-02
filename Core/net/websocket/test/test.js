var child_process = require('child_process');
var spawnObj = child_process.spawn('node', ['./server.js'], {encoding: 'utf-8'});
spawnObj.stdout.on('data', function(chunk) {
    var msg = JSON.parse(chunk.toString());
    console.log(msg.content)
});
spawnObj.stderr.on('data', (data) => {
  console.log(data);
});
spawnObj.on('close', function(code) {
    console.log('close code : ' + code);
});
spawnObj.on('exit', (code) => {
    console.log('exit code : ' + code);
});

console.log("test")