import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: root

    property alias label: titleText.text
    property alias value: valueText.text

    width: parent ? parent.width : 0
    height: contentItem.height
    contentHeight: visible ? Math.max(column.height + 2 * Theme.paddingMedium, Theme.itemSizeSmall) : 0
    opacity: enabled ? 1.0 : Theme.opacityLow

    Column {
        id: column

        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }

        Flow {
            width: parent.width
            move: Transition { NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad; duration: 200 } }

            Label {
                id: titleText
                color: root.down ? palette.highlightColor : palette.primaryColor
                width: Math.min(implicitWidth + Theme.paddingMedium, parent.width)
                height: text.length ? implicitHeight : 0
                fontSizeMode: Text.HorizontalFit
                minimumPixelSize: Theme.fontSizeSmallBase
                truncationMode: TruncationMode.Fade
            }

            Label {
                id: valueText
                color: palette.highlightColor
                width: Math.min(implicitWidth, parent.width)
                truncationMode: TruncationMode.Fade
                horizontalAlignment: Text.AlignRight
            }
        }
    }
}
