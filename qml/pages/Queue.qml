import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.getiplay.progqueue 1.0

Item {
    id: queue

    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string screenName
    property bool busy: false
    property int displayCount: Queue.downloading

    function positionAtTop() {
        listView.scrollToTop();
    }

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
                title: screenName
            }
        }

        PullDownMenu {
            MenuItem {
                //% "About"
                text: qsTrId("getiplay-queue_menu_about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                //% "Settings"
                text: qsTrId("getiplay-proglist_menu_settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
            MenuItem {
                //% "Direct Download"
                text: qsTrId("getiplay-proglist_menu_direct_download")
                onClicked: pageStack.push(Qt.resolvedUrl("DirectDownload.qml"))
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            textFormat: Text.RichText
            //% "Empty"
            text: "&nbsp;<img style=\"scale: 200%;\" src=\"image://getiplay/getiplay-bg\" />&nbsp;<br />" + qsTrId("getiplay-queue_empty_message")
            //% "Programmes you select for download will appear here"
            hintText: qsTrId("getiplay-queue_empty_hint")
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
                pageStack.push(Qt.resolvedUrl("QueueItemInfo.qml"), { name: name, progId: progId, qstatus: qstatus, duration: duration, type: type, channel: channel, description: description, episode: episode, available: available, web: web, filename: filename, imageid: imageid, position: position })
            }

            Component {
                id: queueMenuComponent
                ContextMenu {
                    MenuItem {
                        //% "Remove from queue"
                        text: qsTrId("getiplay-queue_remove")
                        enabled: (qstatus != ProgQueue.STATUS_DOWNLOADING)
                        onClicked: Queue.removeFromQueue(progId)
                    }
                    MenuItem {
                        //% "Delete"
                        text: qsTrId("getiplay-queue_delete")
                        enabled: (qstatus == ProgQueue.STATUS_LOCAL)
                        onClicked: remove(progId)
                    }

                    MenuItem {
                        //% "Requeue"
                        text: qsTrId("getiplay-queue_requeue")
                        visible: (qstatus == ProgQueue.STATUS_ERROR)
                        onClicked: Queue.requeue(progId)
                    }

                }
            }

            function remove(progId) {
                //% "Deleting file"
                remorseAction(qsTrId("getiplay-queue_delete_remourse"), function() {
                    onClicked: Queue.deleteAndRemoveFromQueue(progId)
                })
            }
        }
    }
}
