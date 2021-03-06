import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0
import "../component"
import harbour.getiplay.settings 1.0

import Sailfish.Gallery 1.0
import com.jolla.settings.accounts 1.0


SplitViewItem {
    id: logPage
    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string screenName
    property bool busy: false
    property int displayCount: 0

    function positionAtTop() {
        open = false;
    }

    Connections {
        target: mainView
        onCurrentIndexChanged: {
            if (mainView.currentIndex != 3) {
                logPage.open = false
            }
        }
    }

    background: ShareMethodList {
        height: logPage.backgroundItem.height
        header: PageHeader {
            id: header
            //% "Share Log"
            title: qsTrId("getiplay-log_share_title")
        }

        source: Settings.getLogFile(0)
        content: {
            "name" : "log.txt",
            "type" : "text/plain",
            "icon" : "image://theme/icon-l-document",
            //% "GetiPlay log file"
            "status" : qsTrId("getiplay-log_share_status"),
        }

        filter: "text/plain"

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
        }

        // Add "add account" to the footer. User must be able to
        // create accounts in a case there are none.
        footer: BackgroundItem {
            // Disable mousearea
            enabled: addAccountLabel.visible
            Label {
                id: addAccountLabel
                //% "Add account"
                text: qsTrId("getiplay-log_share_add_account")
                x: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                visible: true
            }

            onClicked: {
                accountCreator.startAccountCreation()
            }
        }

        AccountCreationManager {
            id: accountCreator
            serviceFilter: ["sharing","e-mail"]
            endDestination: logPage
            endDestinationAction: PageStackAction.Pop
        }



    }

    // The log output box
    foreground: MouseArea {
        height: logPage.foregroundItem.height
        width: parent.width

        PageHeader {
            id: othertitle
            title: screenName
        }

        Rectangle {
            color: "transparent"
            border {
                color: Theme.highlightBackgroundColor
                width: 1
            }

            y: parent.y + othertitle.height + Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - othertitle.height - 2 * Theme.paddingLarge
            width: parent.width - 2 * Theme.paddingLarge
            clip: true

            //TextEdit {
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
        onClicked: {
            logPage.open = !logPage.open
        }
    }
}

