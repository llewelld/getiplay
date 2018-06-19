import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: detailItem
    width: parent.width
    height: Math.max(labelText.height, valueText.height)

    property alias label: labelText.text
    property alias value: valueText.text
    property real leftMargin: Theme.horizontalPageMargin
    property real rightMargin: Theme.horizontalPageMargin
    property real midlineRatio: 0.5
    property real midlineMin: 0.0
    property real midlineMax: width
    property real midLine: Math.min(Math.max((width * midlineRatio), midlineMin), midlineMax)

    Text {
        id: labelText

        y: Theme.paddingSmall
        anchors {
            left: parent.left
            right: parent.right
            rightMargin: midLine + Theme.paddingSmall
            leftMargin: detailItem.leftMargin
        }
        horizontalAlignment: Text.AlignLeft
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
        textFormat: Text.PlainText
        wrapMode: Text.Wrap
    }

    Text {
        id: valueText

        y: Theme.paddingSmall
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: midLine + Theme.paddingSmall
            rightMargin: detailItem.rightMargin
        }
        horizontalAlignment: Text.AlignLeft
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
        textFormat: Text.PlainText
        wrapMode: Text.Wrap
    }
}
