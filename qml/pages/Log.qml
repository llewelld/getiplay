import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: logPage

    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string screenName

    Column {
        id: column
        spacing: Theme.paddingLarge
        width: parent.width

        PageHeader {
            id: header
            title: qsTr("Log")
        }

        // The log output box
        Rectangle {
            color: "transparent"
            border {
                color: Theme.highlightBackgroundColor
                width: 1
            }
            //radius: Theme.paddingSmall
            anchors.horizontalCenter: parent.horizontalCenter
            //height: (24 * Theme.fontSizeTiny) + (2 * Theme.paddingLarge)
            height: mainpage.height - header.height - (3 * Theme.paddingLarge) - mainPageHeader.visibleHeight
            width: parent.width - 2 * Theme.paddingLarge
            x: Theme.paddingLarge

            //TextEdit {
            Label {
                id: logOutput
                textFormat: Text.PlainText
                width: parent.width - 2 * Theme.paddingSmall
                height: parent.height - 0 * Theme.paddingSmall
                wrapMode: Text.WrapAnywhere
                font.pixelSize: Theme.fontSizeTiny * 0.6
                font.family: "Monospace"
                color: Theme.highlightColor
                visible: true
                text: Log.logText
                verticalAlignment: Text.AlignBottom
                x: Theme.paddingSmall
                y: Theme.paddingSmall
                //readOnly: true
                clip: true
            }
        }
    }


}
