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

SOURCES += src/GetiPlay.cpp \
    src/refresh.cpp \
    src/programme.cpp \
    src/progmodel.cpp \
    src/download.cpp \
    src/control.cpp \
    src/logfile.cpp

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
    bin/librtmp.so \
    bin/get_iplayer

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/GetiPlay-de.ts

HEADERS += \
    src/refresh.h \
    src/programme.h \
    src/progmodel.h \
    src/download.h \
    src/control.h \
    src/GetiPlay.h \
    src/logfile.h

bin.files = bin\ffmpeg \
    bin\rtmpdump \
    bin\librtmp.so \
    bin\get_iplayer
bin.path = /usr/share/$${TARGET}/bin

INSTALLS += bin
