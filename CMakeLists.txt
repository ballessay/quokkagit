cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(BUNDLE_LIBGIT2 ON)
add_subdirectory(3rd-party/libgit2cpp)

project(quokkagit VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
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


# Qt
#if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
#    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

#    execute_process(
#        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
#        RESULT_VARIABLE return_code
#        OUTPUT_VARIABLE qt5_install_prefix
#        OUTPUT_STRIP_TRAILING_WHITESPACE
#    )

#    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

#    if(EXISTS ${imported_location})
#        add_executable(Qt5::windeployqt IMPORTED)

#        set_target_properties(Qt5::windeployqt PROPERTIES
#            IMPORTED_LOCATION ${imported_location}
#        )
#    endif()
#endif()


#if(Qt5_FOUND AND UNIX AND TARGET Qt5::qmake AND NOT TARGET Qt5::linuxdeployqt)
#    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

#    execute_process(
#        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
#        RESULT_VARIABLE return_code
#        OUTPUT_VARIABLE qt5_install_prefix
#        OUTPUT_STRIP_TRAILING_WHITESPACE
#    )

#    #set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")
#    find_program(LINUXDEPLOYQT_FOUND linuxdeployqt)

#    if(LINUXDEPLOYQT_FOUND)
#        add_executable(Qt5::linuxdeployqt IMPORTED)

#        set_target_properties(Qt5::linuxdeployqt PROPERTIES
#            IMPORTED_LOCATION ${LINUXDEPLOYQT_FOUND}
#        )
#    endif()

##    # execute windeployqt in a tmp directory after build
##    add_custom_command(TARGET ${PROJECT_NAME}
##        POST_BUILD
##        COMMAND Qt5::linuxdeployqt "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
##    )

#  #install(FILES CPackConfig.cmake DESTINATION ${CMAKE_BINARY_DIR})
#endif()

#INCLUDE(InstallRequiredSystemLibraries)

#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The quokkagit log viewer")
#SET(CPACK_PACKAGE_VENDOR "quokkagit")
##SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
##SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
#SET(CPACK_PACKAGE_VERSION_MAJOR "0")
#SET(CPACK_PACKAGE_VERSION_MINOR "1")
#SET(CPACK_PACKAGE_VERSION_PATCH "0")
##SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
##IF(WIN32 AND NOT UNIX)
##  # There is a bug in NSI that does not handle full unix paths properly. Make
##  # sure there is at least one set of four (4) backlasshes.
##  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
##  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
##  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
##  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
##  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
##  SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
##  SET(CPACK_NSIS_MODIFY_PATH ON)
##ELSE(WIN32 AND NOT UNIX)
##  SET(CPACK_STRIP_FILES "bin/MyExecutable")
##  SET(CPACK_SOURCE_STRIP_FILES "")
##ENDIF(WIN32 AND NOT UNIX)
#SET(CPACK_PACKAGE_EXECUTABLES "quokkagit" "quokkagit")
#if(UNIX)
#  set(CPACK_INSTALL_COMMANDS "env DESTDIR=install make install; linuxdeployqt install/usr/bin/quokkagit")
#endif()

#include(CPack)

#include(BundleUtilities)
#fixup_bundle(quokkagit Qt5)

#get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

#set(BLUB $<TARGET_FILE_NAME:${PROJECT_NAME}>)
#set(FOO $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#set(APPS $<TARGET_FILE_NAME:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>)  # paths to executables
#set(DIRS ${_qt5_qmake_location})   # directories to search for prerequisites
#INSTALL(CODE "
#   include(BundleUtilities)
#   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
#   " COMPONENT Runtime)


#INSTALL(CODE "
#   include(BundleUtilities)
#   fixup_bundle(\"${PROJECT_NAME}\"   \"\"   \"${PROJECT_BINARY_DIR}/install\")
#   " COMPONENT Runtime)

#install(SCRIPT  COMPONENT ${PROJECT_NAME})
