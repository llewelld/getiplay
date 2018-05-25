import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0

Page {
    id: infoPage
    property string name: ""
    property string progId: ""
    property int type: 0
    property string statustext: ""

    property string channel: ""
    property string description: ""
    property string episode: ""
    property string date: ""
    property string web: ""

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
        XmlRole { name: "firstbcastdate"; query: "firstbcastdate/string()" }
        XmlRole { name: "desclong"; query: "desclong/string()" }
        XmlRole { name: "episode"; query: "episode/string()" }
        XmlRole { name: "web"; query: "web/string()" }

        onCountChanged: console.log("XML count:", count)

        onStatusChanged: {
            console.log("XML status = " + status)
            if (status == XmlListModel.Ready) {
                name = metadata.get(0).name
                channel = metadata.get(0).channel
                date = metadata.get(0).firstbcastdate
                description = metadata.get(0).desclong
                episode = metadata.get(0).episode
                web = metadata.get(0).web
                thumbnail.source = metadata.get(0).thumbnail
                console.log("Thumbnail url: " + thumbnail.source);
            }
        }
    }

    on_ExposedChanged: {
        if (_exposed) {
            console.log("Exposed changed: " + _exposed)
            status = Queue.getStatusFromId(progId)
            updateStatus(status);
            Metaget.startDownload(progId, type)
        }
    }

    function updateStatus(status) {
        switch (status) {
        case 0:
            // Error
            statustext = "Error"
            addToQueue.enabled = true
            break;
        case 1:
            // Remote
            statustext = "Not yet downloaded"
            addToQueue.enabled = true
            break;
        case 2:
            // Downloading
            statustext = "Downloading"
            addToQueue.enabled = false
            break;
        case 3:
            // Local
            statustext = "Downloaded"
            addToQueue.enabled = false
            break;
        default:
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
            title: (type == 0 ? "Radio" : "TV") + qsTr(" Programme Info")
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
                text: "<b>Date:</b> \t" + date
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
                    source: ""
                    width: 832
                    height: 468
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
                        Queue.addToQueue(progId, name, 100.0, type)
                    }
                }

                Button {
                    id: connect
                    text: "Visit website"
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    enabled: (web != "")
                    onClicked: Qt.openUrlExternally(web)
                }
            }
        }
    }
}





