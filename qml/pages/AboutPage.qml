import QtQuick 2.0
import Sailfish.Silica 1.0
import "../component"

Page {
    id: aboutPage
    // The version property is now set in the application
    // initialisation code in harbour-getiplay.cpp
    //property string version: "?.?-?"

    SilicaFlickable {
        width: parent.width
        height: parent.height
        interactive: true

        anchors.fill: parent
        contentHeight: aboutColumn.height + Theme.paddingLarge

        VerticalScrollDecorator {}

        Column {
            id: aboutColumn
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "About GetiPlay"
                title: qsTrId("getiplay-about_title")
            }

            Image {
                anchors.topMargin: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                source  : Qt.resolvedUrl("image://getiplay/getiplay-title")
            }

            Label {
                //% "User interface for controlling get_iplayer"
                text: qsTrId("getiplay-about-description")
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
            }

            InfoRow {
                //% "Version:"
                label: qsTrId("getiplay-about_getiplay_version")
                value: version
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            InfoRow {
                //% "Author:"
                label: qsTrId("getiplay-about_author")
                value: "David Llewellyn-Jones"
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            InfoRow {
                //% "Licence:"
                label: qsTrId("getiplay-about_licence")
                value: "MIT"
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            Label {
                //% "Please respect the copyright of all files downloaded using this software"
                text: qsTrId("getiplay-about_respect_copyright")
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
            }

            SectionHeader {
                text:  "About get_iplayer"
            }

            Label {
                //% "Backend functionality is provided by get_iplayer"
                text: qsTrId("getiplay-about_getiplayer_description")
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
            }

            InfoRow {
                //% "Version:"
                label: qsTrId("getiplay-about_getiplayer_version")
                value: "3.14"
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            InfoRow {
                //% "Author:"
                label: qsTrId("getiplay-about_getiplayer_author")
                value: "Phil Lewis + others"
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            InfoRow {
                //% "Licence"
                label: qsTrId("getiplay-about_getiplayer_licence")
                value: "GPLv3"
                midlineRatio: 0.3
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
            }

            SectionHeader {
                //% "Links"
                text: qsTrId("getiplay-about_subtitle_links")
            }

            Row {
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    id: connect
                    //% "Website"
                    text: qsTrId("getiplay-about_website")
                    enabled: true
                    onClicked: Qt.openUrlExternally("http://www.flypig.co.uk/?to=getiplay")
                }
                Button {
                    id : disconnect
                    //% "Email"
                    text: qsTrId("getiaplay-about_email")
                    enabled: true
                    onClicked: Qt.openUrlExternally("mailto:david@flypig.co.uk")
                }
            }
        }
    }
}
