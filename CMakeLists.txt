cmake_minimum_required(VERSION 3.1)
project(k9copy CXX)
set (VERSION 3.0.3)

SET(CMAKE_BUILD_TYPE Debug)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_STANDARD 14)
elseif(COMPILER_SUPPORTS_CXX0X)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_STANDARD 14)

include(FindPkgConfig)

find_package(Git)

if (GIT_FOUND)
  message("git found: ${GIT_EXECUTABLE}")
  # Get the current working branch
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  # Get the latest abbreviated commit hash of the working branch
  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  
  # Get the latest commit date of the working branch
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  #TODO:PTZ160811 handling compatible.. with project name!
  set(git_tag  ${GIT_BRANCH})
  set(GIT_REPOSITORY git@github.com:ptizoom/k9copy.git)
  set(GIT_REMOTE_NAME ptizoom)
  #set(GIT_SUBMODULES dependencies/asn1c)
endif (GIT_FOUND)


# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
#include(FindQt)
find_package(Qt5 COMPONENTS REQUIRED
  Network Gui Core Xml X11Extras DBus Widgets
  )

#sed -i 's,kpushbutton.h,qpushbutton.h,g;s,KPushButton,QPushButton,g' $UI;
#for UI in `ls */*/*.ui`; 
#sed -i 's,kpushbutton.h,qpushbutton.h,g' $UI; 
#sed -i 's,KPushButton,QPushButton,g' $UI; 
# add_custom_command(TARGET SCRIPTED_EDITS
#   PRE_BUILD #| PRE_LINK | POST_BUILD
#   COMMAND chmod +x ${CMAKE_SOURCE_DIR}/setupUI4Qt5.sh
#   #COMMAND sed -i 's,kpushbutton.h,qpushbutton.h,g;s,KPushButton,QPushButton,g'
#   #[COMMAND command2 [ARGS] [args2...] ...]
#   BYPRODUCTS ${CMAKE_SOURCE_DIR}/setupUI4Qt5.sh
#   #[BYPRODUCTS [files...]]
#   #[WORKING_DIRECTORY dir]
#   #[COMMENT comment]
#   #[VERBATIM] [USES_TERMINAL]
#   )
##TODO:PTZ161112  NOT USED as being gitted.
##TODO: PTZ161111 or use AUTOUIC_OPTIONS #in AUTOGEN_TARGETS_FOLDER
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(autouic_options_
  -tr tr2i18n
  #-include klocalizedstring.h
  )

#file(GLOB_RECURSE ui_f  "${CMAKE_SOURCE_DIR}/*/*/*.ui")
file(GLOB_RECURSE _ui_f
  RELATIVE ${CMAKE_SOURCE_DIR}
  "*.ui"
  )
foreach(_f ${_ui_f_})
  add_custom_command(OUTPUT ${_f}
    COMMAND sed -i 's,kpushbutton.h,qpushbutton.h,g;s,KPushButton,QPushButton,g' $(<) > $(@)
    BYPRODUCTS ${_f}
    DEPENDS ${CMAKE_SOURCE_DIR}/${_f}
    )
endforeach()


#find_package(KF5I18n NO_MODULE)
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
Include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(Phonon4Qt5 REQUIRED)

find_package(X11 REQUIRED)

find_package(KF5 COMPONENTS REQUIRED
  I18n Config Su CoreAddons WidgetsAddons DocTools XmlGui Solid KIO IconThemes
  KDELibs4Support
  )

include_directories(
  #/usr/include/x86_64-linux-gnu/qt5
  #/usr/include/KDE
  /usr/include/KF5
  /usr/include/KF5/Solid
  /usr/include/KF5/KDELibs4Support
  )


INCLUDE(CheckFunctionExists)
INCLUDE(CheckVariableExists)
INCLUDE(CheckTypeSize)
INCLUDE(CMakeDetermineSystem)
INCLUDE(CheckIncludeFiles)
INCLUDE(FindLibMpeg2)

INCLUDE(FindAvcodec)
pkg_search_module(AVCODEC libavcodec REQUIRED)

INCLUDE(FindAvformat)
INCLUDE(FindAvutil)
#FINDAVFORMAT("avformat")
FINDAVCODEC("avcodec")
FINDAVUTIL("avutil")

INCLUDE(FindFFmpegScale)

INCLUDE(FindLibDvdread)
##TODO: PTZ161103 use libdvdread-dev, libdvdnav-dev
#we have dvdnav.pc dvdread.pc
###################################
pkg_search_module(DVDREAD dvdread REQUIRED)
pkg_search_module(DVDNAV dvdnav REQUIRED)


# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )
CHECK_INCLUDE_FILES(inttypes.h      HAVE_INTTYPES_H)                    # inttypes.h
CHECK_INCLUDE_FILES(stddef.h        HAVE_STDDEF_H)                      # stddef.h
CHECK_INCLUDE_FILES(stdint.h        HAVE_STDINT_H)                      # stdint.h
CHECK_INCLUDE_FILES(stdlib.h        HAVE_STDLIB_H)                      # stdlib.h
CHECK_INCLUDE_FILES(string.h        HAVE_STRING_H)                      # string.h
CHECK_INCLUDE_FILES(sys/stat.h      HAVE_SYS_STAT_H)                    # stat.h
CHECK_INCLUDE_FILES(sys/types.h     HAVE_SYS_TYPES_H)                   # types.h
CHECK_INCLUDE_FILES(unistd.h        HAVE_UNISTD_H)                      # unistd.h
CHECK_INCLUDE_FILES(malloc.h        HAVE_MALLOC_H)                      # malloc.h
CHECK_INCLUDE_FILES(time.h          HAVE_TIME_H)


FINDFFMPEGSCALE("swscale")


CONFIGURE_FILE(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)


# do config checks
########################################
# check dependencies
########################################
MESSAGE(STATUS "Checking dependencies" )

#
# XINE
#
INCLUDE(FindXine)
FINDXINE("xine.h" "xine")
#libxine-dev
pkg_search_module(XINE libxine REQUIRED)

FINDLIBMPEG2("mpeg2dec/mpeg2.h" "mpeg2")
FIND_LIBRARY(LIBMPEG2CONVERT_LIBRARY NAMES mpeg2convert PATHS )
IF (LIBMPEG2CONVERT_LIBRARY)
  MESSAGE(STATUS "Found library mpeg2convert: ${LIBMPEG2CONVERT_LIBRARY}")
ELSE (LIBMPEG2CONVERT_LIBRARY)
  MESSAGE(FATAL_ERROR "Could not find mpeg2convert")
ENDIF (LIBMPEG2CONVERT_LIBRARY)

FIND_LIBRARY(LIBDL_LIBRARY NAMES "dl" PATHS )
IF (LIBDL_LIBRARY)
  MESSAGE(STATUS "Found library libdl: ${LIBDL_LIBRARY}")
ELSE (LIBDL_LIBRARY)
  FIND_LIBRARY(LIBDL_LIBRARY NAMES "c" PATHS )
  MESSAGE(STATUS "Found library libc: ${LIBDL_LIBRARY}")
ENDIF (LIBDL_LIBRARY)
#FINDLIBDVDREAD("dvdread/ifo_types.h" "dvdread")


#
#
#
add_definitions( -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

#PTZ141223 no CMakeLists.txt ? how this?
#add_subdirectory( src/libdvdnav-NOW) #from dvdnav
#add_subdirectory( src/libdvdnav-NOW/dvdnav)
#add_subdirectory( src/libdvdnav-NOW/vm)
include_directories(
  ${DVDREAD_INCLUDE_DIR}
  ${DVDNAV_INCLUDE_DIR}
  )

add_subdirectory( src/core )
add_subdirectory( src/mpeg2)
add_subdirectory(src/vamps)
add_subdirectory(src/devices)
add_subdirectory(src/mplayer)
add_subdirectory(src/assistant)
add_subdirectory(src/import)
add_subdirectory(src/batch)
add_subdirectory(icons)
add_subdirectory(data)
add_subdirectory(po)
add_subdirectory(doc)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/devices
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vamps
  ${CMAKE_CURRENT_SOURCE_DIR}/src/backup
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mpeg2
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mplayer
  ${CMAKE_CURRENT_SOURCE_DIR}/src/assistant
  ${CMAKE_CURRENT_SOURCE_DIR}/src/import
  ${CMAKE_CURRENT_SOURCE_DIR}/src/player
  ${CMAKE_CURRENT_SOURCE_DIR}/src/subtitles
  ${CMAKE_CURRENT_SOURCE_DIR}/src/batch
  #${KDE4_INCLUDES}
  ${QT_INCLUDES}

  #/usr/include/newlib ##TODO: PTZ161103 timespec not defined...
  #${LIBDVDREAD_INCLUDE_DIR}/dvdread

  ${AVCODEC_INCLUDEDIR}
  ${AVFORMAT_INCLUDE_DIR}
  ${AVUTIL_INCLUDE_DIR}
  ${FFMPEGSCALE_INCLUDE_DIR}
  ${FFMPEG_INCLUDE_DIR}
  ${XINE_INCLUDEDIR}
  )



set (devices_SRCS
  src/devices/k9cddrive.cpp
  )

set (subtitles_SRCS
  src/subtitles/k9subtitle2pgm.cpp
  )

if(NOT DVDNAV_FOUND)
  set (k9_dvdnav_SRCS
    #   src/dvdnav/decoder.c 
    #   src/dvdnav/dvdnav.c
    #   src/dvdnav/highlight.c
    #   src/dvdnav/navigation.c
    #   src/dvdnav/read_cache.c
    #   src/dvdnav/remap.c
    #   src/dvdnav/searching.c
    #   src/dvdnav/settings.c
    #   src/dvdnav/vm.c
    #   src/dvdnav/vmcmd.c
    #UPDATED TO CURRENT:
    src/libdvdnav-NOW/dvdnav.c
    src/libdvdnav-NOW/highlight.c
    src/libdvdnav-NOW/read_cache.c
    src/libdvdnav-NOW/searching.c
    src/libdvdnav-NOW/navigation.c
    src/libdvdnav-NOW/settings.c

    src/libdvdnav-NOW/vm/decoder.c  
    src/libdvdnav-NOW/vm/getset.c  
    src/libdvdnav-NOW/vm/play.c
    src/libdvdnav-NOW/vm/vm.c  
    src/libdvdnav-NOW/vm/vmcmd.c  
    src/libdvdnav-NOW/vm/vmget.c
    )
endif()
#list(APPEND dvdread_SRCS  src/core/k9dvdread.cpp)
if(NOT DVDREAD_FOUND)
  list(APPEND dvdread_SRCS
    #   src/dvdread/dvd_input.c
    #   src/dvdread/dvd_reader.c
    #   src/dvdread/dvd_udf.c
    #   src/dvdread/ifo_read.c
    #   src/dvdread/md5.c
    #   src/dvdread/nav_read.c
    src/libdvdread-NOW/bitreader.c  
    src/libdvdread-NOW/md5.c
    src/libdvdread-NOW/ifo_print.c        
    src/libdvdread-NOW/nav_print.c

    src/libdvdread-NOW/dvd_input.c
    src/libdvdread-NOW/dvd_reader.c
    src/libdvdread-NOW/dvd_udf.c
    src/libdvdread-NOW/ifo_read.c
    src/libdvdread-NOW/md5.c
    src/libdvdread-NOW/nav_read.c   
    )
endif()

set (vamps_SRCS
  src/vamps/k9fifo.cpp
  src/vamps/k9requant.cpp
  src/vamps/k9vamps.cpp
  )

set (core_SRCS
  src/core/k9dvd.cpp
  src/core/k9dvdtitle.cpp
  src/core/k9dvdtitleset.cpp
  src/core/k9dvdchapter.cpp
  src/core/k9dvdsize.cpp
  src/core/k9process.cpp
  src/core/k9tools.cpp
  src/core/k9dvdread.cpp
  src/core/k9cellcopylist.cpp
  src/core/k9cell.cpp
  src/core/k9dvdprogress.cpp
  src/core/k9ifo2.cpp
  src/core/k9titleencopt.cpp
  src/core/k9config.cpp
  src/core/k9saveimage.cpp
  src/core/k9drawimage.cpp
  src/core/k9progress.cpp
  src/core/k9burnprogress.cpp
  src/core/k9audiocodecs.cpp
  src/core/k9videocodecs.cpp
  src/core/k9burndvd.cpp
  src/core/k9dvdbackupinterface.cpp
  src/core/k9dialogs.cpp
  src/core/k9log.cpp
  src/core/k9logview.cpp
  src/core/k9dvdread.cpp
  )

#qt_wrap_ui(resultingLibraryName HeadersDestName
#  SourcesDestName SourceLists ...)
##TODO: PTZ161111 need QT_UIC_EXECUTABLE
#qt_wrap_ui(k9_5 core_SRCS
qt5_wrap_ui(core_SRCS
  src/core/dvdprogress.ui
  src/core/progress.ui
  src/core/k9logview.ui
  )

set (backup_SRCS
  src/backup/k9dvdbackup.cpp
  src/backup/k9script.cpp
  src/backup/k9backupdlg.cpp
  src/backup/k9dvdauthor.cpp
  src/backup/k9mp4enc.cpp
  src/backup/k9mp4dlg.cpp
  src/backup/k9execcopy.cpp
  src/backup/k9convertaudio.cpp
  src/player/k9play.cpp
  )

qt5_wrap_ui(backup_SRCS
  src/backup/backupdlg.ui
  src/backup/mp4dlg.ui
  )

set (mpeg2_SRCS
  src/mpeg2/k9decodethread.cpp
  src/mpeg2/k9plaympeg2.cpp
  src/mpeg2/kdecmpeg2.cpp
  )

set (mplayer_SRCS
  src/mplayer/k9mplayer.cpp
  src/mplayer/k9xine.cpp
  src/mplayer/k9phonon.cpp
  )
#qt_wrap_ui(k9_5 mplayer_SRCS
qt5_wrap_ui(mplayer_SRCS
  src/mplayer/mplayer.ui
  src/mplayer/phonon.ui
  )

set (import_SRCS
  src/import/k9chapteredit.cpp
  src/import/k9avidecode.cpp
  src/import/k9avifile.cpp
  src/import/k9import.cpp
  src/import/k9lvitemimport.cpp
  src/import/k9menubutton.cpp
  src/import/k9menu.cpp
  src/import/k9menuedit.cpp
  src/import/k9menuscene.cpp
  src/import/k9newdvd.cpp
  src/import/k9newtitle.cpp
  src/import/k9title.cpp
  src/import/k9processlist.cpp
  src/import/k9importfiles.cpp
  )

qt5_wrap_ui(import_SRCS
  src/import/chapterEdit.ui
  src/import/import.ui
  src/import/menuEdit.ui
  src/import/newTitle.ui
  src/import/processList.ui
  )

set (assistant_SRCS
  src/assistant/k9assistant.cpp
  src/assistant/k9astsource.cpp
  src/assistant/k9astdestination.cpp
  src/assistant/k9asttitles.cpp
  src/assistant/k9aststreams.cpp
  src/assistant/k9astmp4.cpp
  src/assistant/k9astmpeg.cpp
  src/assistant/k9astdvdoption.cpp
  )

qt5_wrap_ui(assistant_SRCS
  src/assistant/astsource.ui
  src/assistant/astdestination.ui
  src/assistant/asttitles.ui
  src/assistant/aststreams.ui
  src/assistant/astmp4.ui
  src/assistant/astmpeg.ui
  src/assistant/astdvdoption.ui
  )

set (main_SRCS
  src/main/k9main.cpp
  src/main/k9langselect.cpp
  src/main/k9playbackoptions.cpp
  src/main/k9updatefactor.cpp
  src/main/kviewmpeg2.cpp
  src/main/k9cropselect.cpp
  src/main/k9titlefactor.cpp
  src/main/k9mp4title.cpp
  src/main/k9settings.cpp
  src/main/k9copy.cpp
  src/main/kconfigdlg.cpp
  src/main/k9prefdvd.cpp
  src/main/k9prefmencoder.cpp
  src/main/k9mencodercmdgen.cpp
  src/main/k9ffmpegcmdgen.cpp
  src/main/k9prefmpeg4.cpp
  src/main/k9prefpreview.cpp
  src/main/k9prefauthor.cpp
  src/main/k9prefdirs.cpp
  src/batch/k9batch.cpp
  )

set (player_SRCS
  src/player/main.cpp
  )


qt5_wrap_ui(main_SRCS
  src/main/k9mainw.ui
  src/main/configDlg.ui
  src/main/playbackoptionsw.ui
  src/main/prefDVD.ui
  src/main/prefMPEG4.ui
  src/main/prefMencoder.ui
  src/main/prefAuthor.ui
  src/main/prefpreview.ui
  src/main/langselectw.ui
  src/main/viewmpeg2.ui
  src/main/titlefactor.ui
  src/main/mencoderCmdGen.ui
  src/main/ffmpegCmdGen.ui
  src/main/prefDirs.ui
  )

set (batch_SRCS
  src/batch/main.cpp
  )

set (k9copylib_SRCS
  #   ${libdvdnav_SRCS}
  ${core_SRCS}
  ${vamps_SRCS}
  ${mpeg2_SRCS}
  ${subtitles_SRCS}
  #   src/dvdread/dvd_input.c
  #   src/dvdread/dvd_reader.c
  #   src/dvdread/dvd_udf.c
  #   src/dvdread/ifo_read.c
  #   src/dvdread/md5.c
  #   src/dvdread/nav_read.c
  #   ${dvdread_SRCS}
  ${backup_SRCS}
  )

set(k9copy_SRCS
  ${devices_SRCS}
  ${mplayer_SRCS}
  ${import_SRCS}
  ${main_SRCS}
  ${assistant_SRCS}
  main.cpp
  )

set(k9play_SRCS
  ${player_SRCS}
  )

set (k9xineplayer_SRCS
  src/xine/main.cpp
  src/xine/k9xineplayer.cpp
  )

if(DVDREAD_FOUND OR DVDNAV_FOUND)
  set(DVD_SRC
    ${k9_dvdnav_SRCS}
    ${dvdread_SRCS}
    )
else()
  inlude_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libdvdnav-NOW #from dvdnav
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libdvdread-NOW #from dvdread
    )
endif()

set(DVD_LIBS
  ${DVDNAV_LIBRARIES}
  ${DVDREAD_LIBRARIES}
  ${LIBDL_LIBRARY}
  ${AVCODEC_LIBRARIES}
  )

##TODO: PTZ161105 SHARED
#/usr/src/k9copy-code/k9copy/src/player/main.cpp:173 : référence indéfinie vers « k9Tools::setBatchCopy(bool) »
#/usr/src/k9copy-code/k9copy/src/player/main.cpp:204 : référence indéfinie vers « k9play::k9play(QObject*) » ...
#
#/usr/src/k9copy-code/k9copy/bld$ nm libk9_5.a | c++filt | grep k9play::k9play
#                 U k9play::k9play(QObject*)
#0000000000000864 T k9play::k9play(QObject*)
#0000000000000864 T k9play::k9play(QObject*)
#/usr/src/k9copy-code/k9copy/bld$ nm libk9_5.so | c++filt | grep k9play"0 instead of 1 arguments to message {Extracting titleset ...} supplied before conversion."
#nm libk9_5.so | c++filt | grep k9play::k9play
#000000000008842c t k9play::k9play(QObject*)
#000000000008842c t k9play::k9play(QObject*)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hdr)
add_library(k9_5 STATIC ##TODO: PTZ161105 SHARED
  ${k9copylib_SRCS}
  ${DVD_SRC}
  )
set_target_properties(k9_5
  PROPERTIES
  LINKER_LANGUAGE CXX
  )
target_link_libraries(k9_5
  #  ${DVD_LIBS}
  ${DVDNAV_LIBRARIES}
  ${DVDREAD_LIBRARIES}
  ${LIBDL_LIBRARY}
  ${AVCODEC_LIBRARIES}
  KF5::WidgetsAddons 	#Tier 1 
  KF5::Su 		#Tier 3  
  KF5::I18n 		#Tier 1
  KF5::ConfigCore 	#Tier 1
  KF5::Solid 		#Tier 1
  ${LIBMPEG2_LIBRARY}
  ${LIBMPEG2CONVERT_LIBRARY}
  ${LIBDL_LIBRARY} 
  Qt5::Xml
  ${LIBAVFORMAT_LIBRARIES}
  ${FFMPEGSCALE_LIBRARY}
)

set(DVD_LIBS
    ${AVCODEC_LIBRARY}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libavcodec.so
    ${FFMPEGSCALE_LIBRARY}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libswscale.so
    ${LIBDL_LIBRARY}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libdl.so
    ${LIBMPEG2CONVERT_LIBRARY}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libmpeg2convert.so
    ${LIBMPEG2_LIBRARY} #:FILEPATH=/usr/lib/x86_64-linux-gnu/libmpeg2.so
    ${X11_ICE_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libICE.so
    ${X11_SM_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libSM.so
    ${X11_X11_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libX11.so
    ${X11_XRes_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXRes.so
    ${X11_XTest_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXtst.so
    ${X11_Xau_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXau.so
    ${X11_Xcomposite_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXcomposite.so
    ${X11_Xcursor_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXcursor.so
    ${X11_Xdamage_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXdamage.so
    ${X11_Xdmcp_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXdmcp.so
    ${X11_Xext_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXext.so
    ${X11_Xfixes_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXfixes.so
    ${X11_Xft_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXft.so
    ${X11_Xi_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXi.so
    ${X11_Xinerama_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXinerama.so
    ${X11_Xinput_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXi.so
    ${X11_Xmu_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXmu.so
    ${X11_Xpm_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXpm.so
    ${X11_Xrandr_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXrandr.so
    ${X11_Xrender_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXrender.so
    ${X11_Xscreensaver_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXss.so
    ${X11_Xt_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXt.so
    ${X11_Xxf86vm_LIB}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libXxf86vm.so
    ${XINE_LIBRARY}#:FILEPATH=/usr/lib/x86_64-linux-gnu/libxine.so
  )

#add_dependencies(k9 SCRIPTED_EDITS)

qt5_wrap_ui(k9copy_SRCS)

add_executable(k9copy ${k9copy_SRCS})
add_executable(k9play ${k9play_SRCS})

include_directories(${XINE_INCLUDE_DIR})

add_executable(k9xineplayer ${k9xineplayer_SRCS})


#target_link_libraries(k9copy
#  k9copylib
#  )
####### Static and non Static Library



################################
target_link_libraries(k9copy
  k9_5
  KF5::WidgetsAddons
  KF5::XmlGui
  KF5::IconThemes
  KF5::KIOCore
  KF5::KIOWidgets
  ${PHONON_LIBRARY}
  KF5::I18n
  #${DVD_LIBS}
  )
target_link_libraries(k9play
  #${DVD_LIBS}
  k9_5
  KF5::WidgetsAddons
  KF5::XmlGui
  )
target_link_libraries(k9xineplayer
  KF5::WidgetsAddons
  KF5::XmlGui
  KF5::I18n
  Qt5::X11Extras
  ${XINE_LIBRARIES}
  ${X11_X11_LIB}
  k9_5
  )

#add_library(foo STATIC foo1.c)
# install(TARGETS foo DESTINATION lib EXPORT myproj-targets)
# install(EXPORT myproj-targets DESTINATION lib/myproj)
#export(k9copy)
install(TARGETS k9_5 DESTINATION lib EXPORT k9copy-targets)
install(TARGETS k9copy DESTINATION ${BIN_INSTALL_DIR} )
install(TARGETS k9play DESTINATION ${BIN_INSTALL_DIR} )
install(TARGETS k9xineplayer DESTINATION ${BIN_INSTALL_DIR} )
install(EXPORT k9copy-targets DESTINATION lib)


########### install files ###############
install( FILES k9copy.desktop k9copy_assistant.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )
#install( FILES k9copy.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES k9copyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/k9copy )

install(FILES k9copy_open.desktop DESTINATION ${DATA_INSTALL_DIR}/solid/actions)
install(FILES k9copy_assistant_open.desktop DESTINATION ${DATA_INSTALL_DIR}/solid/actions)

ecm_install_icons( ${ICON_INSTALL_DIR} )



#
# DOCUMENTS
#
# Add "tags" target and make my_project depending on this target.
set_source_files_properties(TAGS PROPERTIES GENERATED true)
add_custom_command (
  OUTPUT TAGS
  COMMAND ctags -e -R --c++-kinds=+p --fields=+iaS --extra=+q ${CMAKE_SOURCE_DIR}
  #COMMAND etags --members --declarations `find . -name *.cc -or -name *.hh -or -name *.cpp -or -name *.h -or -name *.c -or -name *.f`
  #COMMAND ctags -e -R .
  #WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  #COMMAND etags <list of all *.cpp and *.h files used in 'my_project' target>
  #DEPENDS <list of all *.cpp and *h used in 'my_project' target>
  COMMENT "Creates source code tags for Emacs"
  )
#add_dependencies(k9copy TAGS)
#IF (UNIX)
#  add_custom_target(tags etags --members --declarations  `find . -name 
#*.cc -or -name *.hh -or -name *.cpp -or -name *.h -or -name *.c -or 
#-name *.f`)
#  add_custom_target(etags DEPENDS tags)
#ENDIF (UNIX)  

############package###############
set(CPACK_GENERATOR "DEB")

#PTZ161107 done by ldd k9play|awk '/.so/{print $1}'| xargs dpkg -S | sed 's/:.*//g' | sort -u
set(CPACK_DEBIAN_PACKAGE_DEPENDS "
libavcodec57,libavutil55,libbz2-1.0
,libc6,libcairo2,libcrystalhd3
,libdbus-1-3,libdrm2,libdvdnav4,libdvdread4
,libexpat1,libfam0,libfdk-aac1,libffi6
,libfontconfig1,libfreetype6
,libgcc1,libgcrypt20
,libgl1-mesa-glx,libglib2.0-0
,libgomp1,libgpg-error0
,libgraphite2-3,libgsm1
,libharfbuzz0b,libicu57,libilbc2
,libkf5archive5,libkf5attica5,libkf5auth5,libkf5codecs5
,libkf5completion5,libkf5configcore5,libkf5configgui5
,libkf5configwidgets5,libkf5coreaddons5,libkf5dbusaddons5
,libkf5globalaccel5,libkf5guiaddons5
,libkf5i18n5,libkf5iconthemes5,libkf5itemviews5
,libkf5pty5,libkf5service5,libkf5solid5
,libkf5sonnetcore5,libkf5sonnetui5,libkf5su5
,libkf5textwidgets5,libkf5widgetsaddons5,libkf5windowsystem5
,libkf5xmlgui5,libkvazaar3,liblzma5
,libmfx0,libmp3lame0,libmpeg2-4,libnuma1
,libogg0,libopenh264-2,libopenjp2-7
,libopus0,liborc-0.4-0,libpcre16-3,libpcre3
,libpixman-1-0,libpng16-16,libproxy1v5,libqt5core5a,libqt5dbus5,libqt5gui5,libqt5network5
,libqt5printsupport5,libqt5svg5,libqt5widgets5,libqt5x11extras5,libqt5xml5
,libschroedinger-1.0-0,libselinux1,libshine3,libsnappy1v5,libsoxr0,libspeex1
,libstdc++6,libswresample2
,libsystemd0,libtheora0,libudev1
,libva1,libva-drm1,libva-x11-1,libvdpau1,libvo-amrwbenc0,libvorbis0a,libvorbisenc2,libvpx4,libx11-6
,libx264-148,libx265-95,libxau6
,libxcb1,libxcb-keysyms1,libxcb-render0,libxcb-shm0
,libxdmcp6,libxext6,libxfixes3,libxrender1,libxvidcore4,libzvbi0,pristine-tar,zlib1g"
  )

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS true)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Disk Video DVD9 to DVD5 backup tool")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "yet another full rounded package to helping shrinking double layers onto single layers DVD;
 with players, en/codec, authoring hooks inside.
 Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
 Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
 Copyright: See COPYING file that comes with this distribution
")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER ptizoom@gmail.com)
set(CPACK_PACKAGE_CONTACT "Jean-Michel PETIT <k9copy@free.fr>, Pauline123 <pauline123@users.sf.net>, ptizoom@gmail.com")

#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst")
#// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
#// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
#// Copyright: See COPYING file that comes with this distribution


SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "8-${git_tag}")
SET(CPACK_PACKAGE_NAME "k9copy")
SET(CPACK_SOURCE_IGNORE_FILES
  "moc_"
  "\\\\.a"
  "\\\\.moc"
  "\\\\.gmo"
  "\\\\.ts"
  "k9copy$"
  "k9play$"
  "k9xineplayer$"
  "ui_"
  "\\\\.gz"
  "\\\\.tar.Z"
  "\\\\.tar.bz2"
  "\\\\.tar.xz"
  "\\\\~$"
  "\\\\kdevelop"
  "\\\\automoc"
  "Makefile$"
  "TODO"
  "CMakeCache.txt"
  "install_manifest.txt"
  "cmake_install.cmake"
  "cmake_"
  "CTest"
  "CPack"
  "CMakeLists.txt.user"
  "/CVS/"
  "/CMakeTmp/"
  "/CMakeFiles/"
  "/_CPack_Packages/"
  "/templates/"
  "/\\\\.svn/"
  "\\\\.swp$"
  "\\\\.#"
  "/#"
  "build"
  )
#SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};K9Copy;ALL;/" )

INCLUDE(CPack)

