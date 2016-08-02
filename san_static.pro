######################################################################
# Automatically generated by qmake (3.0) Fri Sep 18 13:08:22 2015
######################################################################

QT += core widgets network
TEMPLATE = app
TARGET = gfd
INCLUDEPATH += .

# Input
INCLUDEPATH += c:/work/ffmpeg1/include
LIBS += -Lc:\work\ffmpeg1\lib -lavformat -lavcodec -lavutil -lswscale -lswresample -lavfilter -lavdevice
INCLUDEPATH += c:\temp\qtbuild\sdl\SDL2-2.0.4\i686-w64-mingw32\include\SDL2
LIBS += -L c:\temp\qtbuild\sdl\SDL2-2.0.4\i686-w64-mingw32\lib -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
HEADERS += debug.h device.h ytdevice.h fetcher.h front.h fpbttn.h widget.h semapp.h urldevice.h audiocontext.h
SOURCES += front.cpp device.cpp ytdevice.cpp fetcher.cpp fpbttn.cpp widget.cpp labelgb.cpp griddb.cpp semapp.cpp videowd.cpp main.cpp dd.c
DEFINES += GFDNODEBUG
DEFINES += NNODEBUG
RESOURCES+= resource.qrc
RC_FILE=myapp.rc
#CONFIG+= console
