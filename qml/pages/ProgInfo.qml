import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0

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
            thumbnail.source = (imageid == "" ? "https://ichef.bbci.co.uk/images/ic/640x360/p01tqv8z.png" : "https://ichef.bbci.co.uk/images/ic/640x360/" + imageid + ".jpg")
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
        width: infoPage.width
        spacing: Theme.paddingLarge

        PageHeader {
            id: header
            title: (type == 0 ? "Radio" : "TV") + " Programme Info"
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
                text: "<b>Episode:</b> \t" + episode
            }
            Label {
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                text: "<b>Channel:</b> \t" + channel
            }
            Label {
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                text: "<b>Date:</b> \t" + Settings.epochToDate(available)
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

            Label {
                id: statusindicator;
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
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
                        if (Queue.addToQueue(progId, name, duration, type, episode, available, channel, web, description, imageid)) {
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





