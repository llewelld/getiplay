import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import "../component"

Page {
    id: settingsPage

    property string videoFolder: "/home/nemo/Videos/harbour-getiplay"
    property string audioFolder: "/home/nemo/Music/harbour-getiplay"

    SilicaFlickable {
        width: parent.width
        height: parent.height
        interactive: true

        anchors.fill: parent
        contentHeight: settingsColumn.height + Theme.paddingLarge

        VerticalScrollDecorator {}

        Column {
            id: settingsColumn
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Settings"
                title: qsTrId("getiplay-settings_title")
            }

            ComboBox {
                width: parent.width
                //% "Programme listing type"
                label: qsTrId("getiplay-settings_listtype")
                currentIndex: 0

                menu: ContextMenu {
                    MenuItem {
                        //% "National"
                        text: qsTrId("getiplay-settings_listtype_national")
                    }
                    MenuItem {
                        //% "Regional"
                        text: qsTrId("getiplay-settings_listtype_regional")
                    }
                    MenuItem {
                        //% "Local"
                        text: qsTrId("getiplay-settings_listtype_local")
                    }
                }
                //% "Programme lists must be refreshed to take effect"
                description: qsTrId("getiplay-settings_listtype_description")
            }

            TextField {
                //% "Web proxy URL"
                label: qsTrId("getiplay-settings_proxy")
                placeholderText: label
                width: parent.width
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
                //EnterKey.onClicked: passwordField.focus = true
            }

            ValueButton {
                //% "Video folder"
                label: qsTrId("getiplay-settings_video_folder")
                value: videoFolder
                onClicked: {
                    pageStack.push(videoPickerDialog, { })
                }
            }

            Component {
                id: videoPickerDialog
                FolderPickerDialog {
                    //% "Select video folder"
                    title: qsTrId("getiplay-settings_select_video_folder")
                    _maskedAcceptDestination: settingsPage

                    onAccepted: {
                        videoFolder = selectedFolder
                    }
                }
            }

            ValueButton {
                //% "Audio folder"
                label: qsTrId("getiplay-settings_audio_folder")
                value: audioFolder ? audioFolder : "None"
                onClicked: {
                    pageStack.push(audioPickerDialog, { })
                }
            }

            Component {
                id: audioPickerDialog
                FolderPickerDialog {
                    //% "Select audio folder"
                    title: qsTrId("getiplay-settings_select_audio_folder")
                    _maskedAcceptDestination: settingsPage

                    onAccepted: {
                        audioFolder = selectedFolder
                    }
                }
            }
        }
    }

    /*
    onDone: {
        if (result == DialogResult.Accepted) {
            // Do some stuff
        }
    }
    */
}
