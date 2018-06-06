/****************************************************************************
**
** Copyright (C) 2017 Jolla Ltd.
** Contact: Raine Mäkeläinen <raine.makelainen@jollamobile.com>
**
****************************************************************************/

// GetiPlay note: This file is an edited version of the original
// David Llewellyn-Jones  - 5/6/2018

import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.private 1.0 as Private
import Sailfish.Pickers 1.0
import "private"

PickerDialog {
    id: filePicker

    property var nameFilters: [""]
    // Qt::CaseSensitivity
    property int caseSensitivity: Qt.CaseInsensitive

    property alias showSystemFiles: partitionList.showSystemFiles

    //% "Select location"
    property string title: qsTrId("components_pickers-he-select_location")

    property var _maskedAcceptDestination

    property string selectedFolder: ""

    forwardNavigation: false
    acceptDestination: forwardNavigation ? _maskedAcceptDestination : null
    acceptDestinationAction: PageStackAction.Pop

    orientationTransitions: Private.PageOrientationTransition {
        fadeTarget: _background ? listView : __silica_applicationwindow_instance.contentItem
        targetPage: filePicker
    }

    PartitionListView {
        id: partitionList

        header: Loader {
            id: headerLoader
            width: parent.width

            sourceComponent: _selectedCount > 0 ? dialogHeader : pageHeader

            Component {
                id: pageHeader
                PageHeader {
                    title: filePicker.title
                }
            }

            Component {
                id: dialogHeader
                PickerDialogHeader {
                    showBack: !_clearOnBackstep
                    selectedCount: _selectedCount
                    _glassOnly: _background
                }
            }
        }

        onSelected: {
            selectedFolder = ""
            var page = pageStack.push('private/DirectoryDialog.qml', {
                                          title: info.name,
                                          path: info.path,
                                          nameFilters: filePicker.nameFilters,
                                          caseSensitivity: filePicker.caseSensitivity,
                                          acceptDestination: filePicker._maskedAcceptDestination,
                                          acceptDestinationAction: filePicker.acceptDestinationAction,
                                          _selectedModel: filePicker._selectedModel,
                                          _animationDuration: filePicker._animationDuration,
                                          _background: filePicker._background,
                                          root: filePicker

            })

            page.accepted.connect(function() {
                filePicker._dialogDone(DialogResult.Accepted)
            })
        }
    }
}
