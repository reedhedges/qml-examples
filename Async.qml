

import QtQuick
import QtQuick.Controls
import dev.dogsbody.AsyncExamples as Examples

Window {
  width: 800
  height: 600
  visible: true
  id: mainWindow


  // Timer hack example (jusnt defers work for some time):

  Button {
      id: beginTimerExampleButton
      x: 10
      y: 10
      text: "Begin Timer Hack Example"
      onPressed: {
          Examples.TimerExample.beginWork();
      }
      enabled: ! Examples.TimerExample.running
  }

  BusyIndicator {
      x: beginTimerExampleButton.x + 200
      y: beginTimerExampleButton.y
      running: Examples.TimerExample.running
  }

  // Thread example:

  Button {
      id: beginThreadExampleButton
      x: 10
      y: 40
      text: "Begin Thread Example"
      onPressed: {
          Examples.ThreadExample.beginWork();
      }
      enabled: ! Examples.ThreadExample.running
  }

  BusyIndicator {
      x: beginThreadExampleButton.x + 200
      y: beginThreadExampleButton.y
      running: Examples.ThreadExample.running
  }

  ProgressBar {
      visible: Examples.ThreadExample.running
      x: beginThreadExampleButton.x + 400
      y: beginThreadExampleButton.y
      value: Examples.ThreadExample.progress
  }



}
