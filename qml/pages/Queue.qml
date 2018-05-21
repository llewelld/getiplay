import QtQuick 2.0
import Sailfish.Silica 1.0

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
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: "Empty"
        }

        delegate: BackgroundItem {
            id: delegate
            focus: false

            // The ListView animations interfere with the menu animation when
            // switching between radio and TV. The also cause the vertical
            // position of the title to jump around.
            // So sadly it's easiest to disable them.
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

                ProgressCircle {
                    id: queueProgress
                    width: Theme.iconSizeLarge
                    height: parent.height
                    value: 0.0
                    inAlternateCycle: true;
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
                //pageStack.push(Qt.resolvedUrl("ProgInfoTV.qml"), { name: name, progId: progId })
            }
        }
    }
}
