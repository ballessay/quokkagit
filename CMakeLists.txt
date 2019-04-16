cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(3rd-party/libgit2cpp)

project(quokkagit)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
  src/dialogs/blamedialog.cpp
  src/dialogs/blamedialog.h
  src/dialogs/blamedialog.ui
  src/dialogs/branchselectiondialog.cpp
  src/dialogs/branchselectiondialog.h
  src/dialogs/branchselectiondialog.ui
  src/dialogs/debuglogdialog.cpp
  src/dialogs/debuglogdialog.h
  src/dialogs/debuglogdialog.ui
  src/git2wrapper.cpp
  src/git2wrapper.h
  src/menus/logcolumnvisibilitymenu.cpp
  src/menus/logcolumnvisibilitymenu.h
  src/menus/logcontextmenu.cpp
  src/menus/logcontextmenu.h
  src/logentry.cpp
  src/logentry.h
  src/models/logmodel.cpp
  src/models/logmodel.h
  src/main.cpp
  src/mainwindow.cpp
  src/mainwindow.h
  src/mainwindow.ui
  resources/resources.qrc
)

#find_package(Git2)
find_package(Qt5 COMPONENTS Core Widgets)

target_include_directories(${PROJECT_NAME} PRIVATE src 3rd-party/better-enums)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    #${GIT2_LIBRARIES}
    git2cpp
    Qt5::Widgets
    Qt5::Core
)

#include(CPack)
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

