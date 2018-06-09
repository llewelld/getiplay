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

CoverBackground {
    id: cover

    property int completed: Queue.completed
    property int downloading: Queue.downloading
    property string max: Math.max(Queue.completed, Queue.downloading)
    property int numwidth: max.length
    property bool mediaavilable: mediaplayerdefined && (mediaplayer.playbackState == MediaPlayer.PlayingState)

    Image {
        id: background
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
        source: "../images/graphic-cover-getiplay-background.png"
        opacity: 0.1
    }


    Label {
        id: queuesizeCount
        text: completed
        x: Theme.paddingLarge
        width: numwidth * Theme.fontSizeMedium
        y: Theme.paddingMedium
        visible: true
        font.pixelSize: Theme.fontSizeHuge
        horizontalAlignment: Text.AlignRight
    }

    Label {
        id: sessionLabel
        //% "Programmes"
        text: qsTrId("getiplay-cover_programmes_completed_line1") + "\n"
        //% "Completed"
        + qsTrId("getiplay-cover_programmes_completed_line2")
        font.pixelSize: Theme.fontSizeExtraSmall
        visible: true
        maximumLineCount: 2
        wrapMode: Text.Wrap
        fontSizeMode: Text.HorizontalFit
        lineHeight: 0.8
        height: implicitHeight/0.8
        verticalAlignment: Text.AlignVCenter
        anchors {
            right: parent.right
            left: queuesizeCount.right
            leftMargin: Theme.paddingMedium
            baseline: queuesizeCount.baseline
            baselineOffset: lineCount > 1 ? -implicitHeight/2 : -(height-implicitHeight)/2
        }
    }

    Label {
        id: downloadingCount
        text: downloading
        x: Theme.paddingLarge
        y: Theme.paddingMedium
        width: numwidth * Theme.fontSizeMedium
        visible: true
        font.pixelSize: Theme.fontSizeHuge
        horizontalAlignment: Text.AlignRight
        anchors {
            top: queuesizeCount.baseline;
            topMargin: Theme.paddingLarge
        }
    }
    Label {
        id: availableLabel
        //% "Programmes"
        text: qsTrId("getiplay-cover_programmes_downloading_line1") + "\n"
        //% "Downloading"
        + qsTrId("getiplay-cover_programmes_downloading_line2")
        font.pixelSize: Theme.fontSizeExtraSmall
        visible: true
        maximumLineCount: 2
        wrapMode: Text.Wrap
        fontSizeMode: Text.HorizontalFit
        lineHeight: 0.8
        height: implicitHeight/0.8
        verticalAlignment: Text.AlignVCenter
        anchors {
            right: parent.right
            left: downloadingCount.right
            leftMargin: Theme.paddingMedium
            baseline: downloadingCount.baseline
            baselineOffset: lineCount > 1 ? -implicitHeight/2 : -(height-implicitHeight)/2
        }
    }

    Label {
        id: statusLabel

        visible: true
        x: Theme.paddingLarge
        color: Theme.highlightColor
        //% "GetiPlay"
        text: qsTrId("getiplay-cover_title")
        anchors { top: downloadingCount.baseline; topMargin: Theme.paddingLarge }
        height: parent.height - coverActionArea.height - statusLabel.y - Theme.paddingMedium
        fontSizeMode: Text.VerticalFit
        font.pixelSize: Theme.fontSizeLarge
        wrapMode: Text.Wrap
        width: parent.width - Theme.paddingLarge
        elide: Text.ElideNone
        maximumLineCount: 3
    }

    CoverActionList {
        id: coverAction
        enabled: mediaplayerdefined

        CoverAction {
            iconSource: "image://theme/icon-cover-backup"

            onTriggered: {
                if (mediaplayerdefined) {
                    console.log("Cover skip back 10s")
                    mediaplayer.seek(mediaplayer.position - 10000)
                }
            }
        }

        CoverAction {
            iconSource: mediaavilable ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"

            onTriggered: {
                if (mediaplayerdefined) {
                    if (mediaplayer.playbackState == MediaPlayer.PlayingState) {
                        console.log("Cover pause")
                        mediaplayer.pause()
                    }
                    else {
                        console.log("Cover play")
                        mediaplayer.play()
                    }
                }
            }
        }
    }
}


