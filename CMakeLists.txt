# Portions of this come from KiCad's CMakeLists.txt
cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "CppMakeRulesOverwrite.cmake")

# set the project name
project(wxWabbitemu VERSION 0.1.0)

# enforce c11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find wx + include it
# Turn on wxWidgets compatibility mode for some classes
add_definitions( -DWX_COMPATIBILITY )

# Don't link statically
set( wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --static=no )

# now hunt for wxWidgets
# (adv is used for "advanced" widgets, such as the wizard)
find_package(wxWidgets REQUIRED adv net core base)
include(${wxWidgets_USE_FILE})

# mixing of C and C++ necessitates builds in c++
file(GLOB_RECURSE CFILES "*.c")
SET_SOURCE_FILES_PROPERTIES(${CFILES} PROPERTIES LANGUAGE CXX )

# win2posix
add_subdirectory(win2posix)

# add the executable
add_executable(wxWabbitemu
	#[[ Core ]]
	wabbitemu/core/core.c
	wabbitemu/core/alu.c
	wabbitemu/core/alu_reverse.c
	wabbitemu/core/control.c
	wabbitemu/core/control_reverse.c
	wabbitemu/core/device.c
	wabbitemu/core/indexcb.c
	wabbitemu/core/indexcb_reverse.c
	wabbitemu/core/reverse_info.c
	
	#[[ Calc Interfacing ]]
	wabbitemu/interface/calc.c
	wabbitemu/interface/state.c
	
	#[[ Utilities/Libraries ]]
	wabbitemu/utilities/exportvar.c
	wabbitemu/utilities/gif.cpp
	wabbitemu/utilities/label.c
	wabbitemu/utilities/linksendvar.c
	wabbitemu/utilities/sendfile.c
	wabbitemu/utilities/var.c
	
	#[[ Utilities/Libraries specific to wx]]
	utilities/fileutilities.c
	utilities/savestate.cpp
	
	#[[ Hardware Emulation ]]
	wabbitemu/hardware/81hw.c
	wabbitemu/hardware/83hw.c
	wabbitemu/hardware/83phw.c
	wabbitemu/hardware/83psehw.c
	wabbitemu/hardware/86hw.c
	wabbitemu/hardware/colorlcd.c
	wabbitemu/hardware/keys.c
	wabbitemu/hardware/link.c
	
	#[[ Hardware Emulation specific to wx ]]
	hardware/lcd.c
	
	#[[ wxWidgets GUI ]]
	gui/gui.cpp                   #[[ This requires gui/gui.h ]]
	gui/guiapp.cpp                #[[ This requires gui/guiapp.h ]]
	gui/guiskinwindow.cpp         #[[ This requires gui/guiskinwindow.h ]]
	gui/guilcd.cpp                #[[ This requires gui/guilcd.h ]]
	gui/guivartree.cpp            #[[ This requires gui/guivartree.h ]]
	gui/droptarget.cpp            #[[ This requires gui/droptarget.h ]]
	gui/wizard/wizardstart.cpp    #[[ This requires gui/wizard/wizardstart.h ]]
	gui/wizard/wizardcalctype.cpp #[[ This requires gui/wizard/wizardcalctype.h ]]
	gui/wizard/wizardos.cpp       #[[ This requires gui/wizard/wizardos.h ]]
	gui/wizard/romwizard.cpp      #[[ This requires gui/wizard/romwizard.h ]]
	debugger/guidebug.cpp         #[[ This requires debugger/guidebug.h ]]
	
	#[[ Debugger and debugger GUI ]]
	debugger/disassemblyview.cpp  #[[ This requires debugger/disassemblyview.h ]]
	debugger/disassemble.c
	debugger/panes/regpane.cpp    #[[ This requires debugger/panes/regpane.h ]]
	debugger/panes/flagspane.cpp  #[[ This requires debugger/panes/flagspane.h ]]
	debugger/panes/cpupane.cpp    #[[ This requires debugger/panes/cpupane.h ]]
)

target_include_directories(wxWabbitemu PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/wabbitemu"
                           "${PROJECT_SOURCE_DIR}/wabbitemu/core"
                           "${PROJECT_SOURCE_DIR}/debugger"
                           "${PROJECT_SOURCE_DIR}/wabbitemu/debugger"
                           "${PROJECT_SOURCE_DIR}/gui"
                           "${PROJECT_SOURCE_DIR}/wabbitemu/hardware"
                           "${PROJECT_SOURCE_DIR}/wabbitemu/interface"
                           "${PROJECT_SOURCE_DIR}/utilities"
                           "${PROJECT_SOURCE_DIR}/wabbitemu/utilities"
                           )

target_link_libraries(wxWabbitemu ${wxWidgets_LIBRARIES} win2posix)

add_definitions(-DHIGH_SHADE_GIF -DVERBOSE -D_LINUX -DWXVER -D_UNICODE -DCORETYPES_H_)

# CPack for packaging
if( UNIX AND NOT APPLE )

    # Create a *.deb file:
    set( CPACK_GENERATOR "DEB" )
    set( CPACK_DEBIAN_PACKAGE_MAINTAINER "https://github.com/alberthdev/wxwabbitemu" )

    set( CPACK_PACKAGE_VERSION_MAJOR 0 )
    set( CPACK_PACKAGE_VERSION_MINOR 1 )
    set( CPACK_PACKAGE_VERSION_PATCH 0 )
    set( CPACK_PACKAGE_CONTACT Albert Huang <alberthdev@users.noreply.github.com> )
    set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "wxWabbitemu built by CMake build system." )

    include( CPack )

endif()

install(TARGETS wxWabbitemu DESTINATION bin)
