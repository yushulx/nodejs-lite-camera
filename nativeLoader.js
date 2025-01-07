if (process.platform === 'win32') {
    console.log('Windows');
}
else if (process.platform === 'linux') {
    console.log('Linux');
}
else if (process.platform === 'darwin') {
    console.log('macOS');
}
else {
    console.log('Unknown Operating System');
}

let nodecam;
module.exports = function () {
    if (!nodecam) {
        nodecam = require('./build/Release/nodecam');
    }
    return nodecam;
};