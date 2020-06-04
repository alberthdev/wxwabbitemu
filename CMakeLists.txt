# Portions of this come from KiCad's CMakeLists.txt

cmake_minimum_required(VERSION 3.0.2)

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
file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/*.c")
SET_SOURCE_FILES_PROPERTIES(${CFILES} PROPERTIES LANGUAGE CXX )

# add the executable
add_executable(wxWabbitemu
	#[[ Core ]]
	core/core.c
	core/alu.c
	core/alu_reverse.c
	core/control.c
	core/control_reverse.c
	core/device.c
	core/indexcb.c
	core/indexcb_reverse.c
	core/reverse_info.c
	
	#[[ Calc Interfacing ]]
	interface/calc.c
	interface/state.c
	
	#[[ Utilities/Libraries ]]
	utilities/exportvar.c
	utilities/fileutilities.c
	utilities/gif.cpp
	utilities/label.c
	utilities/savestate.cpp
	utilities/sendfile.c
	utilities/var.c
	
	#[[ Hardware Emulation ]]
	hardware/81hw.c
	hardware/83hw.c
	hardware/83phw.c
	hardware/83psehw.c
	hardware/86hw.c
	hardware/colorlcd.c
	hardware/keys.c
	hardware/lcd.c
	hardware/link.c
	
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
                           "${PROJECT_SOURCE_DIR}/core"
                           "${PROJECT_SOURCE_DIR}/debugger"
                           "${PROJECT_SOURCE_DIR}/gui"
                           "${PROJECT_SOURCE_DIR}/hardware"
                           "${PROJECT_SOURCE_DIR}/interface"
                           "${PROJECT_SOURCE_DIR}/utilities"
                           )

target_link_libraries(wxWabbitemu ${wxWidgets_LIBRARIES})

add_definitions(-DHIGH_SHADE_GIF -DVERBOSE -D_LINUX -DWXVER -D_UNICODE)

include(CheckTypeSize)
check_type_size(wchar_t SIZEOF_WCHAR_T)

if(NOT ${SIZEOF_WCHAR_T} EQUAL "")
	set(HAVE_WCHAR_T 1)
endif()

include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(mbstowcs_s HAVE_MBSTOWCS_S)
CHECK_FUNCTION_EXISTS(wcsncpy_s HAVE_WCSNCPY_S)
CHECK_FUNCTION_EXISTS(strcpy_s HAVE_STRCPY_S)

configure_file ("${PROJECT_SOURCE_DIR}/cmake.config.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )

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
