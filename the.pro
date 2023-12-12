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
        user_login_page.cpp \
        user_signup_page.cpp \
        video_recorder_page.cpp

HEADERS += \
    profile_page.h \
    the_button.h \
    the_player.h \
    user_login_page.h \
    user_signup_page.h \
    video_recorder_page.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    profile_page.ui \
    user_login_page.ui \
    user_signup_page.ui

#RESOURCES += \
#    pic.qrc

RESOURCES += \
    Profile_resource.qrc \
    homePage_Icons.qrc \
    signupPage_bg.qrc




