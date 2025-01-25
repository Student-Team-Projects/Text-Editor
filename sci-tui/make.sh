
BASE_DIR=$(../)

LIB=""
for i in $(ls scintilla/src/*.cxx); do
    LIB="${LIB} $i"
done
LIB="${LIB} scintilla/call/ScintillaCall.cxx"

PLAT="platform.cc"

INC_FILES=""
for i in $(ls scintilla/src/*.h); do
    INC_FILES="${INC_FILES} $i"
done
for i in $(ls scintilla/include/*.h); do
    INC_FILES="${INC_FILES} $i"
done 

INC_DIR="-I scintilla/include -I scintilla/src"

COMP="g++ main.cc -o main -fmax-errors=5 std.h $INC_DIR $LIB $PLAT"

$COMP
