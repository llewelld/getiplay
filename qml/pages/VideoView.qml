import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0

Page {
    id: videoViewPage
    property string filename: ""
    Image {
        visible: true
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

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: Qt.resolvedUrl("image://theme/icon-l-play")

        MouseArea {
            anchors.fill: parent
            onClicked: {
                video.visible = true
                video.play()
            }
        }
    }

    Video {
        visible: false
        id: video
        source: ""
        width: parent.width - 8
        height: parent.height - 8
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: {
                video.play()
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





