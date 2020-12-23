#include "js-clipboard.h"
#include <codecvt>
#include <iostream>

#ifdef _WIN32
#include "win32-dlfcn.h"
#include <malloc.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/malloc.h>
#endif

char *error;
Napi::Object JSClipboard::Init(Napi::Env env, Napi::Object exports)
{

    Napi::Function func = DefineClass(
        env, "JSClipboard",
        {
            InstanceMethod("lastErrorInfo", &JSClipboard::lastErrorInfo),
            InstanceMethod("add", &JSClipboard::add),
            InstanceMethod("CopyText", &JSClipboard::CopyText),
            InstanceMethod("PasteText", &JSClipboard::PasteText),
            InstanceMethod("CopyFile", &JSClipboard::MyCopyFile),
            InstanceMethod("PasteFile", &JSClipboard::PasteFile),
            InstanceMethod("IsClipboardFormat", &JSClipboard::IsClipboardFormat),
        });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("JSClipboard", func);
    return exports;
}

JSClipboard::JSClipboard(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<JSClipboard>(info)
{
    this->module_ = dlopen(info[0].ToString().Utf8Value().c_str(), RTLD_LAZY);
};

JSClipboard::~JSClipboard()
{
    dlclose(this->module_);
}

Napi::Object JSClipboard::NewInstance(Napi::Env env, Napi::Value arg)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value JSClipboard::add(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int arg0 = info[0].As<Napi::Number>().Int32Value();
    int arg1 = info[1].As<Napi::Number>().Int32Value();

    printf("%d + %d \n", arg0, arg1);

    return Napi::Number::New(env, arg0 + arg1);
}

Napi::Value JSClipboard::lastErrorInfo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if ((error = dlerror()) != NULL)
    {
        return Napi::String::New(env, error);
    }

    return env.Null();
}

Napi::Value JSClipboard::CopyText(const Napi::CallbackInfo &info)
{
    typedef bool (*pFn)(const char *, const int);
    pFn fn = NULL;
    Napi::Env env = info.Env();

    *(void **)(&fn) = dlsym(this->module_, "CopyToClipboard");

    if ((error = dlerror()) != NULL)
    {
        Napi::TypeError::New(env, error)
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string args0 = info[0].As<Napi::String>().Utf8Value();

    bool status = (*fn)(args0.c_str(), sizeof(args0.c_str()));

    return Napi::Boolean::New(env, status);
}

Napi::Value JSClipboard::PasteText(const Napi::CallbackInfo &info)
{
    typedef char *(*pFn)();
    pFn fn = NULL;
    Napi::Env env = info.Env();

    *(void **)(&fn) = dlsym(this->module_, "PasteToClipboard");

    if ((error = dlerror()) != NULL)
    {
        Napi::TypeError::New(env, error)
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    char *reciveBuf = (*fn)();

    return Napi::String::New(env, reciveBuf);
}

Napi::Value JSClipboard::MyCopyFile(const Napi::CallbackInfo &info)
{
    typedef bool (*pFn)(const char *, const int);
    pFn fn = NULL;
    Napi::Env env = info.Env();

    *(void **)(&fn) = dlsym(this->module_, "CopyFileToClipboard");

    if ((error = dlerror()) != NULL)
    {
        Napi::TypeError::New(env, error)
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string args0 = info[0].As<Napi::String>().Utf8Value();

    bool status = (*fn)(args0.c_str(), args0.length());
    return Napi::Boolean::New(env, status);
}

Napi::Value JSClipboard::PasteFile(const Napi::CallbackInfo &info)
{
    typedef char *(*pFn)();
    pFn fn = NULL;
    Napi::Env env = info.Env();

    *(void **)(&fn) = dlsym(this->module_, "PasteFileToClipboard");

    if ((error = dlerror()) != NULL)
    {
        Napi::TypeError::New(env, error)
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    char *reciveBuf = (*fn)();

    return Napi::String::New(env, reciveBuf);
}

Napi::Value JSClipboard::IsClipboardFormat(const Napi::CallbackInfo &info)
{
    typedef int (*pFn)();
    pFn fn = NULL;
    Napi::Env env = info.Env();

    *(void **)(&fn) = dlsym(this->module_, "IsClipboardFormat");

    if ((error = dlerror()) != NULL)
    {
        Napi::TypeError::New(env, error)
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    int status = (*fn)();

    return Napi::Number::New(env, status);
}