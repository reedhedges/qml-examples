
import QtQuick
import QtQuick.Controls

Window {
    id: mainWindow
    height: 800
    width: 400
    visible: true

    Column {
        id: mainItemsColumn

        Item {
            id: dataGridContainer
            width: parent.width
            height: dataGridView.height


            GridView {
                id: dataGridView
                width: parent.width
                cellWidth: width/3
                cellHeight: 40
                property int borderWidth: 2
                property int minHeight: (cellHeight * 2) + (borderWidth / 2)
                property int maxHeight: mainWindow.height

                interactive: true // we handle most mouse events ourselves but this enables scrolling via mouse wheel

                height: minHeight // todo add a transition for this

                snapMode: GridView.SnapToRow


                delegate: Rectangle {
                    id: dataItemDelegate

                    required property string dLabel
                    required property string dText


                    width: dataGridView.cellWidth
                    height: dataGridView.cellHeight
                    border.color: "darkgrey"
                    border.width: dataGridView.borderWidth
                    color:  "white"

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

                } // end data item delegate container rectangle


                //    Dummy/example data for testing. Normally the application provides model data
                model: ExampleListModel {
                    id: dataListModel
                }

            } // end GridView

            MouseArea {
                id: dataGridViewMouseHandler
                anchors.fill: parent
                property int prevY: -1

                onPositionChanged: function (mouseEvent) {
                    if(pressed && prevY !== -1)
                    {
                       const d = mouseEvent.y - prevY ;
                       dataGridView.height += d;
                       if(dataGridView.height > dataGridView.maxHeight)
                           dataGridView.height = dataGridView.maxHeight;
                       if(dataGridView.height < dataGridView.minHeight)
                           dataGridView.height = dataGridView.minHeight;
                    }
                    prevY = mouseEvent.y;
                }

                function item_released() {
                    prevY = -1;
                }
                onReleased: item_released();
                onCanceled: item_released();

                onDoubleClicked: {
                    if(dataGridView.height > dataGridView.minHeight)
                        dataGridView.height = dataGridView.minHeight;
                    else
                        dataGridView.height = dataGridView.maxHeight;
                }
            }

        } // end GridView container item

        Image {
            id: image
            width: mainWindow.width
            height: mainWindow.height - dataGridView.height
            y: dataGridView.minHeight
            source: "qrc:/ExpandGridViewDrag/image.png"
            //anchors.top: dataGridView.bottom
            //anchors.bottom: mainWindow.bottom
        }
    } // end column

} // end Window
