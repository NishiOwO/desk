#!/bin/sh
exec clang-format --verbose -i `find src include -name "*.c" -or -name "*.h"`
