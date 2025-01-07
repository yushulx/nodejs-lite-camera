var camera4nodejs = require('./index.js');
const nodecamera = new camera4nodejs.NodeCam();
console.log(camera4nodejs.getDeviceList());

if (nodecamera.open(0)) {
    let mediaTypes = nodecamera.listMediaTypes();
    console.log(mediaTypes);

    for (let i = 0; i < 3; i++) {
        let frame = nodecamera.captureFrame();
        if (frame) {
            camera4nodejs.saveJpeg(i + '.jpg', frame['width'], frame['height'], frame['data']);
        }
    }

    nodecamera.createWindow(nodecamera.getWidth(), nodecamera.getHeight(), "Camera Stream");

    while (nodecamera.waitKey('q')) {
        // nodecamera.showPreview();
        let frame = nodecamera.captureFrame();
        if (frame) {
            nodecamera.showFrame(frame['width'], frame['height'], frame['data']);
        }

        let contour_points = [[100, 100], [200, 100], [200, 200], [100, 200]];
        nodecamera.drawContour(contour_points)

        let text = "Hello, World!"
        nodecamera.drawText(text, 50, 50, 24, [255, 0, 0])
    }

    nodecamera.release();
}