
import QtQuick

Window {
  width: 800
  height: 800
  visible: true
  Circle {
    x: 200
    y: 200
    r: 100
    color: "red"
    border.width: 1
    border.color: "black"
  }

  Ellipse {
    x: 500
    y: 500
    height: 200
    width: 300
    color: "blue"
  }
}
