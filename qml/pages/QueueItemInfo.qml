import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0

Page {
    id: queueInfoPage
    property string name: ""
    property string progId: ""
    property int type: 0
    property int duration: 0
    property int available: 0
    property int qstatus: 0
    property string statustext: ""

    property string channel: ""
    property string description: ""
    property string episode: ""
    property string web: ""
    property string filename: ""
    property string imageid: ""
    property int position: 0

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
            thumbnail.source = (imageid == "" ?
                                    "https://ichef.bbci.co.uk/images/ic/640x360/p01tqv8z.png" :
                                    "https://ichef.bbci.co.uk/images/ic/640x360/" + imageid
                                )
        }
    }

    function updateStatus(status) {
        switch (status) {
        case ProgQueue.STATUS_UNQUEUED:
            // Unqueued
            //% "Not queued for download"
            statustext = qsTrId("getiplay-queueitem_unqueued")
            break;
        case ProgQueue.STATUS_ERROR:
            // Error
            //% "Error"
            statustext = qsTrId("getiplay-queueitem_error")
            break;
        case ProgQueue.STATUS_REMOTE:
            // Remote
            //% "Queued for download"
            statustext = qsTrId("getiplay-queueitem_remote")
            break;
        case ProgQueue.STATUS_DOWNLOADING:
            // Downloading
            //% "Downloading"
            statustext = qsTrId("getiplay-queueitem_downloading")
            break;
        case ProgQueue.STATUS_LOCAL:
            // Local
            //% "Downloaded"
            statustext = qsTrId("getiplay-queueitem_local")
            var item = Queue.getDetails(progId)
            filename = item["filename"]
            break;
        case ProgQueue.STATUS_DELETED:
            // Deleted
            //% "File deleted"
            statustext = qsTrId("getiplay-queueitem_deleted")
            break;
        default:
            console.log("Status Error: " + status)
            //% "Unexpected error"
            statustext = qsTrId("getiplay-queueitem_status_error")
            break;
        }
        qstatus = status;
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
            //% "Queue Item"
            title: qsTrId("getiplay-queueitem-title")
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
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                //% "Episode"
                text: "<b>" + qsTrId("getiplay-queueitem_episode") + ":</b> \t" + episode
            }
            Label {
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                //% "Channel"
                text: "<b>" + qsTrId("getiplay-queueitem_channel") + ":</b> \t" + channel
            }
            Label {
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                //% "Date"
                text: "<b>" + qsTrId("getiplay-queueitem_date_available") + ":</b> \t" + Settings.epochToDate(available);
            }

            Rectangle {
                width: parent.width - 2 * Theme.paddingLarge
                height: width * 0.5625
                border.width: 0
                border.color: "transparent" // Theme.highlightColor
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"

                Image {
                    visible: true
                    id: thumbnail
                    source: ""
                    width: parent.width - 8
                    height: parent.height - 8
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    NumberAnimation on opacity { id: fadein; from: 0; to: 1; duration: 1000 }
                    onStatusChanged: {
                        if (status == Image.Ready) {
                            fadein.start()
                        }
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: Qt.resolvedUrl("image://theme/icon-l-play")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("VideoView.qml"), { progId: progId, imageid: imageid, filename: filename })
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
                //font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width - 2 * Theme.paddingLarge
                height: Theme.fontSizeMedium * 4
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignTop
                clip: true
                elide: Text.ElideRight
            }

            Label {
                id: statusindicator;
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                //% "Status"
                text: "<b>" + qsTrId("getiplay-queueinfo_status") + ":</b> \t" + statustext
            }

            Row {
                width: parent.width - 2 * Theme.paddingLarge
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                Button {
                    id: openFile
                    //% "Play"
                    text: qsTrId("getiplay-queueinfo_play")
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    enabled: ((qstatus == ProgQueue.STATUS_LOCAL) && (filename != ""))
                    onClicked: {
                        Qt.openUrlExternally(filename);
                    }
                }

                Button {
                    id: visitWebsite
                    //% "Visit website"
                    text: qsTrId("getiplay-queueinfo_website")
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    enabled: (web != "")
                    onClicked: Qt.openUrlExternally(web)
                }
            }
        }
    }
}





