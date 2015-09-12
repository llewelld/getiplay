import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page
    property string name: ""
    property int progId: 0

    Connections {
        target:Download
        onStatusChanged: {
            updateStatus(status)
        }
    }

    function updateStatus(status) {
        switch (status) {
        case 0:
            // Uninitialised
            break;
        case 1:
            // Initialising
            progressBar.label = "Initialising"
            progressBar.enabled = true
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
            break;
        case 5:
            // Done
            progressBar.label = "Waiting"
            progressBar.enabled = false
            // TODO: Check whether the next line is really needed
            programmestv.clear()
            pageStack.pop()
            break;
        default:
            break;
        }
        console.log("Status: " + status)
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("TV Programme Info")
            }
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
            Button {
                id: doDownload
                text: "Download"
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: true
                onClicked: {
                    Download.startDownload(progId)
                    progressBar.enabled = true
                    progress.label = "Initialising"
                }
            }

            ProgressBar {
                id: progressBar
                width: parent.width
                enabled: false
                indeterminate: enabled && (((Download.progress < 0.0) || (Download.progress >= 100.0)))
                value: Download.progress
                label: "Waiting"
            }
        }
    }
}





