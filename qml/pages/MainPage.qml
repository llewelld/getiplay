import QtQuick 2.0
import Sailfish.Silica 1.0
import "../component"
import "."

Page {
    id: mainpage

    allowedOrientations: Orientation.Portrait

    Item {
        anchors.fill: parent

        /*
        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    if (tv) {
                        pageStack.push(refreshTVLoad)
                    }
                    else {
                        pageStack.push(refreshRadioLoad)
                    }
                }
            }
        }
        */

        Rectangle {
            anchors { top: parent.top; left: parent.left; right: parent.right }
            height: parent.height - mainPageHeader.visibleHeight
            width: parent.width
            clip: true
            antialiasing: false
            color: "transparent"
            border.color: "transparent"
            border.width: 0

            SlideshowView {
                id: mainView
                objectName: "mainView"

                itemWidth: width
                itemHeight: height
                height: parent.height
                clip:true

                anchors { top: parent.top; left: parent.left; right: parent.right }
                model: VisualItemModel {
                    ProgList { id: tab4; screenName: "BBC TV Programmes"; tv: true }
                    ProgList { id: tab3; screenName: "BBC Radio Programmes"; tv: false }
                    Queue { id: tab2; screenName: "Queue"}
                    RefreshRadio { id: tab1}
                }
            }
        }

        TabPageHeader {
            id: mainPageHeader
            listView: mainView
            iconArray: [Qt.resolvedUrl("image://theme/icon-m-video"),
                Qt.resolvedUrl("image://theme/icon-m-music"),
                Qt.resolvedUrl("image://theme/icon-m-cloud-download"),
                Qt.resolvedUrl("image://theme/icon-m-document")
            ]
        }
    }
}
