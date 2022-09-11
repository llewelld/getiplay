import QtQuick 2.0
import Sailfish.Silica 1.0

ComboBox {
    width: parent.width

    menu: ContextMenu {
        MenuItem {
            //% "Best"
            text: qsTrId("getiplay-settings_quality_best")
        }
        MenuItem {
            //% "Good"
            text: qsTrId("getiplay-settings_quality_good")
        }
        MenuItem {
            //% "Worst"
            text: qsTrId("getiplay-settings_quality_worst")
        }
    }
}
