cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

option(BUILD_CLAR OFF)
option(BUNDLE_LIBGIT2 ON)
option(BUILD_LIBGIT2CPP_EXAMPLES OFF)
add_subdirectory(3rd-party/libgit2cpp)

project(quokkagit VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
  src/application.cpp
  src/application.h
  src/data/logentry.cpp
  src/data/logentry.h
  src/dialogs/blamedialog.cpp
  src/dialogs/blamedialog.h
  src/dialogs/blamedialog.ui
  src/dialogs/branchselectiondialog.cpp
  src/dialogs/branchselectiondialog.h
  src/dialogs/branchselectiondialog.ui
  src/dialogs/debuglogdialog.cpp
  src/dialogs/debuglogdialog.h
  src/dialogs/debuglogdialog.ui
  src/menus/logcolumnvisibilitymenu.cpp
  src/menus/logcolumnvisibilitymenu.h
  src/menus/logcontextmenu.cpp
  src/menus/logcontextmenu.h
  src/models/filelogmodel.cpp
  src/models/filelogmodel.h
  src/models/logfilterproxymodel.cpp
  src/models/logfilterproxymodel.h
  src/models/logmodel.cpp
  src/models/logmodel.h
  src/main.cpp
  src/tools/git2wrapper.cpp
  src/tools/git2wrapper.h
  src/tools/kdiff3.cpp
  src/tools/kdiff3.h
  src/widgets/mainwindow.cpp
  src/widgets/mainwindow.h
  src/widgets/mainwindow.ui
  src/widgets/tablewidget.cpp
  src/widgets/tablewidget.h
  resources/resources.qrc
)


find_package(Qt5 COMPONENTS Core Widgets)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    src
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    git2cpp
    Qt5::Widgets
    Qt5::Core
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Auto. CCACHE
if(UNIX)
  find_program(CCACHE_FOUND ccache)

  if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  endif(CCACHE_FOUND)
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    install(FILES resources/org.ballessay.quokkagit.desktop DESTINATION share/applications)
    install(FILES resources/quokkagit.png DESTINATION share/icons/hicolor/256x256/apps)
    install(FILES resources/quokkagit.appdata.xml DESTINATION share/metainfo)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

