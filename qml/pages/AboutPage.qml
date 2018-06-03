import QtQuick 2.0
import Sailfish.Silica 1.0

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
                width: 530
                height: 150
                anchors.topMargin: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                source  : "../images/getiplay-title.svg"
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
            Label {
                //% "Version"
                text: qsTrId("getiplay-about_version") + ":\t" + version
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignLeft
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                //% "Author"
                text: qsTrId("getiplay-about_author") + ":\t\t" + "David Llewellyn-Jones"
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignLeft
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
            }
            Label {
                //% "Licence"
                text: qsTrId("getiplay-about_licence") + ":\t" + "MIT"
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignLeft
                anchors {
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
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

            Label {
                //% "Links"
                text: qsTrId("getiplay-about_subtitle_links")
                color: Theme.highlightColor
                font.family: Theme.fontFamilyHeading
                horizontalAlignment: Text.AlignRight
                anchors {
                    margins: Theme.paddingLarge
                    left: parent.left
                    right: parent.right
                }
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
