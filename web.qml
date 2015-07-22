import QtQuick 2.0
import QtWebKit 3.0
import QtQuick.XmlListModel 2.0

Rectangle {
    width:1000
    height:1000

    color: "black"


    WebView {
        id: webView
        anchors.fill: parent
        opacity: 0.5

        url: "https://youtube.com/watch?v=qLUnhoeaMwA"

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }

        onLoadingChanged: {
            switch (loadRequest.status)
            {
            case WebView.LoadSucceededStatus:
                opacity = 1
                break
            default:
                opacity = 0
                break
            }
        }


    }
}

