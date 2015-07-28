import QtQuick 2.0
import QtMultimedia 5.4

Rectangle {
    width: 1000
    height: 1000

    color: "black"

    MediaPlayer {
        id: player
        source: "/home/k/Terminator Genisys Movie - Official Trailer.mp4" // Point this to a suitable video file
        autoPlay: true
        loops: MediaPlayer.Infinite
    }

    VideoOutput {
        source: player
        anchors.fill: parent
    }

}
