import QtQuick
import QtQuick.Controls

// A row of toggle buttons in the same toggle group is like an ios segmented button control
// could try a ToolButton and see what it looks like


Window {
  width: 300
  height: 300
  visible: true

  component ExampleButton : Button {   // also try ToolButton or flat:true and see what it looks like
    property string exampleString
    onPressed: { console.log(`ExampleButton ${exampleString} pressed.`); }
  }

  Column {
    anchors.centerIn: parent
    spacing: 30

    ToggleButtonGroup {
      id: exampleToggleButtonGroup

      onSelectedButtonChanged: {
        console.log(`Example: selectedButton changed to ${selectedButton} with icon.name=${selectedButton.icon.name}`);
        //if(selectedButton == left) console.log(`...left`);
        //else if(selectedButton == center) console.log(`...center`);
        //else if(selectedButton == right) console.log(`...right`);
        console.log(`--> ${selectedButton.exampleString}`);
      }

      ExampleButton {
        id: left
        icon.name: "format-justify-left"
        exampleString: 'left'
      }
      ExampleButton {
        id: center
        icon.name: "format-justify-center"
        exampleString: 'center'
      }
      ExampleButton {
        id: right
        icon.name: "format-justify-right"
        exampleString: 'right'
      }
    }

    ToggleButtonGroup {
      id: example2

      Button {
        id: oneSec
        text: "1 sec."
      }
      Button {
        text: "30 sec."
      }
      Button {
        text: "1 min."
      }
    }

    Button {
      text: "Initialize"
      onPressed: { 
        exampleToggleButtonGroup.selectedButton = left; 
        example2.selectedButton = oneSec;
        text = "Reset";
      }
    }
  }
}

  /* To do it yoruself without the ToggleButtonGroup component:
    Row {
      anchors.centerIn: parent
      ButtonGroup { id: group }
      component GroupButton : Button {
        ButtonGroup.group: group
        display: AbstractButton.TextUnderIcon
        checkable: true
      }
      GroupButton { 
        icon.name: "format-justify-left"
      }
      GroupButton { 
        icon.name: "format-justify-center"
      }
      GroupButton { 
        icon.name: "format-justify-right"
      }
    }
  */
