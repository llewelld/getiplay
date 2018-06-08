import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.getiplay.progqueue 1.0
import harbour.getiplay.settings 1.0

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
            var mediapos = Queue.getMediaPosition(progId)
            console.log("Set media to play from: " + mediapos)
            media.seek(mediapos)
        }
        else {
            console.log("Record media to play from: " + media.position)
            Queue.setMediaPosition(progId, media.position)
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

    /*
    Image {
        visible: !video.visible
        id: thumbnail
        source: "https://ichef.bbci.co.uk/images/ic/640x360/" + imageid
        width: parent.width
        height: parent.height
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        NumberAnimation on opacity { id: fadein; from: 0; to: 1; duration: 1000 }
        onStatusChanged: {
            if (status == Image.Ready) {
                fadein.start()
            }
        }
    }
    */

    MediaPlayer {
        id: media
        source: filename
        autoPlay: true
        onPositionChanged: {
            mediaslider.value = position
        }
        onStatusChanged: {
            if (status == MediaPlayer.Loaded) {
            }
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

        IconButton {
            id: reversebutton
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.right: playbutton.left
            anchors.rightMargin: controlgap
            icon.source: Qt.resolvedUrl("image://getiplay/icon-l-10sback.png?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onClicked: {
                openControls()
                media.seek(media.position - 10000)
                mediaslider.value = mediaslider.value - 10000
            }
        }

        IconButton {
            id: forwardsbutton
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.left: playbutton.right
            anchors.leftMargin: controlgap
            icon.source: Qt.resolvedUrl("image://getiplay/icon-l-10sforw.png?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onClicked: {
                openControls()
                media.seek(media.position + 10000)
                mediaslider.value = mediaslider.value + 10000
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





