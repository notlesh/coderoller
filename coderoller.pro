CONFIG += debug thread 
QMAKE_CXXFLAGS += -std=c++0x -frtti

# get some useful git defines
GIT_HASH = $$system( git rev-parse HEAD )
GIT_BRANCH = $$system( git rev-parse --abbrev-ref HEAD )
DEFINES += GIT_HASH=\\\"$$GIT_HASH\\\" GIT_BRANCH=\\\"$$GIT_BRANCH\\\"

# this will force the makefile to use colorgcc, a wrapper around gcc that colorizes content.
# install on ubuntu with "sudo apt-get install colorgcc"
# or comment out to use straight gcc.
QMAKE_CXX = colorgcc

INCLUDEPATH += include/ \
		../roller/include \
		./tinyxml2/ \

debug:LIBS += -L../roller/debug/ -lroller \
 
release:LIBS += -L../roller/release/ -lroller \

SOURCES = $$files(src/*.cpp) \
		$$files(src/languages/*.cpp) \
		tinyxml2/tinyxml2.cpp \

HEADERS = $$files(include/*.h) \
		$$files(include/languages/*.h) \
		tinyxml2/tinyxml2.h \
 
release:DESTDIR = release
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.obj
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui
