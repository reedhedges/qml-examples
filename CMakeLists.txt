cmake_minimum_required(VERSION 3.16)

project(QMLExamples VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Quick  REQUIRED) 

add_compile_definitions($<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
link_libraries(Qt6::Quick)
set_property(GLOBAL
    PROPERTY MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    PROPERTY MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    PROPERTY MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    PROPERTY MACOSX_BUNDLE TRUE
    PROPERTY WIN32_EXECUTABLE TRUE
)


# Provide a project name as first argument, followed by the same arguments you would pass to qt_add_qml_module except URI -- it will be created based on the first argument -- and VERSION -- it will always be 1.0.
# A CMake target will be created based on the first argument as well. ("${name}ExampleApp")
macro(example)
  set(name ${ARGV0})
    #message(STATUS "name=${name} ARGV=${ARGV}")
  set(targetname ${name})
  string(APPEND targetname "ExampleApp")
  qt_add_executable(${targetname} example_main.cpp)
  set(def "-DNAME=\"${name}\"")
    #message(STATUS "def=${def}")
  target_compile_definitions(${targetname} PRIVATE ${def})
  qt_add_qml_module(${targetname} VERSION 1.0 URI ${ARGV}) #${name} VERSION 1.0 ${qmlargs})
endmacro()

example(ExpandGridViewDrag QML_FILES ExpandGridViewDrag.qml ExampleListModel.qml RESOURCES image.png)

#qt_add_executable(ExpandGridViewDragApp example_main.cpp)
#target_compile_definitions(ExpandGridViewDragApp -DNAME=ExpandGridViewDragApp)
#qt_add_qml_module(ExpandGridViewDragAp
#    URI ExpandGridViewDrag
#    VERSION 1.0
#    QML_FILES ExpandGridViewDrag.qml ExampleListModel.qml
#    RESOURCES image.png
#)

example(ExpandGridViewClick
    QML_FILES ExpandGridViewClick.qml ExampleListModel.qml
    RESOURCES image.png
)

example(TransformsExample
    QML_FILES TransformsExample.qml
    RESOURCES 
      grid-1000x1000.png
      grid-500x1000.png
)


example(CircleEllipseExample
    QML_FILES CircleEllipseExample.qml Circle.qml Ellipse.qml
)

qt_add_executable(LazyEvalPropertiesExample LazyEvalProperties.cpp LazyEvalProperties_DataSource.h LazyEvalProperties_DataItem.h)
qt_add_qml_module(LazyEvalPropertiesExample
    URI LazyEvalProperties
    VERSION 1.0
    QML_FILES LazyEvalProperties.qml 
)
