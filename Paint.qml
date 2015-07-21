import QtQuick 2.0

Rectangle {
    width: 1000
    height: 1000

    property var listPointX:[]
    property var listPointY:[]
    property var xpos
    property var ypos

    Rectangle{
        id:penbox
        width:100
        height:400
        y:0

        border.width: 5
        border.color: "red"

        Text{
            anchors.centerIn:parent
            text:"Pen"
            font.pixelSize: 30
        }
    }

    Rectangle{
        id:eracerbox
        width:100
        height:400
        y:400

        border.width: 5
        border.color: "red"

        Text{
            anchors.centerIn:parent
            text:"Eraser"
            font.pixelSize: 30
        }
    }



    Rectangle{
        id:deletebox
        width:100
        height:200
        y:800

        border.width: 5
        border.color: "red"

        Text{
            anchors.centerIn:parent
            text:"Delete"
            font.pixelSize: 30
        }
    }

/*
    Column{
        id:column
        anchors.top: parent.top
        anchors.left:parent.left
        anchors.margins: 5

        spacing: 2

        Repeater{
            model: ListModel{
                ListElement { text: "Save"; mode : 0 }
                ListElement { text: "Load"; mode : 1 }
                ListElement { text: "Clear"; mode : 2 }
            }

            Rectangle{
                width: 150
                height: 150
                border.width: 1
                border.color: "Gray"
                Text{
                    anchors.centerIn: parent
                    text: model.text
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("save load button")
                        switch(model.mode){
                        case 0:
                            canvas.save("image.png")
                            break
                        case 1:
                            canvas.doClear = true
                            canvas.loadImageUrl = "image.png"
                            canvas.loadImage(canvas.loadImageUrl)
                            break
                        case 2:
                            canvas.doClear = true
                            break
                        default:
                            break
                        }
                    }
                }
            }
        }

        Repeater{
            model: ListModel{
                ListElement{ color: "#000000" }
                ListElement{ color: "#ff0000" }
                ListElement{ color: "#00ff00" }
                ListElement{ color: "#0000ff" }
            }

            Rectangle{
                width: 150
                height:150
                border.width: 1
                border.color: "Gray"
                color: model.color
                MouseArea{
                    anchors.fill:parent
                    onClicked: canvas.strokeColor = color
                }
            }
        }


    }
    */

    Canvas {
        id: canvas
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        renderTarget:  Canvas.Image
        renderStrategy: Canvas.Immediate
        antialiasing: true

        property color strokeColor: "#000000"
        property int lineWidth: 5

        property bool doClear: true
        property string loadImageUrl: ""

        onImageLoaded: requestPaint()

        onDoClearChanged: {
            if(doClear){
                requestPaint()
            }
        }

        onPaint: {
            var x1
            var y1
            var ctx = canvas.getContext("2d")

            ctx.save()

            if(doClear){
                clearPoint()
                ctx.clearRect(0,0,canvas.width, canvas.height)
                doClear = false
            }

            ctx.strokeStyle = strokeColor
            ctx.lineWidth = lineWidth

            if(isImageLoaded(loadImageUrl)){
                ctx.drawImage(loadImageUrl,0,0)
                loadImageUrl = ""
            }

            ctx.beginPath()


         //   circle(ctx, xpos, ypos, lineWidth);

            if(listPointX.length > 1 && listPointY.length > 1){
                x1 = listPointX.shift()
                y1 = listPointY.shift()
                ctx.moveTo(x1, y1)


                while(listPointX.length > 0){
                    x1 = listPointX.shift()
                    y1 = listPointY.shift()
                    ctx.lineTo(x1, y1)
                }

                listPointX.push(x1)
                listPointY.push(y1)
            }

            ctx.stroke()
            ctx.restore()
        }

        MouseArea{
            anchors.fill:parent
            onPressed: {
                if(mouse.y > 0 && mouse.y < 400 &&mouse.x > 0 && mouse.x < 400){
                    canvas.strokeColor= "#000000"
                    canvas.lineWidth= 5
                }else if(mouse.y > 400 && mouse.y < 800 && mouse.x > 0 && mouse.x < 100){
                    canvas.strokeColor="#ffffff"
                    canvas.lineWidth= 30
                }else if(mouse.y > 800 && mouse.x > 0 && mouse.x < 100){
                    canvas.doClear = true
                }else{
                    clearPoint()
                    updatePoint(mouse)
                    canvas.requestPaint()
                }
            }

            onPositionChanged: {
                updatePoint(mouse)
                canvas.requestPaint()
            }

            onReleased: {
                updatePoint(mouse)
                canvas.requestPaint()
            }
        }
    }

    function clearPoint(){
        listPointX = []
        listPointY = []
    }

    function circle(ctx, cx, cy, r){
        ctx.moveTo(r * Math.cos(0) + cx, r * Math.sin(0) + cy);
        ctx.arc(cx, cy, r, 0 , 2* Math.PI, false);
    }

    function updatePoint(mouse){
        listPointX.push(mouse.x)
        listPointY.push(mouse.y)
        xpos = mouse.x
        ypos = mouse.y
    }
}

