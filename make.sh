#!/bin/bash

#Debug, Release, RelWithDebInfo, MinSizeRel
build_type=RelWithDebInfo

while [ -n "$1" ]; do
	if [ "$1" == "--debug" ]; then
		build_type=Debug
	fi
	shift
done


parallel=1
if command -v nproc >/dev/null 2>&1; then
	parallel=$(nproc)
fi

cmake -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build/ .
cmake --build build/ --parallel $parallel

