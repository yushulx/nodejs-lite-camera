#!/bin/bash
node-gyp configure
node-gyp build
install_name_tool -change @rpath/liblitecam.dylib @loader_path/liblitecam.dylib build/Release/litecam.node
