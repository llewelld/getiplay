import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0
import harbour.getiplay.progqueue 1.0

Page {
    id: queueInfoPage
    property string name: ""
    property string progId: ""
    property int type: 0
    property int qstatus: 0
    property string statustext: ""
    property int duration: 0

    property string channel: ""
    property string description: ""
    property string episode: ""
    property string timeadded: ""
    property string web: ""
    property string filename: ""

    Connections {
        target:Queue
        onStatusChanged: {
            if (progidchanged == progId) {
                updateStatus(status)
            }
        }
    }

    on_ExposedChanged: {
        if (_exposed) {
            updateStatus(qstatus)
        }
    }

    function updateStatus(status) {
        switch (status) {
        case ProgQueue.STATUS_UNQUEUED:
            // Unqueued
            statustext = "Not queued for download"
            addToQueue.enabled = true
            break;
        case ProgQueue.STATUS_ERROR:
            // Error
            statustext = "Error"
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_REMOTE:
            // Remote
            statustext = "Queued for download"
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_DOWNLOADING:
            // Downloading
            statustext = "Downloading"
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_LOCAL:
            // Local
            statustext = "Downloaded"
            addToQueue.enabled = false
            break;
        default:
            console.log("Status Error: " + status)
            statustext = "Error"
            addToQueue.enabled = true
            break;
        }
        console.log("Status: " + status)
    }

    // Place our content in a Column.  The PageHeader is always placed at the top
    // of the page, followed by our content.
    Column {
        id: column
        width: queueInfoPage.width
        spacing: Theme.paddingLarge

        PageHeader {
            id: header
            title: qsTr("Queue Item")
        }

        Column {
            id: infopane
            width: parent.width
            spacing: Theme.paddingLarge

            Label {
                x: Theme.paddingLarge
                text: name
                wrapMode: Text.Wrap
                width: parent.width - (2 * Theme.paddingLarge)
                height: Theme.fontSizeLarge* 2 + Theme.paddingSmall
                clip: true
            }
            Label {
                x: Theme.paddingLarge
                text: "<b>Episode:</b> \t" + episode
            }
            Label {
                x: Theme.paddingLarge
                text: "<b>Channel:</b> \t" + channel
            }
            Label {
                x: Theme.paddingLarge
                text: "<b>Date:</b> \t" + timeadded
            }

            Rectangle {
                width: 840
                height: 476
                border.width: 4
                border.color: "transparent" // Theme.highlightColor
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"

                Image {
                    id: thumbnail
                    source: ((type == 0) ? Qt.resolvedUrl("image://theme/icon-m-music") : Qt.resolvedUrl("image://theme/icon-m-video"))
                    width: 832
                    height: 468
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: ((type == 0) ? Image.Pad : Image.Pad)
                    NumberAnimation on opacity { id: fadein; from: 0; to: 1; duration: 1000 }
                    onStatusChanged: {
                        if (status == Image.Ready) {
                            fadein.start()
                        }
                    }
                }

                BusyIndicator {
                    id: downloadbusy
                    running: true
                    visible: (thumbnail.status != Image.Ready)
                    size: BusyIndicatorSize.Large
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                x: Theme.paddingLarge
                text: description
                font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width - 2 * Theme.paddingLarge
                height: Theme.fontSizeExtraSmall * 8
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignTop
                clip: true
                elide: Text.ElideRight
            }

            Label {
                id: statusindicator;
                x: Theme.paddingLarge
                text: "<b>Status:</b> \t" + statustext
            }

            Row {
                width: parent.width - 2 * Theme.paddingLarge
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                Button {
                    id: addToQueue
                    text: "Download"
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    onClicked: {
                        if (Queue.addToQueue(progId, name, 100.0, type)) {
                            enabled = false
                            pageStack.pop()
                        }
                    }
                }

                Button {
                    id: visitWebsite
                    text: "Visit website"
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    enabled: (web != "")
                    onClicked: Qt.openUrlExternally(web)
                }
            }
        }
    }
}





