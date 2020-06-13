# Portions of this come from KiCad's CMakeLists.txt
# set the project name
project('wxWabbitemu', ['c', 'cpp'],
	version: '0.1.0',
	license: 'GPL2+',
	default_options : ['default_library=static', 'cpp_std=c++11'])

# find wx + include it
# Turn on wxWidgets compatibility mode for some classes
add_global_arguments('-DWX_COMPATIBILITY', language: ['c', 'cpp'])

# wxWabbitemu specific defines
add_global_arguments('-DHIGH_SHADE_GIF', '-DVERBOSE', '-D_LINUX', '-DWXVER', '-D_UNICODE', '-DCORETYPES_H_', language: ['c', 'cpp'])

# Enable Unicode, as needed
compiler = meson.get_compiler('cpp')

# now hunt for wxWidgets
# (adv is used for "advanced" widgets, such as the wizard)
wxWidgets_dep = dependency('wxWidgets', required: true, modules: ['adv', 'net', 'core', 'base'], static: false)

# win2posix
subdir('win2posix')

# includes
includes = include_directories(
	'wabbitemu',
	'wabbitemu/core',
	'debugger',
	'wabbitemu/debugger',
	'gui',
	'wabbitemu/hardware',
	'wabbitemu/interface',
	'utilities',
	'wabbitemu/utilities'
)

# add the executable
wxWabbitemu_exe = executable('wxWabbitemu',
	# Core
	'wabbitemu/core/core.c',
	'wabbitemu/core/alu.c',
	'wabbitemu/core/alu_reverse.c',
	'wabbitemu/core/control.c',
	'wabbitemu/core/control_reverse.c',
	'wabbitemu/core/device.c',
	'wabbitemu/core/indexcb.c',
	'wabbitemu/core/indexcb_reverse.c',
	'wabbitemu/core/reverse_info.c',
	
	# Calc Interfacing
	'wabbitemu/interface/calc.c',
	'wabbitemu/interface/state.c',
	
	# Utilities/Libraries
	'wabbitemu/utilities/exportvar.c',
	'wabbitemu/utilities/gif.cpp',
	'wabbitemu/utilities/label.c',
	'wabbitemu/utilities/linksendvar.c',
	'wabbitemu/utilities/sendfile.c', 
	'wabbitemu/utilities/var.c',
	
	# Utilities/Libraries specific to wx
	'utilities/fileutilities.c',
	'utilities/savestate.cpp',
	
	# Hardware Emulation
	'wabbitemu/hardware/81hw.c',
	'wabbitemu/hardware/83hw.c',
	'wabbitemu/hardware/83phw.c',
	'wabbitemu/hardware/83psehw.c',
	'wabbitemu/hardware/86hw.c',
	'wabbitemu/hardware/colorlcd.c',
	'wabbitemu/hardware/keys.c',
	'wabbitemu/hardware/link.c',
	
	# Hardware Emulation specific to wx
	'hardware/lcd.c',
	
	# wxWidgets GUI
	'gui/gui.cpp',                   # This requires ../gui/gui.h
	'gui/guiapp.cpp',                # This requires ../gui/guiapp.h
	'gui/guiskinwindow.cpp',         # This requires ../gui/guiskinwindow.h
	'gui/guilcd.cpp',                # This requires ../gui/guilcd.h
	'gui/guivartree.cpp',            # This requires ../gui/guivartree.h
	'gui/droptarget.cpp',            # This requires ../gui/droptarget.h
	'gui/wizard/wizardstart.cpp',    # This requires ../gui/wizard/wizardstart.h
	'gui/wizard/wizardcalctype.cpp', # This requires ../gui/wizard/wizardcalctype.h
	'gui/wizard/wizardos.cpp',       # This requires ../gui/wizard/wizardos.h
	'gui/wizard/romwizard.cpp',      # This requires ../gui/wizard/romwizard.h
	'debugger/guidebug.cpp',         # This requires ../debugger/guidebug.h
	
	# Debugger and debugger GUI
	'debugger/disassemblyview.cpp',  # This requires ../debugger/disassemblyview.h
	'debugger/disassemble.c',
	'debugger/panes/regpane.cpp',    # This requires ../debugger/panes/regpane.h
	'debugger/panes/flagspane.cpp',  # This requires ../debugger/panes/flagspane.h
	'debugger/panes/cpupane.cpp',    # This requires ../debugger/panes/cpupane.h
	
	dependencies: [wxWidgets_dep, win2posix_dep],
	
	# mixing of C and C++ necessitates builds in c++
	c_args: ['-x', 'c++', '-std=c++11'],
	
	include_directories: includes,
	install: true
)

