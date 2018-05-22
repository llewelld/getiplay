import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: infoPage
    property string name: ""
    property int progId: 0
    property string statustext: ""

    Connections {
        target:Queue
        onStatusChanged: {
            if (progidchanged == progId) {
                updateStatus(status)
            }
        }
    }

    on_ExposedChanged: {
        if (_exposed) {
            console.log("Exposed changed: " + _exposed)
            status = Queue.getStatusFromId(progId)
            updateStatus(status);
        }
    }

    function updateStatus(status) {
        switch (status) {
        case 0:
            // Error
            statustext = "Error"
            addToQueue.enabled = true
            break;
        case 1:
            // Remote
            statustext = "Not yet downloaded"
            addToQueue.enabled = true
            break;
        case 2:
            // Downloading
            statustext = "Downloading"
            addToQueue.enabled = false
            break;
        case 3:
            // Local
            statustext = "Downloaded"
            addToQueue.enabled = false
            break;
        default:
            statustext = "Error"
            addToQueue.enabled = true
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
            title: qsTr("TV Programme Info")
        }

        Column {
            id: infopane
            width: parent.width
            spacing: Theme.paddingLarge

            Label {
                x: Theme.paddingLarge
                text: name
                wrapMode: Text.Wrap
                width: parent.width - (2 * Theme.paddingLarge)
            }
            Label {
                x: Theme.paddingLarge
                text: "<b>ID:</b> \t" + progId
            }
            Label {
                id: statusindicator;
                x: Theme.paddingLarge
                text: "<b>Status:</b> \t" + statustext
            }

            Button {
                id: addToQueue
                text: "Download"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: true
                onClicked: {
                    Queue.addToQueue(progId, name, 100.0, 1)
                }
            }
        }
    }
}





