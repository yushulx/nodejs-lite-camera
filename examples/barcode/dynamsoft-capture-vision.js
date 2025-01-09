const { LicenseManager, CaptureVisionRouter, EnumPresetTemplate, EnumImagePixelFormat } = require('dynamsoft-capture-vision-for-node');
LicenseManager.initLicense("DLS2eyJoYW5kc2hha2VDb2RlIjoiMjAwMDAxLTE2NDk4Mjk3OTI2MzUiLCJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSIsInNlc3Npb25QYXNzd29yZCI6IndTcGR6Vm05WDJrcEQ5YUoifQ==");

const litecam = require('litecam');
const nodecamera = new litecam.NodeCam();
console.log(litecam.getDeviceList());

var isWorking = false;

var results = null;

async function decode(buffer, width, height) {
    if (isWorking) {
        return;
    }
    isWorking = true;
    let imageData = {};
    imageData.bytes = buffer;
    imageData.width = width;
    imageData.height = height;
    imageData.stride = width * 3;
    imageData.format = EnumImagePixelFormat.IPF_RGB_888;
    results = await CaptureVisionRouter.captureAsync(imageData, EnumPresetTemplate.PT_READ_BARCODES);

    isWorking = false;
}

function show() {
    if (nodecamera.waitKey('q')) {

        let frame = nodecamera.captureFrame();
        if (frame) {
            nodecamera.showFrame(frame['width'], frame['height'], frame['data']);
            decode(frame['data'], frame['width'], frame['height']);

            if (results) {
                for (let item of results.barcodeResultItems) {
                    let points = item.location.points;
                    let contour_points = [[points[0].x, points[0].y], [points[1].x, points[1].y], [points[2].x, points[2].y], [points[3].x, points[3].y]];
                    nodecamera.drawContour(contour_points)

                    nodecamera.drawText(item.text, points[0].x, points[0].y, 24, [255, 0, 0])
                }
            }

        }

        setTimeout(show, 30);
    }
    else {
        nodecamera.release();
        (async () => {
            await CaptureVisionRouter.terminateIdleWorkers();
        })();
    }
}

if (nodecamera.open(0)) {
    let mediaTypes = nodecamera.listMediaTypes();
    console.log(mediaTypes);

    nodecamera.createWindow(nodecamera.getWidth(), nodecamera.getHeight(), "Camera Stream");
    show();
}