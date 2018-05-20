import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: refreshPage

    implicitHeight: mainView.height; implicitWidth: mainView.width

    Connections {
        target:Refresh
        onStatusChanged: {
            updateStatus(status)
        }
    }

    /*
    onCanceled: {
        Refresh.cancel()
    }
    */

    function updateStatus(status) {
        switch (status) {
        case 0:
            // Uninitialised
            break;
        case 1:
            // Initialising
            progressBar.label = "Initialising"
            progressBar.visible = true
            break;
        case 2:
            // Downloading
            progressBar.label = "Downloading"
            break;
        case 3:
            // Processing
            progressBar.label = "Processing"
            break;
        case 4:
            // Overflow processing
            progressBar.label = "Processing"
            break;
        case 5:
            // Cancel
            progressBar.label = "Cancelled"
            break;
        case 6:
            // Done
            progressBar.label = "Waiting"
            progressBar.visible = false
            programmestv.clear()
            if (refreshPage.status === PageStatus.Active) {
                pageStack.pop()
            }
            break;
        default:
            break;
        }
        console.log("Refresh TV status: " + status)
    }

    Column {
        id: column
        spacing: Theme.paddingLarge
        width: parent.width

        PageHeader {
            id: header
            title: "Refresh TV Programmes"
        }

        Column {
            id: infopane
            width: parent.width
            height: 200

            Button {
                id: doRefresh
                text: "Go"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: !progressBar.visible
                onClicked: {
                    Refresh.startRefresh(1) // 0 = radio; 1 = tv
                    progressBar.enabled = true
                    progressBar.label = "Initialising"
                }
            }

            ProgressBar {
                id: progressBar
                width: parent.width
                visible: false
                indeterminate: ((Refresh.progress < 0.0) || (Refresh.progress >= 100.0))
                value: Refresh.progress
                label: "Waiting"
            }
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
            height: mainpage.height - infopane.height - header.height - (3 * Theme.paddingLarge) - mainPageHeader.visibleHeight
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
                text: Refresh.logText
                verticalAlignment: Text.AlignBottom
                x: Theme.paddingSmall
                y: Theme.paddingSmall
                //readOnly: true
                clip: true
            }
        }
    }
}
