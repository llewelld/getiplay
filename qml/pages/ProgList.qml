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

Item {
    id: page

    implicitHeight: mainView.height; implicitWidth: mainView.width

    property string searchString
    property bool tv: control.viewTv
    property string screenName
    property bool refreshing: false
    property int totalitems: 0

    Connections {
        target:Refresh
        onStatusChanged: {
            switch (status) {
            case 0:
                // Uninitialised
                refreshing = false
                break;
            case 1:
                // Initialising
                refreshing = true
                break;
            case 5:
                // Cancel
                refreshing = false
                break;
            case 6:
                // Done
                refreshing = false
                break;
            default:
                // Do nothing
                break;
            }
        }
    }

    onSearchStringChanged: {
        (tv ? programmestv : programmesradio).setFilterFixedString(searchString)
        totalitems = (tv ? programmestv : programmesradio).sourceModel.rowCount()
    }

    SilicaListView {
        id: listView
        model: tv ? programmestv : programmesradio
        anchors.fill: parent
        //currentIndex: -1 // otherwise currentItem will steal focus

        onCurrentIndexChanged: {
            // This nasty hack prevents the currentIndex being set
            // away from -1
            // This avoids the virtual keyboard disappearing when the
            // search filter is changed
            listView.currentIndex = -1
            //console.log("CurrentIndex: " + currentIndex)
        }

        VerticalScrollDecorator {}

        header: Column {
            id: headerColumn
            width: page.width
            height: header.height + searchField.height

            PageHeader {
                id: header
                title: screenName
            }

            SearchField {
                id: searchField
                width: parent.width
                //% "Search programmes"
                placeholderText: qsTrId("getiplay-proglist_search_placeholder")
                // Predictive text actually messes up the clear button so it only
                // works if there's more than one word (weird!), but predictive
                // is likely to be the more useful of the two, so I've left it on
                //inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                inputMethodHints: Qt.ImhNoAutoUppercase

                Binding {
                    target: page
                    property: "searchString"
                    value: searchField.text.toLowerCase().trim()
                }
            }
        }

        PullDownMenu {
            id: proglistmenu
            busy: refreshing

            MenuItem {
                //% "About"
                text: qsTrId("getiplay-proglist_menu_about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                //% "Refresh"
                text: qsTrId("getiplay-proglist_menu_refresh")
                enabled: !refreshing
                onClicked: {
                    Refresh.startRefresh((tv ? 1 : 0)) // 0 = radio; 1 = tv
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            textFormat: Text.RichText
            //% "No items found"
            text: ((totalitems == 0) ? "&nbsp;<img style=\"scale: 200%;\" src=\"file:///usr/share/harbour-getiplay/qml/images/getiplay-bg.svg\" />&nbsp;<br />" : "") + qsTrId("getiplay-proglist_empty_message")
            //% "Select Refresh from menu to populate"
            hintText: ((totalitems == 0) ? qsTrId("getiplay-proglist_select_empty_hint") : "")
        }

        delegate: BackgroundItem {
            id: delegate
            focus: false

            // The ListView animations interfere with the menu animation when
            // switching between radio and TV. The also cause the vertical
            // position of the title to jump around.
            // So sadly it's easiest to disable them.
//            ListView.onAdd: AddAnimation {
//                id: animadd
//                target: delegate
//            }
//            ListView.onRemove: RemoveAnimation {
//                id: animremove
//                target: delegate
//            }

            Label {
                property string title: name + ", " + episode + ", " + channel
                x: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                color: searchString.length > 0 ? (highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor)
                                               : (highlighted ? Theme.highlightColor : Theme.primaryColor)
                textFormat: Text.StyledText
                text: Theme.highlightText(title, searchString, Theme.highlightColor)
                width: parent.width - (2 * Theme.paddingLarge)
                elide: Text.ElideRight
                focus: false
            }
            onClicked: {
                console.log("Clicked " + name)
                pageStack.push(Qt.resolvedUrl("ProgInfo.qml"), { name: name, progId: progId, duration: duration, type: (tv ? 1 : 0), episode: episode, available: available, channel: channel, web: web, description: description, imageid: imageid })
            }
        }
    }
}


