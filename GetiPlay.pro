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

VERSION_MAJOR = 0
VERSION_MINOR = 4
VERSION_BUILD = 2

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}-$${VERSION_BUILD}

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR" \
    "VERSION_MINOR=$$VERSION_MINOR" \
    "VERSION_BUILD=$$VERSION_BUILD" \
    "VERSION=\\\"$$VERSION\\\""

CONFIG += sailfishapp

SOURCES += \
    src/download.cpp \
    src/logfile.cpp \
    src/progmodel.cpp \
    src/programme.cpp \
    src/refresh.cpp \
    src/harbour-getiplay.cpp \
    src/settings.cpp \
    src/queue.cpp \
    src/queueitem.cpp \
    src/queuemodel.cpp \
    src/log.cpp \
    src/metaget.cpp \
    src/imageprovider.cpp

OTHER_FILES += qml/GetiPlay.qml \
    qml/cover/CoverPage.qml \
    qml/component/*.qml \
    qml/pages/AboutPage.qml \
    qml/pages/Log.qml \
    qml/pages/MainPage.qml \
    qml/pages/ProgInfo.qml \
    qml/pages/ProgList.qml \
    qml/pages/Queue.qml \
    qml/pages/QueueItemInfo.qml \
    qml/images/* \
    rpm/harbour-getiplay.changes \
    rpm/harbour-getiplay.spec \
    rpm/harbour-getiplay.yaml \
    translations/*.ts \
    harbour-getiplay.desktop \
    bin/ffmpeg \
    bin/AtomicParsley \
    bin/get_iplayer \
    lib/perl5/*

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
CONFIG += sailfishapp_i18n_idbased
CONFIG += sailfishapp_i18n_unfinished

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-getiplay.ts
TRANSLATIONS += translations/harbour-getiplay-de.ts
TRANSLATIONS += translations/harbour-getiplay-en.ts

HEADERS += \
    src/download.h \
    src/logfile.h \
    src/progmodel.h \
    src/programme.h \
    src/refresh.h \
    src/harbour-getiplay.h \
    src/settings.h \
    src/queue.h \
    src/queueitem.h \
    src/queuemodel.h \
    src/log.h \
    src/metaget.h \
    src/imageprovider.h

bin.files = bin
bin.path = /usr/share/$${TARGET}

INSTALLS += bin

lib.files = lib
lib.path = /usr/share/$${TARGET}

INSTALLS += lib

DISTFILES += \
    icons/108x108/harbour-getiplay.png \
    icons/128x128/harbour-getiplay.png \
    icons/256x256/harbour-getiplay.png \
    icons/86x86/harbour-getiplay.png \
    rpm/GetiPlay.changes \
    qml/pages/MainPage.qml \
    qml/pages/Queue.qml \
    qml/pages/Log.qml \
    qml/pages/ProgInfo.qml \
    qml/pages/QueueItemInfo.qml \
    qml/component/SplitViewItem.qml \
    qml/pages/Settings.qml \
    qml/component/FolderPickerDialog.qml \
    qml/component/PickerDialog.qml \
    qml/pages/VideoView.qml
