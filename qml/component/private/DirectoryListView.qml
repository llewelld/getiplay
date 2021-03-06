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
import Sailfish.FileManager 1.0
import Sailfish.Pickers 1.0

SilicaListView {
    id: listView

    property bool highlightSelected
    property bool showParentDirectory

    signal clicked(var model, int index, string iconSource)

    function contentTypeForIcon(iconSource) {
        switch (iconSource) {
            case "image://theme/icon-m-file-image":
                return ContentType.Image
            case "image://theme/icon-m-file-audio":
                return ContentType.Music
            case "image://theme/icon-m-file-video":
                return ContentType.Video
            case "image://theme/icon-m-file-vcard":
                return ContentType.Person
            case "image://theme/icon-m-file-note":
            case "image://theme/icon-m-file-pdf":
            case "image://theme/icon-m-file-spreadsheet":
            case "image://theme/icon-m-file-presentation":
            case "image://theme/icon-m-file-formatted":
                return ContentType.Document
            default:
                return ContentType.File
        }
    }

    currentIndex: -1
    anchors.fill: parent

    delegate: FileItem {
        id: fileItem
        baseName: model.isDir ? model.fileName : model.baseName
        extension: model.isDir ? "" : (model.extension != "" ? "." + model.extension : "")
        size: model.size
        modified: model.modified
        selected: highlightSelected && model.isSelected
        iconSource: model.mimeType ? Theme.iconForMimeType(model.mimeType) : ""
        visible: !isParentDirectory || listView.showParentDirectory

        Binding {
            when: isParentDirectory && !listView.showParentDirectory
            target: fileItem
            property: 'height'
            value: 0
        }

        // Animations disabled because they occasionally cause the delegates to end up with zero height
        //ListView.onAdd: AddAnimation { target: fileItem; duration: _animationDuration }
        //ListView.onRemove: RemoveAnimation { target: fileItem; duration: _animationDuration }

        onClicked: listView.clicked(model, index, iconSource)
    }

    PullDownMenu {
        id: pdm

        property var closeAction
        onActiveChanged: {
            if (!active && closeAction) {
                closeAction()
                closeAction = undefined
            }
        }

        MenuItem {
                                                 //% "Hide hidden folders"
            text: model.includeHiddenFiles ? qsTrId("getiplay-components_pickers-me-hide_hidden_dirs")
                                                 //% "Show hidden folders"
                                               : qsTrId("getiplay-components_pickers-me-show_hidden_dirs")
            onClicked: pdm.closeAction = function() { model.includeHiddenFiles = !model.includeHiddenFiles }
        }
        MenuItem {
            //% "Sort"
            text: qsTrId("components_pickers-me-sort")
            visible: model.count > 0
            onClicked: {
                var dialog = pageStack.push(sortingPage)
                dialog.selected.connect(
                    function(sortBy, sortOrder, directorySort) {
                        if (sortBy !== model.sortBy || sortOrder !== model.sortOrder) {
                            model.sortBy = sortBy
                            model.sortOrder = sortOrder
                            model.directorySort = directorySort

                            // Wait for the changes to take effect
                            // before popping the sorting page
                            model.sortByChanged.connect(pop)
                            model.sortOrderChanged.connect(pop)
                        } else {
                            PageStack.pop()
                        }
                    }
                )
            }
            function pop() {
                pageStack.pop()
                model.sortByChanged.disconnect(pop)
                model.sortOrderChanged.disconnect(pop)
            }

            Component {
                id: sortingPage

                SortingPage {}
            }
        }
    }

    ViewPlaceholder {
        //: Empty state text if there are no folders to show
        //% "Select Accept to choose this folder"
        text: qsTrId("getiplay-components_pickers-ph-unreadable_location")
        enabled: model.populated && model.count == 1 && !page.showParentDirectory
    }

    ViewPlaceholder {
        //: Empty state text if the path is not readable by the application
        //% "The location cannot be accessed"
        text: qsTrId("components_pickers-ph-unreadable_location")
        enabled: model.populated && model.count == 0
    }

    VerticalScrollDecorator {}
}
