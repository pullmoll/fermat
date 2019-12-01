android|ios|tvos|winrt {
    warning( "This example is not supported for android, ios, tvos, or winrt." )
}

QT += 3dcore 3drender 3dinput 3dextras
QT += widgets

SOURCES += main.cpp \
    scenemodifier.cpp

HEADERS += \
    scenemodifier.h


