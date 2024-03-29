import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0
import "../component"

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

    SilicaFlickable {
        width: parent.width
        height: parent.height
        interactive: true

        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        VerticalScrollDecorator {}

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            width: queueInfoPage.width
            spacing: Theme.paddingMedium

            PageHeader {
                id: header
                //% "Queue Item"
                title: qsTrId("getiplay-queueitem-title")
            }

            Label {
                x: Theme.paddingLarge
                text: name
                wrapMode: Text.Wrap
                width: parent.width - (2 * Theme.paddingLarge)
                height: Theme.fontSizeLarge* 2 + Theme.paddingSmall
                clip: true
            }
            InfoRow {
                //% "Episode:"
                label: qsTrId("getiplay-queueitem_episode")
                value: episode
                midlineRatio: 0.25
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
                pixelSize: Theme.fontSizeMedium
                labelTextBold: true
            }
            InfoRow {
                //% "Channel:"
                label: qsTrId("getiplay-queueitem_channel")
                value: channel
                midlineRatio: 0.25
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
                pixelSize: Theme.fontSizeMedium
                labelTextBold: true
            }
            InfoRow {
                //% "Date:"
                label: qsTrId("getiplay-queueitem_date_available")
                //% "Unknown"
                value: (available != 0) ? Settings.epochToDate(available) : qsTrId("getiplay-queueitem_date_unknown")
                midlineRatio: 0.25
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
                pixelSize: Theme.fontSizeMedium
                labelTextBold: true
            }
            InfoRow {
                id: statusindicator;
                //% "Status:"
                label: qsTrId("getiplay-queueinfo_status")
                value: statustext
                midlineRatio: 0.25
                midlineMin: Theme.fontSizeSmall * 5
                midlineMax: Theme.fontSizeSmall * 10
                pixelSize: Theme.fontSizeMedium
                labelTextBold: true
            }

            Rectangle {
                width: parent.width - 2 * Theme.paddingLarge
                height: width * 0.5625
                border.width: 0
                border.color: "transparent" // Theme.highlightColor
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"

                Label {
                    id: thumbnailtext
                    text: ""
                }

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

            Row {
                width: parent.width - 2 * Theme.paddingLarge
                height: implicitHeight
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                Button {
                    id: playFile
                    //% "Play"
                    text: qsTrId("getiplay-queueinfo_play")
                    width: ((parent.width - 2 * Theme.paddingLarge) / 3)
                    enabled: ((qstatus == ProgQueue.STATUS_LOCAL) && (filename != ""))
                    onClicked: {
                        if (type == Settings.REFRESHTYPE_RADIO) {
                            startAudio(progId, filename)
                        }
                        else {
                            stopAudio()
                            pageStack.push(Qt.resolvedUrl("VideoView.qml"), { progId: progId, imageid: imageid, filename: filename })
                        }
                    }
                }

                Button {
                    id: launchFile
                    //% "Launch"
                    text: qsTrId("getiplay-queueinfo_launch")
                    width: ((parent.width - 2 * Theme.paddingLarge) / 3)
                    enabled: ((qstatus == ProgQueue.STATUS_LOCAL) && (filename != ""))
                    onClicked: {
                        Qt.openUrlExternally(filename);
                    }
                }

                Button {
                    id: visitWebsite
                    //% "Website"
                    text: qsTrId("getiplay-queueinfo_website")
                    width: ((parent.width - 2 * Theme.paddingLarge) / 3)
                    enabled: (web != "")
                    onClicked: Qt.openUrlExternally(web)
                }
            }
        }
    }
}





