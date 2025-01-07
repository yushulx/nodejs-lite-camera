#include "nodecam.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
Napi::FunctionReference NodeCam::constructor;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera class implementation
Napi::Value NodeCam::open(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	int index = info[0].As<Napi::Number>().Int32Value();
	bool ret = false;

	if (pCamera)
	{
		ret = pCamera->Open(index);
	}

	return Napi::Boolean::New(env, ret);
}

Napi::Value NodeCam::listMediaTypes(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	Napi::Array list = Napi::Array::New(env);

	std::vector<MediaTypeInfo> mediaTypes = pCamera->ListSupportedMediaTypes();

	for (size_t i = 0; i < mediaTypes.size(); i++)
	{
		MediaTypeInfo &mediaType = mediaTypes[i];

		int width = mediaType.width;
		int height = mediaType.height;

#ifdef _WIN32
		Napi::String jsMediaType = ConvertWCharToJSString(env, mediaType.subtypeName);
#else
		Napi::String jsMediaType = Napi::String::New(env, mediaType.subtypeName);

#endif

		Napi::Object obj = Napi::Object::New(env);
		obj.Set("width", Napi::Number::New(env, width));
		obj.Set("height", Napi::Number::New(env, height));
		obj.Set("mediaType", jsMediaType);

		list.Set(i, obj);
	}

	return list;
}

Napi::Value NodeCam::release(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	if (pCamera)
	{
		pCamera->Release();
	}

	return env.Undefined();
}

Napi::Value NodeCam::setResolution(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	int width = info[0].As<Napi::Number>().Int32Value();
	int height = info[1].As<Napi::Number>().Int32Value();

	bool ret = false;
	if (pCamera)
	{
		ret = pCamera->SetResolution(width, height);
	}

	return Napi::Boolean::New(env, ret);
}

Napi::Value NodeCam::captureFrame(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();

	if (pCamera)
	{
		FrameData frame = pCamera->CaptureFrame();
		if (frame.rgbData)
		{
			Napi::Object res = Napi::Object::New(env);
			int width = frame.width;
			int height = frame.height;
			int size = frame.size;
			unsigned char *rgbData = frame.rgbData;

			// Napi::Buffer<unsigned char> buffer = Napi::Buffer<unsigned char>::New(env, rgbData, size);

			Napi::Buffer<unsigned char> buffer = Napi::Buffer<unsigned char>::New(env, size);
			memcpy(buffer.Data(), rgbData, size);

			res.Set("width", Napi::Number::New(env, width));
			res.Set("height", Napi::Number::New(env, height));
			res.Set("data", buffer);

			ReleaseFrame(frame);

			return res;
		}
	}

	return env.Undefined();
}

Napi::Value NodeCam::getWidth(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();

	if (pCamera)
	{
		int width = pCamera->frameWidth;
		return Napi::Number::New(env, width);
	}

	return env.Undefined();
}

Napi::Value NodeCam::getHeight(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	if (pCamera)
	{
		int height = pCamera->frameHeight;
		return Napi::Number::New(env, height);
	}

	return env.Undefined();
}

NodeCam::NodeCam(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NodeCam>(info)
{
	Napi::Env env = info.Env();
	pCamera = new Camera();
}

NodeCam::~NodeCam()
{
	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pCameraWindow)
	{
		delete pCameraWindow;
		pCameraWindow = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Window class implementation
Napi::Value NodeCam::waitKey(const Napi::CallbackInfo &info)
{
	if (!pCameraWindow)
		return info.Env().Undefined();

	Napi::Env env = info.Env();

	std::string key = info[0].As<Napi::String>();

	bool ret = pCameraWindow->WaitKey(key[0]);
	return Napi::Boolean::New(env, ret);
}

Napi::Value NodeCam::showFrame(const Napi::CallbackInfo &info)
{
	if (!pCameraWindow)
		return info.Env().Undefined();

	Napi::Env env = info.Env();
	int width = info[0].As<Napi::Number>().Int32Value();
	int height = info[1].As<Napi::Number>().Int32Value();
	Napi::Buffer<unsigned char> buffer = info[2].As<Napi::Buffer<unsigned char>>();

	unsigned char *data = buffer.Data();

	pCameraWindow->ShowFrame(data, width, height);

	return env.Undefined();
}

Napi::Value NodeCam::drawContour(const Napi::CallbackInfo &info)
{
	if (!pCameraWindow)
		return info.Env().Undefined();

	Napi::Env env = info.Env();

	std::vector<std::pair<int, int>> points = {};

	Napi::Array arr = info[0].As<Napi::Array>();

	for (size_t i = 0; i < arr.Length(); i++)
	{
		Napi::Value tupleValue = arr.Get(i);
		Napi::Array tupleArray = tupleValue.As<Napi::Array>();

		int x = tupleArray.Get(uint32_t(0)).As<Napi::Number>().Int32Value();
		int y = tupleArray.Get(uint32_t(1)).As<Napi::Number>().Int32Value();

		points.push_back(std::make_pair(x, y));
	}

	pCameraWindow->DrawContour(points);

	return env.Undefined();
}

Napi::Value NodeCam::drawText(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();

	string text = info[0].As<Napi::String>().Utf8Value();
	int x = info[1].As<Napi::Number>().Int32Value();
	int y = info[2].As<Napi::Number>().Int32Value();
	int fontSize = info[3].As<Napi::Number>().Int32Value();
	Napi::Array colorArr = info[4].As<Napi::Array>();

	CameraWindow::Color color;

	color.r = colorArr.Get(uint32_t(0)).As<Napi::Number>().Int32Value();
	color.g = colorArr.Get(uint32_t(1)).As<Napi::Number>().Int32Value();
	color.b = colorArr.Get(uint32_t(2)).As<Napi::Number>().Int32Value();

	pCameraWindow->DrawText(text, x, y, fontSize, color);

	return env.Undefined();
}

Napi::Value NodeCam::createWindow(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	int width = info[0].As<Napi::Number>().Int32Value();
	int height = info[1].As<Napi::Number>().Int32Value();
	string title = info[2].As<Napi::String>().Utf8Value();
	pCameraWindow = new CameraWindow(width, height, title.c_str());
	if (pCameraWindow->Create())
	{
		pCameraWindow->Show();
	}

	return env.Undefined();
}

Napi::Value NodeCam::showPreview(const Napi::CallbackInfo &info)
{
	if (!pCameraWindow || !pCamera)
		return info.Env().Undefined();

	Napi::Env env = info.Env();

	FrameData frame = pCamera->CaptureFrame();
	if (frame.rgbData)
	{
		pCameraWindow->ShowFrame(frame.rgbData, frame.width, frame.height);
	}

	return env.Undefined();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Global functions
Napi::Value getDeviceList(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();

	Napi::Array deviceList = Napi::Array::New(env);

	std::vector<CaptureDeviceInfo> devices = ListCaptureDevices();

	for (size_t i = 0; i < devices.size(); i++)
	{
		CaptureDeviceInfo &device = devices[i];

#ifdef _WIN32
		Napi::String jsFriendlyName = ConvertWCharToJSString(env, device.friendlyName);
#else
		Napi::String jsFriendlyName = Napi::String::New(env, device.friendlyName);
#endif

		deviceList.Set(i, jsFriendlyName);
	}

	return deviceList;
}

Napi::Value saveJpeg(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();

	std::string filename = info[0].As<Napi::String>();
	int width = info[1].As<Napi::Number>().Int32Value();
	int height = info[2].As<Napi::Number>().Int32Value();
	Napi::Buffer<unsigned char> buffer = info[3].As<Napi::Buffer<unsigned char>>();

	unsigned char *data = buffer.Data();
	saveFrameAsJPEG(data, width, height, filename.c_str());

	return env.Undefined();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Node.js module initialization
Napi::Object NodeCam::Init(Napi::Env env, Napi::Object exports)
{

	// Camera
	Napi::Function camerafunc = DefineClass(env, "NodeCam", {InstanceMethod("open", &NodeCam::open), InstanceMethod("listMediaTypes", &NodeCam::listMediaTypes), InstanceMethod("release", &NodeCam::release), InstanceMethod("setResolution", &NodeCam::setResolution), InstanceMethod("captureFrame", &NodeCam::captureFrame), InstanceMethod("getWidth", &NodeCam::getWidth), InstanceMethod("getHeight", &NodeCam::getHeight), InstanceMethod("createWindow", &NodeCam::createWindow), InstanceMethod("waitKey", &NodeCam::waitKey), InstanceMethod("showFrame", &NodeCam::showFrame), InstanceMethod("drawContour", &NodeCam::drawContour), InstanceMethod("drawText", &NodeCam::drawText), InstanceMethod("showPreview", &NodeCam::showPreview)});

	constructor = Napi::Persistent(camerafunc);
	constructor.SuppressDestruct();

	exports.Set("NodeCam", camerafunc);

	return exports;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set("getDeviceList", Napi::Function::New(env, getDeviceList));
	exports.Set("saveJpeg", Napi::Function::New(env, saveJpeg));
	NodeCam::Init(env, exports);
	return exports;
}

NODE_API_MODULE(camera4nodejs, Init)