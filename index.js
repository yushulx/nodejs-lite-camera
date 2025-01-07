const addon = require('./build/Release/litecam');

module.exports = {
    NodeCam: addon.NodeCam,
    getDeviceList: addon.getDeviceList,
    saveJpeg: addon.saveJpeg
};
