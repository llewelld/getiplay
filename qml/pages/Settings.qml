import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import "../component"
import harbour.getiplay.settings 1.0

Page {
    id: settingsPage

    property string videoFolder: Settings.videoDir
    property string audioFolder: Settings.audioDir
    property alias proxyUrl: proxyUrlEntry.text
    property alias refreshType: refreshtypeentry.currentIndex

    Binding { target: Settings; property: "videoDir"; value: videoFolder }
    Binding { target: Settings; property: "audioDir"; value: audioFolder }
    Binding { target: Settings; property: "proxyUrl"; value: proxyUrl }
    Binding { target: Settings; property: "refreshType"; value: refreshType }

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

            SectionHeader {
                //% "Download settings"
                text: qsTrId("getiplay-settings_subtitle_")
            }

            ComboBox {
                id: refreshtypeentry
                width: parent.width
                //% "Programme listing type"
                label: qsTrId("getiplay-settings_listtype")
                currentIndex: Settings.refreshType

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
                id: proxyUrlEntry
                //% "Web proxy URL"
                label: qsTrId("getiplay-settings_proxy")
                placeholderText: label
                width: parent.width
                text: Settings.proxyUrl
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
                //EnterKey.onClicked: passwordField.focus = true
            }

            SectionHeader {
                //% "File storage settings"
                text: qsTrId("getiplay-settings_subtitle_file_storage")
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
                value: audioFolder
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
