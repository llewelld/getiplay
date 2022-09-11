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
    property alias refreshTypeTv: refreshTypeTvEntry.currentIndex
    property alias refreshTypeRadio: refreshTypeRadioEntry.currentIndex
    property alias modeTv: modeTvEntry.currentIndex
    property alias modeRadio: modeRadioEntry.currentIndex

    Binding { target: Settings; property: "videoDir"; value: videoFolder }
    Binding { target: Settings; property: "audioDir"; value: audioFolder }
    Binding { target: Settings; property: "proxyUrl"; value: proxyUrl }
    Binding { target: Settings; property: "refreshTypeTv"; value: refreshTypeTv }
    Binding { target: Settings; property: "refreshTypeRadio"; value: refreshTypeRadio }
    Binding { target: Settings; property: "modeTv"; value: modeTv }
    Binding { target: Settings; property: "modeRadio"; value: modeRadio }

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
            spacing: Theme.paddingMedium

            PageHeader {
                //% "Settings"
                title: qsTrId("getiplay-settings_title")
            }

            SectionHeader {
                //% "Progamme Listings"
                text: qsTrId("getiplay-settings_subtitle_listings")
            }

            ListingTypeComboBox {
                id: refreshTypeTvEntry
                //% "Television listing type"
                label: qsTrId("getiplay-settings_listtype-tv")
                currentIndex: Settings.refreshTypeTv
            }

            ListingTypeComboBox {
                id: refreshTypeRadioEntry
                //% "Radio listing type"
                label: qsTrId("getiplay-settings_listtype-radio")
                currentIndex: Settings.refreshTypeRadio
            }

            Label {
                //% "Refresh lists to apply the changes"
                text: qsTrId("getiplay-settings_listtype_description")
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                font.pixelSize: Theme.fontSizeExtraSmall
                color: palette.secondaryColor
                wrapMode: Text.Wrap
            }

            SectionHeader {
                //% "Download"
                text: qsTrId("getiplay-settings_subtitle_download")
            }

            QualityComboBox {
                id: modeTvEntry
                //% "Video quality"
                label: qsTrId("getiplay-settings_videoquality")
                currentIndex: Settings.modeTv
            }

            QualityComboBox {
                id: modeRadioEntry
                //% "Audioquality"
                label: qsTrId("getiplay-settings_autioquality")
                currentIndex: Settings.modeTv
            }

            TextField {
                id: proxyUrlEntry
                //% "Web proxy URL"
                label: qsTrId("getiplay-settings_proxy")
                placeholderText: label
                width: parent.width
                text: Settings.proxyUrl
                inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
                //EnterKey.onClicked: passwordField.focus = true
            }

            Slider {
                id: indexMaxConn
                //% "Maximum connections"
                label: qsTrId("getiplay-settings_max_connections")
                minimumValue: 1
                maximumValue: 10
                value: Settings.indexMaxConn
                stepSize: 1
                handleVisible: true
                valueText: value
                width: parent.width
                onValueChanged: Settings.indexMaxConn = value
            }

            SectionHeader {
                //% "File storage"
                text: qsTrId("getiplay-settings_subtitle_file_storage")
            }

            ValueButtonRight {
                //% "Video folder"
                label: qsTrId("getiplay-settings_video_folder")
                value: videoFolder
                onClicked: {
                    pageStack.push(videoPickerDialog, { })
                }
            }

            Component {
                id: videoPickerDialog
                FolderPickerPage {
                    //% "Video folder"
                    dialogTitle: qsTrId("getiplay-settings_select_video_folder")

                    onSelectedPathChanged: {
                        videoFolder = selectedPath
                    }
                }
            }

            ValueButtonRight {
                //% "Audio folder"
                label: qsTrId("getiplay-settings_audio_folder")
                value: audioFolder
                onClicked: {
                    pageStack.push(audioPickerDialog, { })
                }
            }

            Component {
                id: audioPickerDialog
                FolderPickerPage {
                    //% "Audio folder"
                    dialogTitle: qsTrId("getiplay-settings_select_audio_folder")

                    onSelectedPathChanged: {
                        audioFolder = selectedPath
                    }
                }
            }

            SectionHeader {
                //% "Playback"
                text: qsTrId("getiplay-settings_subtitle_playback")
            }

            ValueButton {
                property int skipTime: Settings.skipTimeShort

                //% "Short skip duration"
                label: qsTrId("getiplay-settings_skip_time_short")
                value: formatTime(skipTime)
                width: parent.width
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../component/SkipPickerDialog.qml"), { minute: skipTime / 60, second: skipTime % 60, title: qsTrId("getiplay-settings_skip_time_short")})
                    dialog.accepted.connect(function() {
                        Settings.skipTimeShort = (dialog.minute * 60) + dialog.second
                        value = formatTime(skipTime)
                    })
                }
            }

            ValueButton {
                property int skipTime: Settings.skipTimeLong

                //% "Long skip duration"
                label: qsTrId("getiplay-settings_skip_time_long")
                value: formatTime(skipTime)
                width: parent.width
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../component/SkipPickerDialog.qml"), { minute: skipTime / 60, second: skipTime % 60, title: qsTrId("getiplay-settings_skip_time_long")})
                    dialog.accepted.connect(function() {
                        Settings.skipTimeLong = (dialog.minute * 60) + dialog.second
                        value = formatTime(skipTime)
                    })
                }
            }
        }
    }

    function formatTime(seconds) {
        var fmt = "m 'mins' s 'secs'"
        var date = new Date()
        date.setMinutes(seconds / 60)
        date.setSeconds(seconds % 60)
        return Qt.formatDateTime(date, fmt)
    }

    /*
    onDone: {
        if (result == DialogResult.Accepted) {
            // Do some stuff
        }
    }
    */
}
