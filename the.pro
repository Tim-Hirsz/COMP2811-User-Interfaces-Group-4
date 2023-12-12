QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        profile_page.cpp \
        the_button.cpp \
        the_player.cpp \
        tomeo.cpp \
        video_recorder_page.cpp

HEADERS += \
    profile_page.h \
    the_button.h \
    the_player.h \
    video_recorder_page.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    profile_page.ui

RESOURCES +=

DISTFILES += \
    ../Downloads/black-24dp/2x/baseline_close_black_24dp.png



