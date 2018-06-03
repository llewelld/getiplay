import QtQuick 2.0
import Sailfish.Silica 1.0
import "../component"
import "."

Page {
    id: mainpage

    allowedOrientations: Orientation.Portrait

    Item {
        anchors.fill: parent

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
                    ProgList {
                        id: tab4;
                        //% "BBC TV Programmes"
                        screenName: qsTrId("getiplay-proglisttv_title");
                        tv: true
                    }
                    ProgList {
                        id: tab3;
                        //% "BBC Radio Programmes"
                        screenName: qsTrId("getiplay-proglistradio_title");
                        tv: false
                    }
                    Queue {
                        id: tab2;
                        //% "Queue"
                        screenName: qsTrId("getiplay-queue_title")
                    }
                    Log {
                        id: tab1;
                        //% "Log"
                        screenName: qsTrId("getiplay-log_title")
                    }
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
