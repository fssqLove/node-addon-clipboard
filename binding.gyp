{
    "targets": [
        {
            "target_name": "addon-clipboard",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": ["src/main.cc", "src/js-clipboard.cc", "src/win32-dlfcn.cc"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
        }
    ]
}
