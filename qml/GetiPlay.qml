/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import "pages"
import "component"
import harbour.getiplay.settings 1.0
import org.nemomobile.mpris 1.0

ApplicationWindow
{
    id: appwindow
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    bottomMargin: mediapanel.visibleSize
    property alias audio: audioplayer
    property string audioProgId: ""
    property alias mediapanelvisible: mediapanel.visible
    property MediaPlayer mediaplayer
    property bool mediaplayerdefined: false

    Component.onDestruction: {
        stopAudio()
        console.log("Destroying");
    }

    function enableControls () {
        mediaplayerdefined = true
        mprisPlayer.canControl = true
    }

    function disableControls () {
        mediaplayerdefined = false
        mprisPlayer.canControl = false
    }

    function startAudio (progId, filename) {
        mediaplayer = audioplayer
        enableControls()
        mediapanelvisible = true
        if (audioProgId != "") {
            console.log("Record audio to play from: " + audio.position)
            Queue.setMediaPosition(audioProgId, audio.position)
            audio.stop()
        }
        audioProgId = progId
        audio.source = filename

        var playingDetails = Queue.getDetails(progId)
        mprisPlayer.updateMetadata(playingDetails["name"] + " - " + playingDetails["episode"], playingDetails["channel"])

        var mediapos = Queue.getMediaPosition(audioProgId)
        console.log("Set audio to play from: " + mediapos)
        audio.seek(mediapos)
        mediapanel.open = true
        audio.play()
    }

    function stopAudio () {
        disableControls()
        mediapanel.open = false
        console.log("Record audio to play from: " + audio.position)
        Queue.setMediaPosition(audioProgId, audio.position)
        audio.stop()
        mprisPlayer.updateMetadata("", "")
        audioProgId =  ""
    }

    DockedPanel {
        id: mediapanel
        width: parent.width
        dock: Dock.Bottom
        open: false
        height: playbutton.height + 2 * Theme.paddingSmall

        Row {
            anchors.fill: parent
            anchors.margins: Theme.paddingSmall

            IconButtonDual {
                id: reversebutton
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                icon.source: Qt.resolvedUrl("image://getiplay/icon-m-replay")

                onShortClick: {
                    audio.seek(audio.position - (1000 * Settings.skipTimeShort))
                    mprisPlayer.emitSeeked()
                }
                onLongClick: {
                    audio.seek(audio.position - (1000 * Settings.skipTimeLong))
                    mprisPlayer.emitSeeked()
                }
            }

            IconButton {
                id: playbutton
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                icon.sourceSize.width: width
                icon.sourceSize.height: height
                icon.fillMode: Image.PreserveAspectFit
                icon.source: (audio.playbackState == MediaPlayer.PlayingState) ? Qt.resolvedUrl("image://theme/icon-m-pause") : Qt.resolvedUrl("image://theme/icon-m-play")

                onClicked: {
                    if (audio.playbackState == MediaPlayer.PlayingState) {
                        audio.pause()
                    }
                    else {
                        audio.play()
                    }
                }
            }

            IconButtonDual {
                id: forwardsbutton
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                icon.source: Qt.resolvedUrl("image://getiplay/icon-m-skip")

                onShortClick: {
                    audio.seek(audio.position + (1000 * Settings.skipTimeShort))
                    mprisPlayer.emitSeeked()
                }
                onLongClick: {
                    audio.seek(audio.position + (1000 * Settings.skipTimeLong))
                    mprisPlayer.emitSeeked()
                }
            }

            Slider {
                id: audioslider
                minimumValue: 0
                maximumValue: (audio.duration > 0 ? audio.duration : 1)
                stepSize: 1
                value: 0
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - reversebutton.width - playbutton.width - forwardsbutton.width - stopbutton.width
                height: implicitHeight
                anchors.margins: 0
                leftMargin: Theme.paddingLarge
                rightMargin: Theme.paddingLarge

                onReleased: audio.seek(sliderValue)
            }

            IconButton {
                id: stopbutton
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                icon.sourceSize.width: width
                icon.sourceSize.height: height
                icon.source: Qt.resolvedUrl("image://getiplay/icon-m-eject")

                onClicked: stopAudio()
            }
        }
    }

    MediaPlayer {
        id: audioplayer
        source: ""
        autoPlay: false
        onPositionChanged: {
            if (mediaplayerdefined) {
                audioslider.value = position
                mprisPlayer.emitSeeked()
            }
        }
    }

    MprisPlayer {
        id: mprisPlayer
        serviceName: "harbour-getiplay"
        //% "harbour-getiplay"
        identity: qsTrId("getiplay-title")
        desktopEntry: "harbour-getiplay"
        supportedUriSchemes: []
        supportedMimeTypes: []

        canControl: true
        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true
        canSeek: true
        loopStatus: Mpris.None
        function updateMetadata (title, channel) {
            var update = {}
            update[Mpris.metadataToString(Mpris.Artist)] = channel
            update[Mpris.metadataToString(Mpris.Title)] = title
            metadata = update
        }
        playbackStatus: mediaplayerdefined && mediaplayer.playbackState == MediaPlayer.PlayingState ? Mpris.Playing : Mpris.Paused
        position: mediaplayerdefined ? mediaplayer.position * 1000 : 0

        onPlayRequested: {
            if (mediaplayerdefined) {
                mediaplayer.play()
            }
        }
        onPauseRequested: {
            if (mediaplayerdefined) {
                mediaplayer.pause()
            }
        }
        onPlayPauseRequested: {
            if (mediaplayerdefined) {
                if (mediaplayer.playbackState == MediaPlayer.PlayingState) {
                    mediaplayer.pause()
                }
                else {
                    mediaplayer.play()
                }
            }
        }
        onNextRequested: mediaplayer.seek(mediaplayer.position + (1000 * Settings.skipTimeShort))
        onPreviousRequested: mediaplayer.seek(mediaplayer.position - (1000 * Settings.skipTimeShort))
        onSeekRequested: {
            var position = mediaplayer.position + (offset / 1000.0)
            mediaplayer.seek(position < 0 ? 0 : position)
        }
        onSetPositionRequested: {
            mediaplayer.seek(position / 1000.0)
        }

        function emitSeeked() {
            seeked(mediaplayer.position * 1000)
        }
    }
}


