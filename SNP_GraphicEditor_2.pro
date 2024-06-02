QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS   += -lUser32
LIBS   += -lopengl32
# LIBS   += -lglut32
LIBS   += -lGLU32

SOURCES += \
    group3d.cpp \
    main.cpp \
    scene.cpp \
    simpleobject.cpp \
    window.cpp

HEADERS += \
    group3d.hpp \
    scene.hpp \
    simpleobject.hpp \
    transformational.hpp \
    window.hpp

FORMS += \
    window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
  shaders.qrc \
  textures.qrc
