
import QtQuick
import QtQuick.Shapes

Window {
  width: 800
  height: 800
  visible: true
  CircleShape {
    x: 200
    y: 200
    radius: 100
    fillColor: "red"
    strokeColor: "black"
    strokeWidth: 2
  }

  CircleShape {
    x: 100
    y: 300
    radius: 50
    fillColor: "blue"
  }

  CircleShape {
    x: 300
    y: 400
    radius: 150
    fillColor: "yellow"
    strokeColor: "black"
    strokeWidth: 3
    strokeStyle: ShapePath.DashLine
  }

  CircleShape {
      x: 400
      y: 75
      radius: 80
      fillGradient: RadialGradient {
          focalX: 85
          focalY: 85
          centerX: 80
          centerY: 80
          centerRadius: 100
          GradientStop { position: 0; color: "cyan" }
          GradientStop { position: 1.3; color: "green" }
      }
  }
}
