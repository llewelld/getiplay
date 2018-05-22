import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: infoPage
    property string name: ""
    property int progId: 0

    Connections {
        target:Download
        onStatusChanged: {
            updateStatus(status)
        }
    }

    onCanceled: {
        Download.cancel()
    }

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
            // Converting
            progressBar.label = "Converting"
            break;
        case 4:
            // Cancel
            progressBar.label = "Cancelled"
            break;
        case 5:
            // Done
            progressBar.label = "Waiting"
            progressBar.visible = false
            // TODO: Check whether the next line is really needed
            programmesradio.clear()
            if (infoPage.status === PageStatus.Active) {
                pageStack.pop()
            }
            break;
        case 6:
            // Error
            progressBar.label = "Error"
            progressBar.visible = false
            break;
        default:
            break;
        }
        console.log("Status: " + status)
    }

    // Place our content in a Column.  The PageHeader is always placed at the top
    // of the page, followed by our content.
    Column {
        id: column
        width: infoPage.width
        spacing: Theme.paddingLarge

        PageHeader {
            id: header
            title: qsTr("Radio Programme Info")
        }

        Column {
            id: infopane
            width: parent.width

            Label {
                x: Theme.paddingLarge
                text: name
                wrapMode: Text.Wrap
                width: parent.width - (2 * Theme.paddingLarge)
            }
            Label {
                x: Theme.paddingLarge
                text: "ID: " + progId
            }

            Image {
                id: warningTriangle
                source: "image://theme/icon-lock-warning"
                asynchronous: true
                visible: (progressBar.label === "Error")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Item {
                width: 1
                height: Theme.paddingLarge
                visible: warningTriangle.visible
            }

            Button {
                id: addToQueue
                text: "Add to Queue"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: true
                onClicked: {
                    Queue.addToQueue(progId, name, 100.0, 0)
                }
            }

            /*
            Button {
                id: doDownload
                text: "Download"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: !progressBar.visible
                onClicked: {
                    Download.startDownload(progId, "radio")
                    progressBar.enabled = true
                    progressBar.label = "Initialising"
                }
            }
            */

            ProgressBar {
                id: progressBar
                width: parent.width
                visible: false
                indeterminate: ((Download.progress < 0.0) || (Download.progress >= 100.0))
                value: Download.progress
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
            height: infoPage.height - infopane.height - header.height - (3 * Theme.paddingLarge)
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
                text: Download.logText
                verticalAlignment: Text.AlignBottom
                x: Theme.paddingSmall
                y: Theme.paddingSmall
                //readOnly: true
                clip: true
            }
        }
    }
}





