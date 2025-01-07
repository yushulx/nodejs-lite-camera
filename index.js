const addon = require('./build/Release/camera4nodejs');

module.exports = {
    NodeCam: addon.NodeCam,
    getDeviceList: addon.getDeviceList,
    saveJpeg: addon.saveJpeg
};
