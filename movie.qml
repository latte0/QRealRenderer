import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
    width: 1000
    height: 1000

    MediaPlayer {
        id: player
        source: "/home/k/smile.mp4" // Point this to a suitable video file
        autoPlay: true
    }

    VideoOutput {
        source: player
        anchors.fill: parent
    }
}
