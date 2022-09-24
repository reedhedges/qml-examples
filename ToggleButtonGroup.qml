import QtQuick
import QtQuick.Controls

// A row of toggle buttons in the same exclusive toggle group, like an ios segmented button control.
// Only one button can be selected ("checked"); clicking any button unselects any other, and 
// changes the 'selectedButton' property.  Initialize by setting "checked: true" on one of
// the buttons.
// Children should be buttons (inherit from AbstractButton). When this component is created,
// it will add each child component to the same ButtonGroup, and set each child component's
// 'checkable' property to true.   
// The 'selectedButton' property of ToggleButtonGroup will be the selected ("checked")
// button in the group, you can set this property to select a button, or use it to
// get properties from the selected button.  Or you can use the onPressed or onChecked handler 
// of a button to perform some action. (See Qt docs for AbstractButton and ButtonGroup.)
// See ToggleButtonGroupExample.qml for an example.


Row {
    property alias selectedButton: group.checkedButton
    ButtonGroup { 
      id: group 
      exclusive: true
    }
    Component.onCompleted: {
      for(let i = 0; i < children.length; i++)
      {
        //console.log(`${children[i]}`);
        children[i].ButtonGroup.group = group;
        children[i].checkable = true;
      }
      //console.log('done');
    }
}

