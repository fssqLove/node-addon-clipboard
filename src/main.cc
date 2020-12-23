#include <napi.h>
#include "js-clipboard.h"

Napi::Object CreateObject(const Napi::CallbackInfo &info)
{
	return JSClipboard::NewInstance(info.Env(), info[0]);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
	Napi::Object new_exports =
		Napi::Function::New(env, CreateObject, "clipboardObject");
	return JSClipboard::Init(env, new_exports);
}

NODE_API_MODULE(addon, InitAll)