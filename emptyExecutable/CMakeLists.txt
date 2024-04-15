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
	)

set(UI
    "mainwindow.ui"
    )

set(SRCS
    "main.cpp"
    "mainwindow.cpp"
	)

set(PROJECT_SOURCES ${SRCS} ${HDRS} ${UI})

## Executable
message(STATUS "Building ${PROJECT_NAME}EmptyExecutable...")
if(${QT_DEFAULT_MAJOR_VERSION} GREATER_EQUAL 6)
  find_package(Qt6 REQUIRED COMPONENTS QmlIntegration)
  qt_add_executable(${PROJECT_NAME}EmptyExecutable MANUAL_FINALIZATION)
  target_link_libraries(${PROJECT_NAME}EmptyExecutable PRIVATE Qt6::QmlIntegration)
else()
  add_executable(${PROJECT_NAME}EmptyExecutable)
endif()
target_sources(${PROJECT_NAME}EmptyExecutable PRIVATE ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME}EmptyExecutable PRIVATE Qt${QT_DEFAULT_MAJOR_VERSION}::Core Qt${QT_DEFAULT_MAJOR_VERSION}::Gui Qt${QT_DEFAULT_MAJOR_VERSION}::Widgets)
target_include_directories(${PROJECT_NAME}EmptyExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Static link to runtime library
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_link_options(${PROJECT_NAME}EmptyExecutable PRIVATE -stdlib=libc++ -lpthread -g)
elseif(MSVC)
  target_compile_options(${PROJECT_NAME}EmptyExecutable PRIVATE "/MT")
else()
  target_link_options(${PROJECT_NAME}EmptyExecutable PRIVATE -static-libstdc++ -static-libgcc -g)
endif()

# OS dependent libraries
if(WIN32)
  target_link_libraries(${PROJECT_NAME}EmptyExecutable PRIVATE ws2_32)
endif()

# Qt Properties
if(WIN32)
  set_target_properties(${PROJECT_NAME}EmptyExecutable PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(APPLE)
  set_target_properties(${PROJECT_NAME}EmptyExecutable PROPERTIES MACOSX_BUNDLE TRUE)
endif()

## MANUAL_FINALIZATION of qt6 exectuable
if(${QT_DEFAULT_MAJOR_VERSION} GREATER_EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME}EmptyExecutable)
endif()

## Install
install(TARGETS ${PROJECT_NAME}EmptyExecutable DESTINATION executable)
install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}EmptyExecutable> DESTINATION executable)

# Find the C++ compiler path
find_program(CMAKE_CXX_COMPILER NAMES c++ g++ cl)

# Check if the C++ compiler was found
if(CMAKE_CXX_COMPILER)
  # Get the directory where the C++ compiler is located
  get_filename_component(COMPILER_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)
  message(STATUS "Found C++ compiler: ${CMAKE_CXX_COMPILER}")
else()
  message(FATAL_ERROR "Could not find C++ compiler")
endif()
#[[
install(TARGETS ${PROJECT_NAME}EmptyExecutable RUNTIME_DEPENDENCIES
  PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
  POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
  DIRECTORIES ${COMPILER_PATH}
  DESTINATION executable
)
]]

# Get the path to the Qt shared platforms directory
get_target_property(QT_EXECUTABLE_LOCATION Qt${QT_DEFAULT_MAJOR_VERSION}::Core LOCATION)
if(QT_EXECUTABLE_LOCATION STREQUAL "QT_EXECUTABLE_LOCATION-NOTFOUND")
  message(FATAL_ERROR "Could not find QT exectuable location. Missing variable QT_EXECUTABLE_LOCATION.")
endif()
message(STATUS "Found QT_EXECUTABLE_LOCATION ${QT_EXECUTABLE_LOCATION}")
set(QT_SHARE_PLATFORMS_DIR "${QT_EXECUTABLE_LOCATION}/../../share/qt${QT_DEFAULT_MAJOR_VERSION}/plugins/platforms")

install(DIRECTORY "${QT_SHARE_PLATFORMS_DIR}" DESTINATION executable)
