/****************************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Matt Vogt <matthew.vogt@jollamobile.com>
** All rights reserved.
** 
** This file is part of Sailfish Silica UI component package.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Jolla Ltd nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
** 
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: timePickerDialog

    property int minute
    property int second
    property string title

    property date time: new Date(0, 0, 0, 0, minute, second)

    allowedOrientations: Orientation.All

    Column {
        spacing: Theme.paddingLarge
        width: parent.width

        DialogHeader {
            title: timePickerDialog.title
        }
        SkipPicker {
            id: timePicker
            anchors.horizontalCenter: parent.horizontalCenter

            Column {
                id: timerLabelColumn
                anchors.centerIn: parent
                property alias time: timePicker.time

                //: Can have two values: "LTR" if remaining time in timer item should be written in "[value] [unit]" order i.e. "2 min", or "RTL" i.e. right-to-left like in Arabic writing systems
                //% "LTR"
                property bool leftToRight: qsTrId("getiplay-settings-timer_writing_direction") !== "RTL"
                spacing: -Theme.paddingMedium
                Row {
                    spacing: Theme.paddingSmall
                    anchors.horizontalCenter: parent.horizontalCenter
                    layoutDirection: timerLabelColumn.leftToRight ? Qt.LeftToRight : Qt.RightToLeft
                    Label {
                        id: minutes
                        text: timePicker.minute.toLocaleString()
                        font.pixelSize: Theme.fontSizeHuge
                        height: Math.min(implicitHeight, (timePicker.height*0.4 - timerLabelColumn.spacing)/2)
                        fontSizeMode: Text.VerticalFit
                    }
                    Label {
                        id: minutesLabel
                        //: "Minute abbrevation. Should be short form if possible."
                        //% "min"
                        text: qsTrId("getiplay-settings-timer_minutes_short", timePicker.minute)
                        anchors.baseline: minutes.baseline
                        opacity: 0.7
                    }
                }
                Row {
                    spacing: Theme.paddingSmall
                    anchors.horizontalCenter: parent.horizontalCenter
                    layoutDirection: timerLabelColumn.leftToRight ? Qt.LeftToRight : Qt.RightToLeft
                    Label {
                        id: seconds
                        text: timePicker.second.toLocaleString()
                        font.pixelSize: Theme.fontSizeHuge
                        height: Math.min(implicitHeight, (timePicker.height*0.4 - timerLabelColumn.spacing)/2)
                        fontSizeMode: Text.VerticalFit
                    }
                    Label {
                        id: secondLabel
                        //: "Second abbrevation. Should be short form if possible."
                        //% "sec"
                        text: qsTrId("getiplay-settings-timer_seconds_short", timePicker.second)
                        anchors.baseline: seconds.baseline
                        verticalAlignment: Text.AlignBottom
                        opacity: 0.7
                    }
                }
            }
        }
    }

    onOpened: {
        timePicker.minute = minute
        timePicker.second = second
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            minute = timePicker.minute
            second = timePicker.second
        }
    }
}
