import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: refreshPage

    Connections {
        target:Refresh
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
            // Refreshing
            progressBar.label = "Refreshing"
            break;
        case 3:
            // Cancel
        case 4:
            // Done
            progressBar.label = "Waiting"
            progressBar.enabled = false
            programmestv.clear()
            pageStack.pop()
            break;
        default:
            break;
        }
        console.log("Status: " + status)
    }

    Column {
        id: column
        spacing: Theme.paddingLarge
        width: parent.width

        PageHeader { title: "Refreshing TV Programmes" }

        Button {
            id: doRefresh
            text: "Go"
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: true
            onClicked: {
                Refresh.startRefresh(1) // 0 = radio; 1 = tv
                progressBar.enabled = true
                progressBar.label = "Initialising"
            }
        }

        ProgressBar {
            id: progressBar
            width: parent.width
            enabled: false
            indeterminate: enabled && (((Refresh.progress < 0.0) || (Refresh.progress >= 100.0)))
            value: Refresh.progress
            label: "Waiting"
        }
    }
}
