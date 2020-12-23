#ifndef JSCLIPBOARD_H
#define JSCLIPBOARD_H

#include <napi.h>

class JSClipboard : public Napi::ObjectWrap<JSClipboard>
{
private:
    Napi::Value add(const Napi::CallbackInfo &info);
    Napi::Value CopyText(const Napi::CallbackInfo &info);
    Napi::Value PasteText(const Napi::CallbackInfo &info);
    Napi::Value MyCopyFile(const Napi::CallbackInfo &info);
    Napi::Value PasteFile(const Napi::CallbackInfo &info);
    Napi::Value IsClipboardFormat(const Napi::CallbackInfo &info);
    Napi::Value lastErrorInfo(const Napi::CallbackInfo &info);
    void *module_;

public:
    JSClipboard(const Napi::CallbackInfo &info);
    ~JSClipboard();
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
};

#endif