/*
 * @file
 * @author  David Llewellyn-Jones <David.Llewellyn-Jones@cl.cam.ac.uk>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright Pico project, 2017
 *
 * @brief Pico on Sailfish OS
 * @section DESCRIPTION
 *
 * Qt interface for Pico so it can be used on Qt-based platforms.
 *
 */

import QtQuick 2.1
import Sailfish.Silica 1.0

Item {
    id: tabPageHeader

    // listView must be Silica SlideshowView and have:
    // VisualItemModel as model
    // function - moveToColumn(index)
    // Each children of VisualItemModel must have:
    // properties - busy (bool) and displayCount (int)
    // method - positionAtTop()
    property SlideshowView listView: null
    property variant iconArray: []
    property int visibleHeight: flickable.contentY + height

    anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
    height: Theme.itemSizeMedium

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: parent.height

        Row {
            anchors.fill: parent

            Repeater {
                id: sectionRepeater
                model: iconArray
                delegate: BackgroundItem {

                    width: tabPageHeader.width / sectionRepeater.count
                    height: tabPageHeader.height

                    Image {
                        id: icon
                        height: Theme.iconSizeSmall * 1.9
                        width: Theme.iconSizeSmall * 1.9
                        anchors.centerIn: parent
                        source: modelData
                    }

                   Label {
                        anchors {
                            top: parent.top; topMargin: Theme.paddingSmall
                            left: icon.right; leftMargin: -Theme.paddingMedium
                        }
                        visible: listView.model.children[index].displayCount > 0
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor

                        text: listView.model.children[index].displayCount ? listView.model.children[index].displayCount : ""
                    }

                    Loader {
                        anchors.fill: parent
                        sourceComponent: listView.model.children[index].busy
                                         ? busyIndicator : undefined
                        Component {
                            id: busyIndicator

                            Rectangle {
                                anchors.fill: parent
                                color: "black"
                                opacity: 0

                                Behavior on opacity { NumberAnimation { duration: 250 } }

                                BusyIndicator {
                                    opacity: 1
                                    anchors.centerIn: parent
                                    running: true
                                    height: tabPageHeader.height - Theme.paddingLarge
                                    width: height
                                }

                                Component.onCompleted: opacity = 0.75
                            }
                        }

                    }

                    onClicked: {
                        if (listView.currentIndex === index) {
                            listView.currentItem.positionAtTop();
                        }
                        else {
                            listView.currentIndex = index;
                        }
                    }

                }
            }
        }

        Rectangle {
            id: currentSectionIndicator
            anchors.top: parent.top
            color: Theme.highlightColor
            height: Theme.paddingSmall
            width: tabPageHeader.width / sectionRepeater.count
            x: listView.currentIndex * width

            Behavior on x {
                NumberAnimation {
                    duration: 200
                }
            }
        }
        /*
        PushUpMenu {
            MenuItem {
               onClicked: pageStack.push(Qt.resolvedUrl("../About.qml"))
               //% "About"
               text: qsTrId("getiplay-main_menu_about")
            }
        }
        */
    }
}
