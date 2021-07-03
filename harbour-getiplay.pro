# Application name
TARGET = harbour-getiplay

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 1

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

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/component/*.qml \
    qml/harbour-getiplay.qml \
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
    translations/*.ts \
    README.md \
    generate.sh \
    harbour-getiplay.desktop \
    atomicparsley/*

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

PKGCONFIG += mlite5

# Translations
CONFIG += sailfishapp_i18n
CONFIG += sailfishapp_i18n_idbased
CONFIG += sailfishapp_i18n_unfinished

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

DISTFILES += \
    icons/108x108/harbour-getiplay.png \
    icons/128x128/harbour-getiplay.png \
    icons/172x172/harbour-getiplay.png \
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
    qml/pages/VideoView.qml \
    qml/component/InfoRow.qml \
    qml/pages/DirectDownload.qml
    qml/component/IconButtonDual.qml

# Perl library building and installation
perl_lib.commands = mkdir -p lib/perl5
perl_lib.commands += && touch lib/perl5/file02.txt
perl_lib.commands += && touch lib/perl5/file03.txt
perl_lib.commands += && curl https://cpanmin.us | perl - -l lib/perl5 App::cpanminus local::lib
perl_lib.commands += && eval `perl -I lib/perl5/lib/perl5 -Mlocal::lib=lib/perl5`
perl_lib.commands += && cpanm -n LWP LWP::Protocol::https XML::LibXML Mojolicious CGI Time::Piece
perl_lib.commands += && chmod -R +w lib/perl5

perl_lib.CONFIG += no_check_exist no_link
perl_lib.output = lib
perl_lib.input = .

lib_install.commands = $$QMAKE_INSTALL_DIR $$OUT_PWD/lib/ $(INSTALL_ROOT)/usr/share/$${TARGET}/lib
lib_install.CONFIG += no_check_exist dummy_install

# AtomicParsley and get_iplayer building and installation
ATOMICPARSLEY_FILE = $$OUT_PWD/bin/AtomicParsley
GETIPLAYER_FILE = $$OUT_PWD/bin/get_iplayer
GETIPLAYER_URL = https://raw.githubusercontent.com/get-iplayer/get_iplayer/master/get_iplayer

getiplayer_bin.commands = mkdir -p bin
getiplayer_bin.commands += && curl $$GETIPLAYER_URL -o bin/get_iplayer
getiplayer_bin.CONFIG += no_check_exist no_link
getiplayer_bin.output = $$GETIPLAYER_FILE
getiplayer_bin.input = .

atomicparsley_bin.commands = mkdir -p bin
atomicparsley_bin.commands = mkdir -p bin
atomicparsley_bin.commands += && mkdir -p atomicparsley-build
atomicparsley_bin.commands += && cd atomicparsley-build
atomicparsley_bin.commands += && cmake -S $$PWD/atomicparsley -B .
atomicparsley_bin.commands += && make
atomicparsley_bin.commands += && mv AtomicParsley ../bin/
atomicparsley_bin.commands += && cd ..
atomicparsley_bin.depends = $$PWD/atomicparsley
atomicparsley_bin.CONFIG += no_check_exist no_link
atomicparsley_bin.output = $$ATOMICPARSLEY_FILE
atomicparsley_bin.input = $$PWD/atomicparsley

bin_install.files = $$ATOMICPARSLEY_FILE $$GETIPLAYER_FILE
bin_install.path = /usr/share/$${TARGET}/bin
bin_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += atomicparsley_bin getiplayer_bin perl_lib

PRE_TARGETDEPS += atomicparsley_bin getiplayer_bin perl_lib

INSTALLS += bin_install lib_install
