#!/bin/bash

# this is the compile-script for a simple compilation of the game
# the following variables will be used:
#   SYSTEM_DATA_DIR		- the global data dir for the game; default=/usr/share
#	CPPFLAGS			- some other g++ flags
#	DEBUG				- if set to 1, the game will compiled with debug-info

# check variables and set default values if unset
[ "$SYSTEM_DATA_DIR" == "" ] && SYSTEM_DATA_DIR=/usr/share
[ "$DEBUG" == "" ] && DEBUG=0
[ "$COMPILER" == "" ] && COMPILER=g++

function test_include_file() {
	[ -e /usr/include/$1 -o -e /usr/local/include/$1 ]
	return $?
}

# do some simple checks
type $COMPILER 1>/dev/null 2>/dev/null || \
	{ echo "ERROR: g++ not found" >&2; exit -1; }
test_include_file libxml2/libxml/parser.h || \
	{ echo "ERROR: libxml2 headers not found" >&2; exit -1; }
test_include_file SDL/SDL.h || \
	{ echo "ERROR: SDL headers not found" >&2; exit -1; }
test_include_file SDL/SDL_image.h || \
	{ echo "ERROR: SDL_image.h not found" >&2; exit -1; }
test_include_file SDL/SDL_mixer.h || \
	{ echo "ERROR: SDL_image.h not found" >&2; exit -1; }
test_include_file nl.h || \
	{ echo "ERROR: HawkNL header not found" >&2; exit -1; }
test_include_file zlib.h || \
	{ echo "ERROR: zlib header not found" >&2; exit -1; }
test_include_file gd.h || \
	{ echo "ERROR: gd header not found" >&2; exit -1; }

mkdir -p bin

echo ">>> compiling now, this could take some time ..."
if $COMPILER src/*.cpp src/client/*.cpp src/common/*.cpp src/server/*.cpp \
	-I include -I /usr/include/libxml2 \
	-lSDL -lSDL_image -lSDL_mixer -lNL -lz -lgd -lxml2 \
	-DSYSTEM_DATA_DIR="\"$SYSTEM_DATA_DIR\"" \
	-DDEBUG="$DEBUG" \
	$CPPFLAGS \
	-o bin/openlierox
then
	echo ">>> success"
	exit 0
else
	echo ">>> error(s) reported, check the output above"
	exit -1
fi

