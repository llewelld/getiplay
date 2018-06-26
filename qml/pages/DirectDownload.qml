import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.getiplay.progqueue 1.0
import "../component"

Page {
    id: directDownload

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
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Direct Download"
                title: qsTrId("getiplay-directdownload_title")
            }

            TextField {
                id: progToFind
                //% "Programme ID or URL"
                label: qsTrId("getiplay-direct-download_pid_or_url")
                placeholderText: label
                width: parent.width
                inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                //text: Settings.proxyUrl
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
                //EnterKey.onClicked: passwordField.focus = true
            }

            Label {
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                text: Queue.extractPid(progToFind.text)
            }

            /*
            Row {
                width: parent.width - 2 * Theme.paddingLarge
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                Button {
                    id: addToQueue
                    //% "Download"
                    text: qsTrId("getiplay-direct-download_download")
                    width: ((parent.width - Theme.paddingLarge) / 2)
                    onClicked: {
                        if (Queue.addToQueue(progId, name, duration, type, episode, available, channel, web, description, imageid, 0)) {
                            enabled = false
                            pageStack.pop()
                        }
                    }
                }
            }
            */
        }
    }
}
