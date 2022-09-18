
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
                    required property int index

                    onIndexChanged: console.log(`Index changed to ${index}. datagrid dragging item is ${dataGridViewMouseHandler.draggingItemIndex}. dragtarget is ${dataGridViewMouseHandler.dragTargetIndex}`)

                    property bool dragging : index === dataGridViewMouseHandler.draggingItemIndex
                    property bool dragTarget : index === dataGridViewMouseHandler.dragTargetIndex

                    width: dataGridView.cellWidth
                    height: dataGridView.cellHeight
                    border.color: "darkgrey"
                    border.width: dataGridView.borderWidth
                    color: dragging ? "lightsteelblue" : (dragTarget ? "lightpink" : "white")

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
                property int draggingItemIndex: -1
                property int dragTargetIndex: -1
                onPositionChanged: function (mouseEvent) {
                    if(draggingItemIndex !== -1)
                    {
                        // handle drag
                        if(mouseEvent.x < 0 || mouseEvent.x > dataGridView.width || mouseEvent.y < 0 || mouseEvent.y > dataGridView.height)
                            return;
                        let toIndex = dataGridView.indexAt(mouseEvent.x, mouseEvent.y);
                        if(toIndex !== -1 && toIndex !== draggingItemIndex)
                        {
                            //if(toIndex !== 0) toIndex -= 1;
                            const item = dataGridView.itemAtIndex(toIndex);
                            console.log(`Moving item #${draggingItemIndex} to place of item with label "${item.dLabel}"`);
                            item.color = "lightpink";
                            dataGridView.model.move(draggingItemIndex, toIndex, 1);
                            dragTargetIndex = -1;
                        }
                        return;
                    }

                    // otherwise resize
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
                    if(draggingItemIndex !== -1)
                    {
                        const item = dataGridView.itemAtIndex(draggingItemIndex);
                        item.color = "white";
                    }
                    draggingItemIndex = -1;
                    if(dragTargetIndex !== -1)
                    {
                        const item = dataGridView.itemAtIndex(dragTargetIndex);
                        item.color = "white";
                    }
                    dragTargetIndex = -1;
                }
                onReleased: item_released();
                onCanceled: item_released();

                onDoubleClicked: {
                    if(dataGridView.height > dataGridView.minHeight)
                        dataGridView.height = dataGridView.minHeight;
                    else
                        dataGridView.height = dataGridView.maxHeight;
                }
                onPressAndHold: function (mouseEvent) {
                    const i = dataGridView.indexAt(mouseEvent.x, mouseEvent.y);
                    const item = dataGridView.itemAt(mouseEvent.x, mouseEvent.y);
                    console.log(`Press and hold over data item #${i}`);
                    if(item) console.log(`label ${item.dLabel}`);
                    draggingItemIndex = i;
                    if(i === -1) return;
                    item.dragging = true;
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
