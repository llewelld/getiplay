import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.getiplay.progqueue 1.0

Item {
    id: queue

    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string screenName

    SilicaListView {
        id: listView
        model: programmesqueue
        anchors.fill: parent
        currentIndex: -1 // otherwise currentItem will steal focus

        VerticalScrollDecorator {}

        header: Column {
            id: headerColumn
            width: queue.width
            height: header.height

            PageHeader {
                id: header
                title: "Queue"
            }
        }

        PullDownMenu {
            MenuItem {
                //% "About"
                text: qsTrId("getiplay-queue_menu_about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            textFormat: Text.RichText
            text: "&nbsp;<img style=\"scale: 200%;\" src=\"file:///usr/share/harbour-getiplay/qml/images/getiplay-bg.svg\" />&nbsp;<br />Empty"
            hintText: "Programmes you select for download will appear here"
        }

        delegate: ListItem {
            id: delegate
            focus: false
            menu: queueMenuComponent

//            ListView.onAdd: AddAnimation {
//                id: animadd
//                target: delegate
//            }
//            ListView.onRemove: RemoveAnimation {
//                id: animremove
//                target: delegate
//            }

            Row {
                x: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - Theme.paddingLarge - Theme.paddingMedium
                spacing: 0

                Rectangle {
                    width: Theme.iconSizeLarge
                    height: parent.height
                    color: "transparent"

                    ProgressCircle {
                        id: queueProgress
                        width: Theme.iconSizeLarge
                        height: parent.height
                        value: progress
                        inAlternateCycle: true;
                        visible: (qstatus == ProgQueue.STATUS_DOWNLOADING)
                    }

                    Image {
                        id: queueIcon
                        width: Theme.iconSizeLarge
                        height: parent.height
                        visible: (qstatus != ProgQueue.STATUS_DOWNLOADING)
                        fillMode: Image.PreserveAspectFit
                        source: [Qt.resolvedUrl("image://theme/icon-s-time"),
                            Qt.resolvedUrl("image://theme/icon-s-high-importance"),
                            Qt.resolvedUrl("image://theme/icon-s-time"),
                            Qt.resolvedUrl("image://theme/icon-s-update"),
                            Qt.resolvedUrl("image://theme/icon-s-installed"),
                            Qt.resolvedUrl("image://theme/icon-s-high-importance")
                        ][qstatus]
                    }
                }

                Label {
                    x: Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.primaryColor
                    textFormat: Text.StyledText
                    text: name
                    width: parent.width - (2 * Theme.paddingLarge) - (1 * Theme.iconSizeLarge)
                    elide: Text.ElideRight
                    focus: false
                }
            }
            onClicked: {
                console.log("Clicked " + name)
                pageStack.push(Qt.resolvedUrl("QueueItemInfo.qml"), { name: name, progId: progId, qstatus: qstatus, duration: duration, type: type, channel: channel, description: description, episode: episode, available: available, web: web, filename: filename })
            }

            Component {
                id: queueMenuComponent
                ContextMenu {
                    MenuItem {
                        text: "Remove from queue"
                        enabled: (qstatus != ProgQueue.STATUS_DOWNLOADING)
                        onClicked: Queue.removeFromQueue(progId)
                    }
                    MenuItem {
                        text: "Delete"
                        enabled: (qstatus == ProgQueue.STATUS_LOCAL)
                        onClicked: remove(progId)
                    }
                }
            }

            function remove(progId) {
                remorseAction("Deleting file", function() {
                    onClicked: Queue.deleteAndRemoveFromQueue(progId)
                })
            }
        }
    }
}
