cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


option(BUILD_CLAR OFF)

option(BUNDLE_LIBGIT2 ON)
if (BUNDLE_LIBGIT2)
    message("BUILD libgit2")
    option(BUILD_TESTS OFF)
    option(BUILD_SHARED_LIBS OFF)
    option(BUILD_CLI OFF)
endif()

option(BUILD_LIBGIT2CPP_EXAMPLES OFF)

add_subdirectory(3rd-party/libgit2cpp)

project(turtlegit VERSION 0.2.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(version)

add_executable(${PROJECT_NAME}
    src/application.cpp
    src/application.h
    src/data/blamedata.h
    src/data/diffentry.h
    src/data/helpers.cpp
    src/data/helpers.h
    src/data/logentry.cpp
    src/data/logentry.h
    src/data/settings.cpp
    src/data/settings.h
    src/dialogs/aboutdialog.cpp
    src/dialogs/aboutdialog.h
    src/dialogs/aboutdialog.ui
    src/dialogs/blamedialog.cpp
    src/dialogs/blamedialog.h
    src/dialogs/blamedialog.ui
    src/dialogs/branchselectiondialog.cpp
    src/dialogs/branchselectiondialog.h
    src/dialogs/branchselectiondialog.ui
    src/dialogs/debuglogdialog.cpp
    src/dialogs/debuglogdialog.h
    src/dialogs/debuglogdialog.ui
    src/dialogs/settingsdialog.cpp
    src/dialogs/settingsdialog.h
    src/dialogs/settingsdialog.ui
    src/menus/logcolumnvisibilitymenu.cpp
    src/menus/logcolumnvisibilitymenu.h
    src/menus/logcontextmenu.cpp
    src/menus/logcontextmenu.h
    src/models/blamemodel.cpp
    src/models/blamemodel.h
    src/models/filelogmodel.cpp
    src/models/filelogmodel.h
    src/models/filelogproxymodel.cpp
    src/models/filelogproxymodel.h
    src/models/logfilterproxymodel.cpp
    src/models/logfilterproxymodel.h
    src/models/logmodel.cpp
    src/models/logmodel.h
    src/main.cpp
    src/tools/difftool.cpp
    src/tools/difftool.h
    src/tools/git2wrapper.cpp
    src/tools/git2wrapper.h
    src/widgets/mainwindow.cpp
    src/widgets/mainwindow.h
    src/widgets/mainwindow.ui
    src/widgets/tablewidget.cpp
    src/widgets/tablewidget.h
    resources/resources.qrc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        git2cpp
        Qt::Widgets
        Qt::Core
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

# Auto. distcc
if(UNIX)
    find_program(DISTCC_FOUND distcc)

    if(DISTCC_FOUND)
        set(ENV{CCACHE_PREFIX} distcc)
    endif(DISTCC_FOUND)
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(FILES resources/org.ballessay.turtlegit.desktop DESTINATION share/applications)
    install(FILES resources/turtlegit.png DESTINATION share/icons/hicolor/256x256/apps)
    install(FILES resources/turtlegit.appdata.xml DESTINATION share/metainfo)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


option(USE_ASAN "Build with address sanitizer" OFF)

if(USE_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()
