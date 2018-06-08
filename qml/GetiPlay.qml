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

ApplicationWindow
{
    id: appwindow
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    bottomMargin: mediapanel.height
    property alias audio: audioplayer

    DockedPanel {
        id: mediapanel
        width: parent.width
        dock: Dock.Bottom
        open: true
        height: playbutton.height + 2 * Theme.paddingSmall

        IconButton {
            id: reversebutton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: playbutton.left
            anchors.rightMargin: Theme.paddingSmall
            icon.source: Qt.resolvedUrl("image://theme/icon-m-left")

            onClicked: {
                audio.seek(audio.position - 10000)
            }
        }

        IconButton {
            id: playbutton
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
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

        IconButton {
            id: forwardsbutton
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: playbutton.right
            anchors.leftMargin: Theme.paddingSmall
            icon.source: Qt.resolvedUrl("image://theme/icon-m-right")

            onClicked: {
                audio.seek(audio.position + 10000)
            }
        }
    }

    MediaPlayer {
        id: audioplayer
        source: ""
        autoPlay: false
    }
}


