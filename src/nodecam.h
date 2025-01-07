#ifndef NodeCamera_H
#define NodeCamera_H

#include <napi.h>
#include <string>
#include <uv.h>
#include <vector>

#include "Camera.h"
#include "CameraPreview.h"

using namespace std;

Napi::String ConvertWCharToJSString(Napi::Env env, const wchar_t *wideStr)
{
    std::wstring wstr(wideStr);

    std::string utf8Str(wstr.begin(), wstr.end());

    return Napi::String::New(env, utf8Str);
}

class NodeCam : public Napi::ObjectWrap<NodeCam>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NodeCam(const Napi::CallbackInfo &info);
    ~NodeCam();

    // Camera
    Napi::Value open(const Napi::CallbackInfo &info);
    Napi::Value listMediaTypes(const Napi::CallbackInfo &info);
    Napi::Value release(const Napi::CallbackInfo &info);
    Napi::Value setResolution(const Napi::CallbackInfo &info);
    Napi::Value captureFrame(const Napi::CallbackInfo &info);
    Napi::Value getWidth(const Napi::CallbackInfo &info);
    Napi::Value getHeight(const Napi::CallbackInfo &info);

    // Window
    Napi::Value createWindow(const Napi::CallbackInfo &info);
    Napi::Value waitKey(const Napi::CallbackInfo &info);
    Napi::Value showPreview(const Napi::CallbackInfo &info);
    Napi::Value showFrame(const Napi::CallbackInfo &info);
    Napi::Value drawContour(const Napi::CallbackInfo &info);
    Napi::Value drawText(const Napi::CallbackInfo &info);

private:
    Camera *pCamera;
    CameraWindow *pCameraWindow;

    static Napi::FunctionReference constructor;
};
#endif