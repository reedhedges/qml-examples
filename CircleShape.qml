
import QtQuick
import QtQuick.Shapes

// Draw a circle or disc using a Shape with PathAngleArc
// Specify radius property radius
// Any Shape property can also included e.g. anchors, clip,
// opacity, palette, scale, transform, visible,
// and a few ShapePath properties can be set as well, i.e.:
// fillColor, fillGradient, fillRule, dashPattern, dashOffset, 
// strokeColor, strokeStyle, strokeWidth. (Note you will need
// to import QtQuick.Shapes to access ShapePath enumerated
// constants e.g. for strokeStyle)

Shape {
  id: circleShape
  required property real radius
  property alias strokeColor: circleShapePath.strokeColor
  property alias strokeStyle: circleShapePath.strokeStyle
  property alias strokeWidth: circleShapePath.strokeWidth
  property alias fillColor: circleShapePath.fillColor
  property alias fillGradient: circleShapePath.fillGradient
  property alias fillRule: circleShapePath.fillRule
  property alias dashPattern: circleShapePath.dashPattern
  property alias dashOffset: circleShapePath.dashOffset
  ShapePath {
    id: circleShapePath
    //property alias radius: circleShape.radius
    PathAngleArc {
      centerX: radius
      centerY: radius
      radiusX: radius
      radiusY: radius
      startAngle: 0
      sweepAngle: 360
    }
  }
}
