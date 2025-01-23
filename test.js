const litecam = require('./index.js');
const nodecamera = new litecam.NodeCam();
console.log(litecam.getDeviceList());

if (nodecamera.open(0)) {
    let mediaTypes = nodecamera.listMediaTypes();
    console.log(mediaTypes);

    for (let i = 0; i < 3; i++) {
        let frame = nodecamera.captureFrame();
        if (frame) {
            litecam.saveJpeg(i + '.jpg', frame['width'], frame['height'], frame['data']);
        }
    }

    nodecamera.createWindow(nodecamera.getWidth(), nodecamera.getHeight(), "Camera Stream");

    while (nodecamera.waitKey('q')) {
        // nodecamera.showPreview();
        // let starttime = Date.now();
        let frame = nodecamera.captureFrame();
        if (frame) {
            // console.log('Capture Time: ' + (Date.now() - starttime) + 'ms');
            nodecamera.showFrame(frame['width'], frame['height'], frame['data']);
        }

        let contour_points = [[100, 100], [200, 100], [200, 200], [100, 200]];
        nodecamera.drawContour(contour_points)

        let text = "Hello, World!"
        nodecamera.drawText(text, 50, 50, 24, [255, 0, 0])
    }

    nodecamera.release();
}