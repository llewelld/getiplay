import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0
import "../component"

Page {
    id: videoViewPage
    allowedOrientations: Orientation.All
    showNavigationIndicator: false

    property string progId: ""
    property string filename: ""
    property string imageid: ""
    property bool playing: (media.playbackState == MediaPlayer.PlayingState)
    property bool controlsvisible: false
    property int controlgap: 2 * Theme.paddingLarge

    on_ExposedChanged: {
        if (_exposed) {
            media.source = filename
            var mediapos = Queue.getMediaPosition(progId)
            console.log("Set video to play from: " + mediapos)
            media.seek(mediapos)
            mediapanelvisible = false
            media.play()
            mediaplayer = media
            mediaplayerdefined = true
            appwindow.enableControls()

            var playingDetails = Queue.getDetails(progId)
            mprisPlayer.updateMetadata(playingDetails["name"] + " - " + playingDetails["episode"], playingDetails["channel"])
        }
        else {
            console.log("Record video to play from: " + media.position)
            Queue.setMediaPosition(progId, media.position)
            media.stop()
            appwindow.disableControls()
            mprisPlayer.updateMetadata("", "")
        }
    }

    Timer {
        id: controlsTimer
        interval: 10000
        running: false
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            console.log("Control fadeout")
            fadeout.start()
        }
    }

    function toggleControls() {
        if ((!controlsvisible) || (fadeout.running)) {
            console.log("Display controls")
            fadeout.stop()
            controls.opacity = 1
            controlsvisible = true
            controlsTimer.restart()
        }
        else {
            console.log("Remove controls")
            controlsTimer.stop()
            fadeout.stop()
            controlsvisible = false
        }
    }

    function openControls () {
        console.log("Display controls")
        fadeout.stop()
        controls.opacity = 1
        controlsvisible = true
        controlsTimer.restart()
    }

    MediaPlayer {
        id: media
        source: filename
        autoPlay: true
        onPositionChanged: {
            mediaslider.value = position
        }
    }

    VideoOutput {
        id: video
        anchors.fill: parent
        visible: true
        fillMode: Image.PreserveAspectFit
        source: media
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Video clicked")
            toggleControls()
        }
    }

    Item {
        id: controls
        anchors.fill: parent
        visible: controlsvisible
        opacity: 1

        NumberAnimation on opacity {
            id: fadeout
            from: 1
            to: 0
            duration: 1000
            onRunningChanged: {
                if (!running) {
                    console.log("Faded out, making invisible")
                    controlsvisible = false
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Rectangle clicked")
                toggleControls()
            }
        }

        IconButton {
            id: playbutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            icon.sourceSize.width: width
            icon.sourceSize.height: height
            icon.fillMode: Image.PreserveAspectFit
            anchors.bottom: mediaslider.top
            anchors.bottomMargin: controlgap
            anchors.horizontalCenter: parent.horizontalCenter
            icon.source: (playing ? Qt.resolvedUrl("image://theme/icon-l-pause?") : Qt.resolvedUrl("image://theme/icon-l-play?"))
                    + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onClicked: {
                openControls()
                console.log("MouseArea click 2")
                if (playing) {
                    console.log("Pause 2")
                    media.pause()
                }
                else {
                    console.log("Play 2")
                    media.play()
                }
            }
        }

        IconButtonDual {
            id: reversebutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.right: playbutton.left
            anchors.rightMargin: controlgap
            icon.source: Qt.resolvedUrl("image://getiplay/icon-l-replay?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onShortClick: {
                openControls()
                media.seek(media.position - (1000 * Settings.skipTimeShort))
                mediaslider.value = mediaslider.value - (1000 * Settings.skipTimeShort)
            }
            onLongClick: {
                openControls()
                media.seek(media.position - (1000 * Settings.skipTimeLong))
                mediaslider.value = mediaslider.value - (1000 * Settings.skipTimeLong)
            }
        }

        IconButtonDual {
            id: forwardsbutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.left: playbutton.right
            anchors.leftMargin: controlgap
            icon.source: Qt.resolvedUrl("image://getiplay/icon-l-skip?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onShortClick: {
                openControls()
                media.seek(media.position + (1000 * Settings.skipTimeShort))
                mediaslider.value = mediaslider.value + (1000 * Settings.skipTimeShort)
            }
            onLongClick: {
                openControls()
                media.seek(media.position + (1000 * Settings.skipTimeLong))
                mediaslider.value = mediaslider.value + (1000 * Settings.skipTimeLong)
            }
        }

        Slider {
            id: mediaslider
            anchors.bottom: parent.bottom
            anchors.margins: controlgap
            anchors.left: parent.left
            anchors.right: parent.right
            minimumValue: 0
            maximumValue: media.duration
            stepSize: 1
            value: 0
            enabled: media.seekable
            valueText: Settings.millisecondsToTime(sliderValue)
            onPressed: {
                openControls()
                controlsTimer.stop()
            }
            onReleased: {
                media.seek(sliderValue)
                openControls()
            }
        }
    }
}





