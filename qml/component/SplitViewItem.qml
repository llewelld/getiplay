import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: page

    property alias open: drawer.open
    property alias opened: drawer.opened

    property alias backgroundItem: drawer.backgroundItem
    property alias background: drawer.background

    property alias foregroundItem: drawer.foregroundItem
    property alias foreground: drawer.foreground

    property alias closeOnMinimize: drawer.hideOnMinimize

    default property alias data: drawer.data

    //backNavigation: drawer.open
    //allowedOrientations: Orientation.All

    Drawer {
        id: drawer
        dock: page.orientation == Orientation.Portrait ? Dock.Left: Dock.Top
        hideOnMinimize: true
        anchors.fill: parent
    }
}
