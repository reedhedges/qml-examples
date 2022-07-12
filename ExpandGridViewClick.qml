
import QtQuick
import QtQuick.Controls


// A GridView is shown at the top of the window, showing only the top two rows of items.
// An image is shown below.
// On double click, or by clicking a push button, its size is expanded to the full window
// showing more items, and the image is hidden.

Window {
    id: mainWindow
    height: 800
    width: 400
    visible: true

    Column {
        anchors.fill: parent

        Column {
            id: dataGrid
            width: parent.width


            GridView {
                id: dataGridView
                width: parent.width
                cellWidth: width/3
                cellHeight: 40
                property int borderWidth: 2
                property bool expanded: false
                property int defaultHeight: (cellHeight * 2) + (borderWidth / 2)
                property int expandedHeight: mainWindow.height - expandButton.height

                interactive: expanded
                height: expanded ? expandedHeight : defaultHeight // todo add a transition for this

                clip: true
                snapMode: GridView.SnapToRow

                displaced: Transition {
                    NumberAnimation {
                        properties: "x,y"
                        easing.type: Easing.OutQuad
                    }
                }

                delegate: Rectangle {
                    id: dataItemDelegate

                    required property string dLabel
                    required property string dText


                    width: dataGridView.cellWidth
                    height: dataGridView.cellHeight
                    border.color: "darkgrey"
                    border.width: dataGridView.borderWidth
                    Column {
                        anchors.fill: parent
                        Text {
                            text: dLabel
                            font.pointSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: Text.ElideRight
                            maximumLineCount: 2
                            width: parent.width
                        }
                        Text {
                            text: dText
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                            anchors.horizontalCenter: parent.horizontalCenter
                            elide: Text.ElideRight
                            maximumLineCount: 2
                                width: parent.width

                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: dataGridView.expanded = ! dataGridView.expanded
                    }

                } // end data item delegate container rectangle


                model: ExampleListModel {
                    id: dataGridModel
                }

            } // end GridView

            Button {
                id: expandButton
                text: dataGridView.expanded ? "^" : "v"
                enabled: true
                width: dataGridView.width
                height: 20
                onClicked: {
                    dataGridView.expanded = ! dataGridView.expanded
                }
            }
        }

        Image {
            id: image
            visible: ! dataGridView.expanded
            width: mainWindow.width
            height: mainWindow.height - dataGridView.height
            source: "qrc:/ExpandGridViewClick/image.png"
            //anchors.top: dataGridView.bottom
            //anchors.bottom: mainWindow.bottom
        }
    } // end column

} // end Window
