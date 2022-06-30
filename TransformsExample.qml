import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts


// This example shows how to transform an image to show the image positioned (pan), oriented (rotate) and scaled (zoomed)
// within a view, relative to a fixed pointer or focus point or reticule in the center of that view (or offset within the view).

Window {
   id: mainWindow
   title: qsTr("Transforms Example")
   height: 600
   width: 800
   visible: true

   Row {
    Rectangle {
      id: view
      height: mainWindow.height
      width: mainWindow.width - controls.width

      property int viewWidth: mainWindow.width - controls.width
      property int viewHeight: mainWindow.height

      property int viewCenterX: viewWidth / 2
      property int viewCenterY: viewHeight / 2

      property int pointerPosX: pointerPosXInput.value //viewCenterX
      property int pointerPosY: pointerPosYInput.value //viewCenterY

      Image {
          source: "qrc:/TransformsExample/grid-1000x1000.png"
          sourceSize.width: 1000
          sourceSize.height: 1000

          // The image position and angle are transformed with inverted input values,
          // and offset to the center of the view,
          // because the input values represent the position and orientation of the
          // fixed pointer (positioned at the center of the view), relative to the image.
          // Note the order of the transform steps.
          transform: [
              Translate {
                  x: (-xInput.value) + view.viewCenterX
                  y: (-yInput.value) + view.viewCenterY
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
      }

      Shape {
          id: centerPointer
          x: view.pointerPosX
          y: view.pointerPosY

          // square around center of view with an arrow on top:
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
          onClicked: {
              console.log(`Click on imageContainer at  ${mouseX}, ${mouseY}`);
          }
          propagateComposedEvents: true
      }
    }

    Pane {
      id: controls
      height: mainWindow.height
      width: 225
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
              to: 100
              editable: true
              stepSize: 10
              value: 100
              textFromValue: function(val) {
                return `${val} %`;
              }
          }


          Text { text: "View Center = " }
          Text { text: `${view.viewCenterX}, ${view.viewCenterY}` }

          Text { text: "Pointer Pos X:" }
          SpinBox {
              id: pointerPosXInput
              value: view.viewCenterX
              from: 0
              to: 1000
              editable: true
              stepSize: 10
          }

          Text {text: "Pointer Pos Y:" }
          SpinBox {
              id: pointerPosYInput
              value: view.viewCenterY
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
