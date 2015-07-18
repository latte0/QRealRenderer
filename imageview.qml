import QtQuick 2.0

import Qt.labs.folderlistmodel 1.0
import QtQuick.Controls 1.0


Rectangle {
    id: window
    width: 1000
    height: 1000

    property int highestZ: 0
    property real defaultSize: 1000
    property var currentFrame: undefined


        ListView {
            anchors.fill: parent
                model: FolderListModel {
                    id: folderModel
                    objectName: "folderModel"
                    folder: "file:///home/k/research/qrealrenderer/resources/photo/"
                    showDirs: true
                    nameFilters: ["*.png", "*.jpg", "*.gif"]
                }
                delegate: Rectangle {
                    width: 1000; height: 1000
                    border.width: 1
                    color: "black"
                    Image {
                        id: image
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: folderModel.folder + fileName
                        scale: defaultSize / Math.max(sourceSize.width, sourceSize.height)
                        antialiasing: true
                    }
                    MouseArea{
                        anchors.fill:parent
                        onClicked: {

                        }
                    }
                }
    }

}
