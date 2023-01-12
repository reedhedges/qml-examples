

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

  // C++ Thread example:

  Button {
      id: beginCppThreadExampleButton
      x: 10
      y: 40
      text: "Begin std::thread Example"
      onPressed: {
          Examples.CppThreadExample.beginWork();
      }
      enabled: ! Examples.CppThreadExample.running
  }

  BusyIndicator {
      x: beginCppThreadExampleButton.x + 200
      y: beginCppThreadExampleButton.y
      running: Examples.CppThreadExample.running && ! Examples.CppThreadExample.haveError
      id: cppThreadExampleBusyIndicator
  }

  ProgressBar {
      visible: Examples.CppThreadExample.running || Examples.CppThreadExample.haveError
      x: beginCppThreadExampleButton.x + 300
      y: beginCppThreadExampleButton.y
      value: Examples.CppThreadExample.progress
      from: 0.0
      to: 1.0
      indeterminate: (Examples.CppThreadExample.progress === -1.0) || Examples.CppThreadExample.haveError
      height: 15
  }

  Image {
      source: 'error.png'
      visible: Examples.CppThreadExample.haveError
      x: cppThreadExampleBusyIndicator.x
      y: cppThreadExampleBusyIndicator.y
      z: cppThreadExampleBusyIndicator.z + 1
        width: cppThreadExampleBusyIndicator.width
        height: cppThreadExampleBusyIndicator.height
  }

  Button {
      x: beginCppThreadExampleButton.x + 500
      y: beginCppThreadExampleButton.y
      text: "Cause Error"
      onPressed: {
          Examples.CppThreadExample.requestErrorInThread();
      }
      enabled: Examples.CppThreadExample.running
  }

  Button {
      x: beginCppThreadExampleButton.x + 600
      y: beginCppThreadExampleButton.y
      text: "Cause 3s Stall"
      onPressed: {
          Examples.CppThreadExample.requestStallInThread();
      }
      enabled: Examples.CppThreadExample.running
  }

  // QThread example:

  Button {
      id: beginQThreadExampleButton
      x: 10
      y: 70
      text: "Begin QThread Example"
      onPressed: {
          Examples.QThreadExample.beginWork();
      }
      enabled: ! Examples.QThreadExample.running
  }

  BusyIndicator {
      x: beginQThreadExampleButton.x + 200
      y: beginQThreadExampleButton.y
      running: Examples.QThreadExample.running && ! Examples.QThreadExample.haveError
      id: qThreadExampleBusyIndicator
  }

  ProgressBar {
      visible: Examples.QThreadExample.running || Examples.QThreadExample.haveError
      x: beginQThreadExampleButton.x + 300
      y: beginQThreadExampleButton.y
      value: Examples.QThreadExample.progress
      from: 0.0
      to: 1.0
      indeterminate: (Examples.QThreadExample.progress === -1.0) || Examples.QThreadExample.haveError
      height: 15
  }

  Image {
      source: 'error.png'
      visible: Examples.QThreadExample.haveError
      x: qThreadExampleBusyIndicator.x
      y: qThreadExampleBusyIndicator.y
      z: qThreadExampleBusyIndicator.z + 1
        width: qThreadExampleBusyIndicator.width
        height: qThreadExampleBusyIndicator.height
  }

  Button {
      x: beginQThreadExampleButton.x + 500
      y: beginQThreadExampleButton.y
      text: "Cause Error"
      onPressed: {
          Examples.QThreadExample.requestErrorInThread();
      }
      enabled: Examples.QThreadExample.running
  }

  Button {
      x: beginQThreadExampleButton.x + 600
      y: beginQThreadExampleButton.y
      text: "Cause 3s Stall"
      onPressed: {
          Examples.QThreadExample.requestStallInThread();
      }
      enabled: Examples.QThreadExample.running
  }

}
