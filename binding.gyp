{
    "variables": {
        "arch": ["<!(node -e \"console.log(process.arch);\")"]
    },
    "targets": [
        {
            "target_name": "camera4nodejs",
            "sources": ["src/barcode4nodejs.cc"],
            "defines": [
                "NAPI_DISABLE_CPP_EXCEPTIONS"
            ],
            "include_dirs": [
                "./",
                "<!(node -e \"try { require.resolve('node-addon-api'); console.log(require('node-addon-api').include); } catch (e) { console.log(require('child_process').execSync('npm root -g').toString().trim() + '/node-addon-api'); }\")",
                "<!(node -e \"require('child_process').execSync('node -p process.execPath').toString().trim() + '/../../include/node'\")"
            ],
            "conditions": [
                ["OS=='linux'", {
                    "cflags": ["-std=c++11", "-DNAPI_CPP_EXCEPTIONS", "-fexceptions"],
                    "cflags_cc": ["-std=c++11", "-DNAPI_CPP_EXCEPTIONS", "-fexceptions"],
                    "ldflags": ["-Wl,-rpath,'$$ORIGIN'"],
                    "libraries": [
                        "-llitecam", "-L../platforms/linux"
                    ],
                    "copies": [
                        {
                            "destination": "build/Release/",
                            "files": [
                                "./platforms/linux/liblitecam.so",
                            ]
                        }
                    ]
                }],
                ["OS=='win'", {
                    "defines": ["NAPI_CPP_EXCEPTIONS"],
                    "libraries": [
                        "-l../platforms/windows/litecam.lib"
                    ],
                    "copies": [
                        {
                            "destination": "build/Release/",
                            "files": [
                                "./platforms/windows/litecam.dll",
                            ]
                        }
                    ]

                }],
                ["OS=='mac'", {
                    "cflags": ["-std=c++11", "-DNAPI_CPP_EXCEPTIONS"],
                    "cflags_cc": ["-std=c++11", "-DNAPI_CPP_EXCEPTIONS"],
                    "ldflags": ["-Wl,-rpath,@loader_path"],
                    "libraries": [
                        "-llitecam", "-L../platforms/macos"
                    ],
                    "copies": [
                        {
                            "destination": "build/Release/",
                            "files": [
                                "./platforms/macos/liblitecam.dylib",
                            ]
                        }
                    ]
                }]
            ]
        }
    ]
}
