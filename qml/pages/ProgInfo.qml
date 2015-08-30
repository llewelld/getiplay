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


Page {
    id: page
    property string name: ""
    property int progId: 0

    Connections {
        target:Download
        onStatusChanged: {
            updateStatus(status)
        }
    }

    function updateStatus(status) {
        switch (status) {
        case 0:
            break;
        case 4:
            progress.indeterminate = false
            progress.value = 1.0
            progress.label = "Waiting"
            programmes.clear()
            pageStack.pop()
            break;
        default:
            break;
        }
        console.log("Status: " + status)
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Programme Info")
            }
            Label {
                x: Theme.paddingLarge
                text: name
                wrapMode: Text.Wrap
                width: parent.width - (2 * Theme.paddingLarge)
            }
            Label {
                x: Theme.paddingLarge
                text: "ID: " + progId
            }
            Button {
                id: doDownload
                text: "Download"
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: true
                onClicked: {
                    Download.startDownload(progId)
                    progress.value = 0.0
                    progress.indeterminate = true
                    progress.label = "Working"
                }
            }

            ProgressBar {
                id: progress
                width: parent.width
                indeterminate: false
                value: 0.0
                label: "Doin' nothin'"
                enabled: false
            }
        }
    }
}





