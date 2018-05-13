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
TARGET = GetiPlay

CONFIG += sailfishapp

SOURCES += src/control.cpp \
    src/download.cpp \
    src/GetiPlay.cpp \
    src/logfile.cpp \
    src/progmodel.cpp \
    src/programme.cpp \
    src/refresh.cpp

OTHER_FILES += qml/GetiPlay.qml \
    qml/cover/CoverPage.qml \
    rpm/GetiPlay.changes.in \
    rpm/GetiPlay.spec \
    rpm/GetiPlay.yaml \
    translations/*.ts \
    GetiPlay.desktop \
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
    perl5/*

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
    src/GetiPlay.h \
    src/logfile.h \
    src/progmodel.h \
    src/programme.h \
    src/refresh.h

bin.files = bin
bin.path = /usr/share/$${TARGET}

INSTALLS += bin

lib.files = lib
lib.path = /usr/share/$${TARGET}

perl.files = perl5
perl.path = /usr/share/$${TARGET}

INSTALLS += lib \
    perl

DISTFILES += \
    lib/librtmp.a \
    lib/librtmp.so.1 \
    rpm/GetiPlay.changes
