import QtQuick 2.0
import Sailfish.Silica 1.0

ComboBox {
    width: parent.width

    menu: ContextMenu {
        MenuItem {
            //% "National"
            text: qsTrId("getiplay-settings_listtype_national")
        }
        MenuItem {
            //% "Regional"
            text: qsTrId("getiplay-settings_listtype_regional")
        }
        MenuItem {
            //% "Local"
            text: qsTrId("getiplay-settings_listtype_local")
        }
        MenuItem {
            //% "All"
            text: qsTrId("getiplay-settings_listtype_all")
        }
    }
}
