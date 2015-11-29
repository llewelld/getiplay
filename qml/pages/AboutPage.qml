import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage
    property string version: "0.1-3"

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
                title: "About GetiPlay"
            }

            Image {
                width: 311
                height: 94
                anchors.topMargin: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                source  : "../images/getiplay-title.png"
            }

            Label {
                text: "User interface for controlling get_iplayer"
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
                text: "Version:\t" + version
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
                text: "Author:\tDavid Llewellyn-Jones"
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
                text: "Licence:\tMIT"
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
                text: "Please respect the copyright of all files downloaded using this software"
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
                text: "Links"
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
                    text: "Website"
                    enabled: true
                    onClicked: Qt.openUrlExternally("http://www.flypig.co.uk/?to=getiplay")
                }
                Button {
                    id : disconnect
                    text: "Email"
                    enabled: true
                    onClicked: Qt.openUrlExternally("mailto:david@flypig.co.uk")
                }
            }
        }
    }
}
