import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0
import "../component"

Page {
    id: directDownload
    property string name: ""
    property alias progId: progIdLabel.value
    property int type: 0
    property int duration: 0
    property int available: 0
    property string statustext: ""

    property string channel: ""
    property string description: ""
    property string episode: ""
    property string web: ""
    property string imageid: ""

    property bool queryrunning: false
    property bool querycomplete: false
    property bool queryshowing: false
    property bool queryerror: false

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
                if (metadata.get(0).available) {
                    available = Settings.dateToEpoch(metadata.get(0).available)
                }
                console.log("Available: " + available + " (" + metadata.get(0).available + ")")
                description = metadata.get(0).desclong
                episode = metadata.get(0).episode
                web = metadata.get(0).web
                if (metadata.get(0).thumbnail != thumbnail.source) {
                    thumbnail.source = metadata.get(0).thumbnail
                    imageid = metadata.get(0).thumbnail.substr(-12)
                    console.log("Image id: " + imageid)
                }
                console.log("Thumbnail url: " + thumbnail.source);

                if ((name != "") && (name != "get_iplayer")) {
                    queryrunning = false
                    querycomplete = true
                }
                else {
                    queryrunning = false
                    querycomplete = false
                    queryerror = true
                }
            }
        }
    }

    onCanceled: {
        console.log("Metadata cancelled");
        Metaget.cancel()
    }

    BusyIndicator {
        x: (Screen.width - width) / 2.0
        y: (Screen.height - height) / 2.0
        running: true
        visible: queryrunning
        size: BusyIndicatorSize.Large
    }

    SilicaFlickable {
        width: parent.width
        height: parent.height
        interactive: true

        anchors.fill: parent
        contentHeight: downloadColumn.height + Theme.paddingLarge

        VerticalScrollDecorator {}

        Column {
            id: downloadColumn
            width: parent.width
            spacing: 0

            PageHeader {
                //% "Direct Download"
                title: qsTrId("getiplay-directdownload_title")
            }

            Column {
                id: pidinput
                width: parent.width
                spacing: Theme.paddingLarge
                height: (queryrunning || querycomplete) ? 1 : implicitHeight
                clip: true

                Behavior on height {
                    PropertyAnimation {
                        properties: "height"
                        easing.type: Easing.OutCubic
                        duration: 1000
                    }
                }

                TextField {
                    id: progToFind
                    //% "Programme ID or URL"
                    label: qsTrId("getiplay-direct-download_pid_or_url")
                    placeholderText: label
                    width: parent.width - Theme.itemSizeMedium
                    inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    //text: Settings.proxyUrl
                    //EnterKey.iconSource: "image://theme/icon-m-enter-next"
                    //EnterKey.onClicked: passwordField.focus = true
                    enabled: !(queryrunning || querycomplete)

                    Item {
                        parent: progToFind
                        anchors.fill: parent
                        IconButton {
                            id: clearButton
                            anchors {
                                top: parent.top
                                left: parent.right
                                leftMargin: Theme.horizontalPageMargin
                            }
                            width: icon.width
                            height: icon.height
                            icon.source: "image://theme/icon-m-clear"
                            highlighted: down || progToFind._editor.activeFocus
                            opacity: progToFind.text.length > 0 ? 1 : 0
                            Behavior on opacity {
                                FadeAnimation {}
                            }

                            onClicked: {
                                progToFind.text = ""
                                console.log("Clicked")
                            }
                        }
                    }
                }

                Row {
                    width: parent.width
                    InfoRow {
                        id: progIdLabel
                        //% "PID:"
                        label: value == "" ? "" : qsTrId("getiplay-direct-download_pid")
                        value: Queue.extractPid(progToFind.text)
                        midlineRatio: 0.3
                        midlineMin: Theme.fontSizeSmall * 0
                        midlineMax: Theme.fontSizeSmall * 10
                        pixelSize: Theme.fontSizeMedium
                        labelTextBold: true
                        width: 2 * ((parent.width - Theme.paddingLarge) / 3)
                    }

                    Button {
                        id: collectData
                        //% "Query"
                        text: qsTrId("getiplay-direct-download_query")
                        width: ((parent.width - Theme.paddingLarge) / 3)
                        enabled: (progId != "") && !queryrunning
                        onClicked: {
                            queryerror = false
                            queryrunning = true
                            Metaget.startDownload(progId, type)
                        }
                    }
                }

                Rectangle {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.paddingLarge
                    color: "transparent"
                    height: 2
                }
            }

            Column {
                id: pidinfo
                width: parent.width
                spacing: Theme.paddingLarge
                height: querycomplete ? implicitHeight : 0
                clip: true

                Behavior on height {
                    PropertyAnimation {
                        properties: "height"
                        easing.type: Easing.OutCubic
                        duration: 1000

                        onRunningChanged: {
                            if (pidinfo.height > 0) {
                                queryshowing = true
                            }
                        }
                    }
                }

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
                    label: qsTrId("getiplay-direct-download_episode")
                    value: episode
                    midlineRatio: 0.25
                    midlineMin: Theme.fontSizeSmall * 5
                    midlineMax: Theme.fontSizeSmall * 10
                    pixelSize: Theme.fontSizeMedium
                    labelTextBold: true
                }
                InfoRow {
                    //% "Channel:"
                    label: qsTrId("getiplay-direct-download_channel")
                    value: channel
                    midlineRatio: 0.25
                    midlineMin: Theme.fontSizeSmall * 5
                    midlineMax: Theme.fontSizeSmall * 10
                    pixelSize: Theme.fontSizeMedium
                    labelTextBold: true
                }
                InfoRow {
                    //% "Date:"
                    label: qsTrId("getiplay-direct-download_date_available")
                    //% "Unknown"
                    value: (available != 0) ? Settings.epochToDate(available) : qsTrId("getiplay-direct-download_date_unknown")
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
                        text: qsTrId("getiplay-direct-download_download")
                        width: ((parent.width - Theme.paddingLarge) / 2)
                        enabled: (name != "")
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
                        text: qsTrId("getiplay-direct-download_website")
                        width: ((parent.width - Theme.paddingLarge) / 2)
                        enabled: (web != "")
                        onClicked: Qt.openUrlExternally(web)
                    }
                }

                Rectangle {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.paddingLarge
                    color: "transparent"
                    height: 2
                }
            }

            Column {
                width: parent.width
                spacing: Theme.paddingLarge

                Rectangle {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.paddingLarge
                    color: Theme.highlightColor
                    height: 2
                    opacity: 0.5
                    visible: !queryshowing
                }

                Label {
                    x: Theme.paddingLarge
                    //% "Error downloading programme data"
                    text: qsTrId("getiplay-direct-download_error")
                    width: parent.width - 2 * Theme.paddingLarge
                    visible: queryerror
                }
            }
        }
    }
}
