import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0
import "../component"

Page {
    id: infoPage
    property string name: ""
    property string progId: ""
    property int type: 0
    property int duration: 0
    property int available: 0
    property string statustext: ""

    property string channel: ""
    property string description: ""
    property string episode: ""
    property string web: ""
    property string imageid: ""

    Connections {
        target:Queue
        onStatusChanged: {
            if (progidchanged == progId) {
                updateStatus(status)
            }
        }
    }

    Connections {
        target:Metaget
        onStatusChanged: {
            console.log("Metadata status: " + status)
            if (status == 4) {
                metadata.xml = Metaget.metagot()
                metadata.reload()
            }
        }
    }

    XmlListModel {
        id: metadata
        xml: ""
        namespaceDeclarations: "declare default element namespace 'http://linuxcentre.net/xmlstuff/get_iplayer';"
        query: "/program_meta_data"

        XmlRole { name: "name"; query: "name/string()" }
        XmlRole { name: "thumbnail"; query: "thumbnail/string()" }
        XmlRole { name: "channel"; query: "channel/string()" }
        XmlRole { name: "available"; query: "available/string()" }
        XmlRole { name: "desclong"; query: "desclong/string()" }
        XmlRole { name: "episode"; query: "episode/string()" }
        XmlRole { name: "web"; query: "web/string()" }

        onCountChanged: console.log("XML count:", count)

        onStatusChanged: {
            console.log("XML status = " + status)
            if (status == XmlListModel.Ready) {
                name = metadata.get(0).name
                channel = metadata.get(0).channel
                available = Settings.dateToEpoch(metadata.get(0).available)
                description = metadata.get(0).desclong
                episode = metadata.get(0).episode
                web = metadata.get(0).web
                if (metadata.get(0).thumbnail != thumbnail.source) {
                    thumbnail.source = metadata.get(0).thumbnail
                }
                console.log("Thumbnail url: " + thumbnail.source);
            }
        }
    }

    on_ExposedChanged: {
        if (_exposed) {
            console.log("Exposed changed: " + _exposed)
            status = Queue.getStatusFromId(progId)
            updateStatus(status);
            // The following code line triggers metadata collection
            // It's currently disabled because we get all the data we need from the 'get_iplayer --listitem' call
            //Metaget.startDownload(progId, type)
            // We just need to download the thumbnail
            thumbnail.source = (imageid == "" ?
                                    "https://ichef.bbci.co.uk/images/ic/640x360/p01tqv8z.png" :
                                    "https://ichef.bbci.co.uk/images/ic/640x360/" + imageid
                                )
        }
    }

    onCanceled: {
        console.log("Metadata cancelled");
        Metaget.cancel()
    }

    function updateStatus(status) {
        switch (status) {
        case ProgQueue.STATUS_UNQUEUED:
            // Unqueued
            //% "Not queued for download"
            statustext = qsTrId("getiplay-proginfo_unqueued")
            addToQueue.enabled = true
            break;
        case ProgQueue.STATUS_ERROR:
            // Error
            //% "Error"
            statustext = qsTrId("getiplay-proginfo_error")
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_REMOTE:
            // Remote
            //% "Queued for download"
            statustext = qsTrId("getiplay-proginfo_remote")
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_DOWNLOADING:
            // Downloading
            //% "Downloading"
            statustext = qsTrId("getiplay-proginfo_downloading")
            addToQueue.enabled = false
            break;
        case ProgQueue.STATUS_LOCAL:
            // Local
            //% "Downloaded"
            statustext = qsTrId("getiplay-proginfo_local")
            addToQueue.enabled = false
            break;
        default:
            console.log("Status Error: " + status)
            //% "Unexpected error"
            statustext = qsTrId("getiplay-proginfo_status_error")
            addToQueue.enabled = true
            break;
        }
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
            width: infoPage.width
            spacing: Theme.paddingLarge

            PageHeader {
                id: header
                title: (type == 0 ?
                            //% "Radio Programme Info"
                            qsTrId("getiplay-proginfo_radio_info")
                            //% "TV Programme Info"
                          : qsTrId("getiplay-proginfo_tv_info"))
            }

            Column {
                id: infopane
                width: parent.width
                spacing: Theme.paddingLarge

                Label {
                    x: Theme.paddingLarge
                    text: name
                    wrapMode: Text.Wrap
                    width: parent.width - 2 * Theme.paddingLarge
                    height: Theme.fontSizeLarge* 2 + Theme.paddingSmall
                    clip: true
                }
                InfoRow {
                    //% "Episode:"
                    label: qsTrId("getiplay-proginfo_episode")
                    value: episode
                    midlineRatio: 0.25
                    midlineMin: Theme.fontSizeSmall * 5
                    midlineMax: Theme.fontSizeSmall * 10
                    pixelSize: Theme.fontSizeMedium
                    labelTextBold: true
                }
                InfoRow {
                    //% "Channel:"
                    label: qsTrId("getiplay-proginfo_channel")
                    value: channel
                    midlineRatio: 0.25
                    midlineMin: Theme.fontSizeSmall * 5
                    midlineMax: Theme.fontSizeSmall * 10
                    pixelSize: Theme.fontSizeMedium
                    labelTextBold: true
                }
                InfoRow {
                    //% "Date:"
                    label: qsTrId("getiplay-proginfo_date_available")
                    value: Settings.epochToDate(available)
                    midlineRatio: 0.25
                    midlineMin: Theme.fontSizeSmall * 5
                    midlineMax: Theme.fontSizeSmall * 10
                    pixelSize: Theme.fontSizeMedium
                    labelTextBold: true
                }
                InfoRow {
                    id: statusindicator
                    //% "Status:"
                    label: qsTrId("getiplay-proginfo_status")
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

                    Image {
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
                    //height: Theme.fontSizeExtraSmall * 8
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
                        id: addToQueue
                        //% "Download"
                        text: qsTrId("getiplay-proginfo_download")
                        width: ((parent.width - Theme.paddingLarge) / 2)
                        onClicked: {
                            if (Queue.addToQueue(progId, name, duration, type, episode, available, channel, web, description, imageid, 0)) {
                                enabled = false
                                pageStack.pop()
                            }
                        }
                    }

                    Button {
                        id: visitWebsite
                        //% "Visit website"
                        text: qsTrId("getiplay-proginfo_website")
                        width: ((parent.width - Theme.paddingLarge) / 2)
                        enabled: (web != "")
                        onClicked: Qt.openUrlExternally(web)
                    }
                }
            }
        }
    }
}





