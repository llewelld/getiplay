import QtQuick 2.0
import Sailfish.Silica 1.0
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
        }
    }
}
