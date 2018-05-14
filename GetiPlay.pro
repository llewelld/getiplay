# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-getiplay

CONFIG += sailfishapp

SOURCES += src/control.cpp \
    src/download.cpp \
    src/logfile.cpp \
    src/progmodel.cpp \
    src/programme.cpp \
    src/refresh.cpp \
    src/harbour-getiplay.cpp \
    src/settings.cpp

OTHER_FILES += qml/GetiPlay.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-getiplay.changes.in \
    rpm/harbour-getiplay.spec \
    rpm/harbour-getiplay.yaml \
    translations/*.ts \
    harbour-getiplay.desktop \
    qml/pages/ProgList.qml \
    qml/pages/AboutPage.qml \
    qml/images/getiplay-title.png \
    qml/pages/ProgInfoTV.qml \
    qml/pages/ProgInfoRadio.qml \
    qml/pages/RefreshTV.qml \
    qml/pages/RefreshRadio.qml \
    bin/ffmpeg \
    bin/rtmpdump \
    bin/AtomicParsley \
    bin/rtmpdump \
    bin/get_iplayer \
    lib/librtmp.a \
    lib/librtmp.so.1 \
    lib/perl5/*

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/GetiPlay-de.ts

HEADERS += \
    src/control.h \
    src/download.h \
    src/logfile.h \
    src/progmodel.h \
    src/programme.h \
    src/refresh.h \
    src/harbour-getiplay.h \
    src/settings.h

bin.files = bin
bin.path = /usr/share/$${TARGET}

INSTALLS += bin

lib.files = lib
lib.path = /usr/share/$${TARGET}

INSTALLS += lib

DISTFILES += \
    lib/librtmp.a \
    lib/librtmp.so.1 \
    icons/108x108/harbour-getiplay.png \
    icons/128x128/harbour-getiplay.png \
    icons/256x256/harbour-getiplay.png \
    icons/86x86/harbour-getiplay.png \
    rpm/GetiPlay.changes
