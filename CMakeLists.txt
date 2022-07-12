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



qt_add_executable(ExpandGridViewDragApp ExpandGridViewDrag.cpp)
qt_add_qml_module(ExpandGridViewDragApp
    URI ExpandGridViewDrag
    VERSION 1.0
    QML_FILES ExpandGridViewDrag.qml ExampleListModel.qml
    RESOURCES image.png
)

qt_add_executable(ExpandGridViewClickApp ExpandGridViewClick.cpp)
qt_add_qml_module(ExpandGridViewClickApp
    URI ExpandGridViewClick
    VERSION 1.0
    QML_FILES ExpandGridViewClick.qml ExampleListModel.qml
    RESOURCES image.png
)

qt_add_executable(TransformsExampleApp TransformsExample.cpp)
qt_add_qml_module(TransformsExampleApp
    URI TransformsExample
    VERSION 1.0
    QML_FILES TransformsExample.qml
    RESOURCES grid-1000x1000.png
)
