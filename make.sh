#!/bin/bash

#Debug, Release, RelWithDebInfo, MinSizeRel
build_type=RelWithDebInfo

while [ -n "$1" ]; do
	if [ "$1" == "--debug" ]; then
		build_type=Debug
	fi
	shift
done


cmake -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build/ .
cmake --build build/ --parallel 2
