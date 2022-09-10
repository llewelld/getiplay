import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Share 1.0
import "../component"
import harbour.getiplay.settings 1.0

Item {
    id: logPage
    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string screenName
    property bool busy: false
    property int displayCount: 0

    function positionAtTop() {
        open = false;
    }

    // The log output box
    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: othertitle
            title: screenName
        }

        PullDownMenu {
            id: logmenu

            MenuItem {
                //% "About"
                text: qsTrId("getiplay-log_menu_about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                //% "Settings"
                text: qsTrId("getiplay-proglist_menu_settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
            MenuItem {
                //% "Clear log"
                text: qsTrId("getiplay-log_menu_clear")
                onClicked: {
                    Log.clear();
                }
            }
            MenuItem {
                //% "Share log"
                text: qsTrId("getiplay-log_menu_share")
                onClicked: share.trigger()
            }
        }

        ShareAction {
            id: share
            mimeType: "text/plain"
            resources: [
                Settings.latestLogFile
            ]
            title: "GetiPlay log file"
        }

        Rectangle {
            color: "transparent"
            border {
                color: Theme.highlightBackgroundColor
                width: 1
            }

            y: othertitle.height + Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - othertitle.height - 2 * Theme.paddingLarge
            width: parent.width - 2 * Theme.paddingLarge
            clip: true

            Label {
                id: logOutput
                textFormat: Text.PlainText
                width: parent.width - 2 * Theme.paddingSmall
                height: logPage.height - othertitle.height - 2 * Theme.paddingLarge - 2 * Theme.paddingSmall
                wrapMode: Text.WrapAnywhere
                font.pixelSize: Theme.fontSizeTiny * 0.6
                font.family: "Monospace"
                color: Theme.highlightColor
                visible: true
                text: Log.logText
                verticalAlignment: Text.AlignBottom
                x: Theme.paddingSmall
                anchors.bottom: parent.bottom
            }
        }
    }
}

