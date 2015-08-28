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
            break;
        case 4:
            progress.indeterminate = false
            progress.value = 1.0
            progress.label = "Waiting"
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

        PageHeader { title: "Refreshing Programmes" }

        Button {
            id: doRefresh
            text: "Go"
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: true
            onClicked: {
                Refresh.startRefresh()
                progress.value = 0.0
                progress.indeterminate = true
                progress.label = "Working"
            }

        }

        ProgressBar {
            id: progress
            width: parent.width
            indeterminate: false
            value: 0.0
            label: "Waiting"
            enabled: false
        }
    }
}
