import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import Qt.labs.platform 1.0 as Labs

ApplicationWindow {

    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Material.theme: Material.Dark
    Material.accent: Material.Red

    Labs.FolderDialog {
        id: selectDirADialog

        onAccepted: {
            dirA.text = folder
        }
    }

    Labs.FolderDialog {
        id: selectDirBDialog

        onAccepted: {
            dirB.text = folder
        }
    }

    MessageDialog {
        id: messageDialog
        title: "May I have your attention please"
        text: "It's so cool that you are using Qt Quick."
    }


    ColumnLayout{

        anchors.fill: parent
        anchors.topMargin: 10
        anchors.rightMargin: 10

        RowLayout {

            TextField {
                Layout.fillWidth: true
                id : dirA
            }

            Button{
                id : selectDirA
                text : "select"
                onClicked: {
                    selectDirADialog.open();
                }
            }
        }

        RowLayout {
            TextField {
                id : dirB
                Layout.fillWidth: true
            }

            Button{
                id : selectDirB
                text : "select"
                onClicked: {
                    selectDirBDialog.open();
                }
            }

        }
        Button {
            id : compare
            text : "Compare"

            onClicked:  {
                if(dirA.text.length === 0){
                    messageDialog.text = "Please select first directory";
                    messageDialog.visible = true;
                }
                if(dirB.text.length === 0){
                    messageDialog.text = "Please select second directory";
                    messageDialog.open()
                }

                ui.compare(dirA.text, dirB.text)
            }
        }
        ProgressBar {
            id : progress
            Layout.fillWidth: true
            value: ui.crcProgress
        }

        RowLayout {
            ListView {

                id: constList
                clip: true
                Layout.fillWidth: true
                Layout.fillHeight: true

                anchors.fill: parent

                model: ui

                delegate: Label {
                    text : model.dirA + " " + model.dirB
                }

                ScrollBar.vertical: ScrollBar {
                    id: scrollbarConst
                }

            }
        }


    }
}
