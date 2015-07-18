
import QtQuick 2.0
import "qrc:/shared" as Shared

Rectangle {
    id: container

    color: "black"

    signal presetClicked(string name)

    property int neededHeight: view.contentItem.childrenRect.height

    ListModel {
        id: model
        ListElement {
            name: "trailers"
        }
        ListElement {
            name: "ClevverMovies"
        }
        ListElement {
            name: "nogoodflix"
        }
        ListElement {
            name: "PalaceFilms"
        }
        ListElement {
            name: "CieonMovies"
        }
        ListElement {
            name: "FilmsActuTrailers"
        }
        ListElement {
            name: "movieclipsTRAILERS"
        }
    }

    Component {
        id: delegate
        Shared.Button {
            buttonWidth: 200
            text: name
            onClicked: presetClicked(name)
        }
    }

    ListView {
        id: view
        anchors.centerIn: parent
        width: 200
        height: (container.neededHeight > parent.height) ? parent.height : container.neededHeight
        model: model
        delegate: delegate
        boundsBehavior: Flickable.StopAtBounds
    }
}
