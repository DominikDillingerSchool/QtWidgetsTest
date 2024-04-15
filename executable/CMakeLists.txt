message(STATUS "Selecting QT Version")

if(NOT QT_DEFAULT_MAJOR_VERSION)
    set(QT_DEFAULT_MAJOR_VERSION 5 CACHE STRING "Qt version to use (5 or 6), defaults to 5")
endif()

message(STATUS "Finding QT Version ${QT_DEFAULT_MAJOR_VERSION}")

find_package(Qt${QT_DEFAULT_MAJOR_VERSION} REQUIRED COMPONENTS Core Gui Widgets)

# Qt Defines
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

## Sources
set(HDRS
    "mainwindow.h"
    "listModel.h"
    "listDelegate.h"
    "benchmark.h"
	)

set(UI
    "mainwindow.ui"
    )

set(SRCS
    "main.cpp"
    "mainwindow.cpp"
    "listModel.cpp"
    "listDelegate.cpp"
    "benchmark.cpp"
	)

set(PROJECT_SOURCES ${SRCS} ${HDRS} ${UI})

## Executable
message(STATUS "Building ${PROJECT_NAME}Executable...")
if(${QT_DEFAULT_MAJOR_VERSION} GREATER_EQUAL 6)
  find_package(Qt6 REQUIRED COMPONENTS QmlIntegration)
  qt_add_executable(${PROJECT_NAME}Executable MANUAL_FINALIZATION)
  target_link_libraries(${PROJECT_NAME}Executable PRIVATE Qt6::QmlIntegration)
else()
  add_executable(${PROJECT_NAME}Executable)
endif()
target_sources(${PROJECT_NAME}Executable PRIVATE ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME}Executable PRIVATE Qt${QT_DEFAULT_MAJOR_VERSION}::Core Qt${QT_DEFAULT_MAJOR_VERSION}::Gui Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets)
target_include_directories(${PROJECT_NAME}Executable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Static link to runtime library
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_options(${PROJECT_NAME}Executable PRIVATE -stdlib=libc++ -lpthread -g)
elseif(MSVC)
  target_compile_options(${PROJECT_NAME}Executable PRIVATE "/MT")
else()
  target_link_options(${PROJECT_NAME}Executable PRIVATE -static-libstdc++ -static-libgcc -g)
endif()

# OS dependent libraries
if(WIN32)
  target_link_libraries(${PROJECT_NAME}Executable PRIVATE ws2_32)
endif()

# Qt Properties
if(WIN32)
  set_target_properties(${PROJECT_NAME}Executable PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(APPLE)
  set_target_properties(${PROJECT_NAME}Executable PROPERTIES MACOSX_BUNDLE TRUE)
endif()

## MANUAL_FINALIZATION of qt6 exectuable
if(${QT_DEFAULT_MAJOR_VERSION} GREATER_EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME}Executable)
endif()

## Install
install(TARGETS ${PROJECT_NAME}Executable DESTINATION executable)
install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}Executable> DESTINATION executable)
