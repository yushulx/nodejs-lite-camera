# LiteCam for Node.js
[LiteCam](https://github.com/yushulx/cmake-cpp-barcode-qrcode-mrz/tree/main/litecam) is a lightweight, cross-platform library for capturing RGB frames from cameras and displaying them. Designed with simplicity and ease of integration in mind, LiteCam supports **Windows**, **Linux** and **macOS** platforms. It integrates easily with image processing frameworks like **OpenCV**, making it a versatile choice for developers.

## Features

- Capture RGB frames from video devices.
- Display live frames in a window.
- Draw contours and text directly on the displayed frames.
- Simple API for quick integration.

## Supported Platforms

- **Windows**
- **Linux**
- **macOS**

## How to Build the Node.js Extension    
```bash
npm i node-gyp -g
npm i node-addon-api -g
node-gyp configure
node-gyp build
```

## Usage

### Basic Example

```js
var litecam = require('litecam');
const nodecamera = new litecam.NodeCam();
console.log(litecam.getDeviceList());

if (nodecamera.open(0)) {
    let mediaTypes = nodecamera.listMediaTypes();
    console.log(mediaTypes);

    nodecamera.createWindow(nodecamera.getWidth(), nodecamera.getHeight(), "Camera Stream");
    while (nodecamera.waitKey('q')) {
        let frame = nodecamera.captureFrame();
        if (frame) {
            nodecamera.showFrame(frame['width'], frame['height'], frame['data']);
        }
    }

    nodecamera.release();
}
```

### API Overview

- **getDeviceList()**: Lists available video capture devices.
- **saveJpeg(filename, width, height, rgbdata)**: Saves the frame as a JPEG image.

#### NodeCam

**Camera**
- **open(index)**: Opens the camera with the specified index.
- **listMediaTypes()**: Lists supported media types.
- **setResolution(int width, int height)**: Sets the resolution for the camera.
- **captureFrame()`**: Captures a single RGB frame.
- **release()**: Closes the camera and releases resources.
- **getWidth()**: Returns the width of the frame.
- **getHeight()**: Returns the height of the frame.

**Window**
- **createWindow(width, height, title)**: Creates a window with the specified dimensions and title.
- **waitKey(key)**: Waits for user input; returns `false` if the specified key is pressed or the window is closed.
- **showFrame(width, height, rgbdata)**: Displays a frame in the window.
- **drawContour(points)**: Draws contours on the preview window.
- **drawText(text, x, y, fontSize, color)**: Draws text on the preview window.