import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts


// This example shows how to transform an image to show the image positioned (pan), oriented (rotate) and scaled (zoomed)
// within a view, relative to a fixed pointer or focus point or reticule in the center of that view (or offset within the view).

Window {
   id: mainWindow
   title: qsTr("Transforms Example")
   height: 800
   width: 1000
   visible: true

   Pane {
       id: topPane
       width: mainWindow.width
       height: 100
       Text {
           text: "This is a panel at the top of the window."
       }
   }

   Row {
    id: viewAndControlsRow
    anchors.top: topPane.bottom

    Rectangle {
      id: view
      height: mainWindow.height
      width: mainWindow.width - controls.width

      property int viewWidth: mainWindow.width - controls.width
      property int viewHeight: mainWindow.height

      property int viewCenterX: viewWidth / 2
      property int viewCenterY: viewHeight / 2

      property int pointerPosX: viewCenterX + pointerPosXInput.value //viewCenterX
      property int pointerPosY: viewCenterY + pointerPosYInput.value //viewCenterY

      Item {

          // The image position and angle are transformed with inverted input values,
          // and offset to the center of the view,
          // because the input values represent the position and orientation of the
          // fixed pointer (positioned at the center of the view), relative to the image.
          // Note the order of the transform steps.
          transform: [
              Translate {
                  x: (-xInput.value) + view.pointerPosX
                  y: (-yInput.value) + view.pointerPosY
              },
              Rotation {
                  angle: -rotInput.value
                  origin.x: view.pointerPosX //rotOriginXInput.value
                  origin.y: view.pointerPosY //rotOriginYInput.value
              },
              Scale {
                  xScale: scaleInput.value / 100.0
                  yScale: scaleInput.value / 100.0
                  origin.x: view.pointerPosX // scaleOriginXInput.value
                  origin.y: view.pointerPosY // scaleOriginYInput.value
              }
          ]

          Image {
              source: "qrc:/TransformsExample/grid-500x1000.png"
              sourceSize.width: 500
              sourceSize.height: 1000
          }

          // blue arrow pointing at a certain pixel
          Shape {
              x: selectedPixelXInput.value
              y: selectedPixelYInput.value
              id: selectedPixel
              ShapePath {
                  strokeWidth: 3
                  strokeColor: "blue"
                  fillColor: "transparent"
                  startX: 0
                  startY: 0
                  PathLine { x: 15; y: 15 }
                  PathMove { x: 0; y: 0 }
                  PathLine { x: 0; y: 10 }
                  PathMove { x: 0; y: 0 }
                  PathLine { x: 10; y: 0 }
              }
          }

      }

      // red square around focal point (grid etc. are transformed around this) with an arrow pointing up.
      Shape {
          id: centerPointer
          x: view.pointerPosX
          y: view.pointerPosY
          ShapePath {
              strokeWidth: 2
              strokeColor: "red"
              fillColor: "transparent"
              startX: -10
              startY: -10
              PathLine { x: 10; y: -10; }
              PathLine { x: 10; y: 10; }
              PathLine { x: -10; y: 10 }
              PathLine { x: -10; y: -10 }
              PathLine { x: 0; y: -10 }
              PathLine { x: 0; y: -40 }
              PathLine { x: -5; y: -30 }
              PathLine { x: 0; y: -40 }
              PathLine { x: 5; y: -30 }
          }
      }

      MouseArea {
          anchors.fill: parent
          cursorShape: Qt.CrossCursor
          onClicked: (evt) => {
              console.log(`Click on imageContainer at  ${evt.x}, ${evt.y}`);
          }
          propagateComposedEvents: true
      }
    }

    Pane {
      id: controls
      height: mainWindow.height
      width: 300
      GridLayout {
          columns: 2
          flow: GridLayout.LeftToRight

          Text { text: "X:" }
          SpinBox {
              id: xInput
              from: -1000
              to: 1000
              editable: true
              stepSize: 10
          }

          Text { text: "Y:" }
          SpinBox {
              id: yInput
              from: -1000
              to: 1000
              editable: true
              stepSize: 10
          }

          Text { text: "Rotation:" }
          SpinBox {
              id: rotInput
              from: 0
              to: 360
              editable: true
              stepSize: 5
              wrap: true
              textFromValue: function(val) {
                  return `${val} °`
              }
          }



          Text { text: "Scale:" }
          SpinBox {
              id: scaleInput
              from: 1
              to: 250
              editable: true
              stepSize: 10
              value: 100
              textFromValue: function(val) {
                return `${val} %`;
              }
          }


          Text { text: "View Center = " }
          Text { text: `${view.viewCenterX}, ${view.viewCenterY}` }

          Text { text: "Reticule Pointer Offset X:" }
          SpinBox {
              id: pointerPosXInput
              value: 0 //view.viewCenterX
              from: -1000 
              to: 1000
              editable: true
              stepSize: 10
          }

          Text {text: "Reticule Pointer Offset Y:" }
          SpinBox {
              id: pointerPosYInput
              value: 0 //view.viewCenterY
              from: -1000
              to: 1000
              editable: true
              stepSize: 10
          }

          Text { text: "Arrow X: " }
          SpinBox {
              id: selectedPixelXInput
              value: 0
              from: 0
              to: 1000
              editable: true
              stepSize: 10
          }

          Text { text: "Arrow Y: " }
          SpinBox {
              id: selectedPixelYInput
              value: 0
              from: 0
              to: 1000
              editable: true
              stepSize: 10
          }

          // Uncomment if you want controls to modify transform origins directly:
//          Text { text: "Rot. Origin X:" }
//          SpinBox {
//              id: rotOriginXInput
//              value: view.viewCenterX
//              from: -1000
//              to: 1000
//              editable: true
//              stepSize: 10
//          }

//          Text { text: "Rot. Origin Y:" }
//          SpinBox {
//              id: rotOriginYInput
//              value: view.viewCenterY
//              from: -1000
//              to: 1000
//              editable: true
//              stepSize: 10
//          }

//          Text { text: "Scale Origin X:" }
//          SpinBox {
//              id: scaleOriginXInput
//              value: view.viewCenterX
//              from: -1000
//              to: 1000
//              editable: true
//              stepSize: 10
//          }

//          Text { text: "Scale Origin Y:" }
//          SpinBox {
//              id: scaleOriginYInput
//              value: view.viewCenterY
//              from: -1000
//              to: 1000
//              editable: true
//              stepSize: 10
//          }
      }
    }
   }
}
