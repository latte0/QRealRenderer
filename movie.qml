import QtQuick 2.0
import QtMultimedia 5.4

Rectangle {
    width: 640
    height: 480
/*
    MediaPlayer {
        id: player
        source: "/home/k/smile.mp4" // Point this to a suitable video file
        autoPlay: true
    }

    VideoOutput {
        source: player
        anchors.fill: parent
    }

    */
    Camera {
           id: camera

           imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

           exposure {
               exposureCompensation: -1.0
               exposureMode: Camera.ExposurePortrait
           }

           flash.mode: Camera.FlashRedEyeReduction

           imageCapture {
               onImageCaptured: {
                   photoPreview.source = preview  // Show the preview in an Image
               }
           }

           videoRecorder {
                resolution: "640x480"
                frameRate: 15
           }
       }

       VideoOutput {
           source: camera
           anchors.fill: parent
           focus : visible // to receive focus and capture key events when visible
       }

       Image {
           id: photoPreview
           anchors.fill: parent
       }
}
