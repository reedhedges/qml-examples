
import QtQuick
import QtQuick.Controls

Window {
    id: mainWindow
    width: 250
    height: 300
    visible: true

    Column {
        spacing: 5
        anchors.fill: parent
        anchors.margins: 5

        Column {
            id: dataSourceControls
            width: mainWindow.width
            spacing: 5

            Row {
                id: dataSourceValueControls
                width: parent.width
                spacing: 5

                SpinBox {
                    id: dataSourceValueInput
                    value: dataSource.data
                }
                Button {
                    text: "Update Data Source"
                    onPressed: {
                        if(dataSource.data !== dataSourceValueInput.value)
                            dataSource.data = dataSourceValueInput.value;
                    }
                }
            }

            Row {
                width: parent.width
                spacing: 5

                Text {
                    text: "List cacheBuffer Size: "
                }
                SpinBox {
                    id: listViewCacheBufferInput
                    value: 200
                    stepSize: 100
                    from: 0
                    to: 3000
                }
            }

            CheckBox {
                width: parent.width
                id: listVisibleCheckBox
                checked: true
                text: "List Visible"
            }

            CheckBox {
                width: parent.width
                id: listLoaderAsyncCheckBox
                checked: true
                text: "List item Loaders asynchronous"
            }
        }

        component DataItemView: Text {
            visible: listVisibleCheckBox.checked
            width: dataItemsListView.width
            height: 20
            text: `${dataItemName} = ${dataItemData}`
            Component.onCompleted: {
                console.log(`list view delegate ${dataItemName} created`);
                dataItemVisible = visible;
            }
            Component.onDestruction: {
                console.log(`list view delegate ${dataItemName} destroyed`);
                dataItemVisible = false;
            }
            onVisibleChanged: {
                dataItemVisible = visible;
            }
        }

        Rectangle
        {
            visible: listVisibleCheckBox.checked
            width: mainWindow.width - parent.anchors.leftMargin - parent.anchors.rightMargin - border.width
            height: mainWindow.height - dataSourceControls.height - parent.anchors.topMargin - parent.anchors.bottomMargin - border.width
            border.width: 1
            ListView {
                id: dataItemsListView
                visible: listVisibleCheckBox.checked
                width: parent.width
                model: dataItemsListModel
                cacheBuffer: listViewCacheBufferInput.value
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5
                clip: true
                //onCountChanged: { console.log(`ListView count ${count}`); }
                delegate: Loader {
                    asynchronous: listLoaderAsyncCheckBox.checked
                    sourceComponent: DataItemView {}
                }
                //delegate: DataItemView { }
                ScrollBar.vertical: ScrollBar {
                    active: true
                }
            }
        }
    }
}
