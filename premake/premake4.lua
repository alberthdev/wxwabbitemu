dofile "use_wxwidgets.lua"
	solution "wxWabbitemu"
		configurations {"Debug", "Release"}
		location "../build" -- optional, but cleaner
		targetdir "../bin"  -- optional, same here
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols", "ExtraWarnings" }
		configuration "Release"
			flags { "Optimize" }
		
		configuration { "linux", "gmake" }
			buildoptions { "-x c++" }
		
		project "wxWabbitemu"
			kind "WindowedApp"
			language "C++"
			wx_config {Unicode="yes", Version="2.8"}
			includedirs {
				"..",
				"../core",
				"../debugger",
				"../gui",
				"../hardware",
				"../interface",
				"../utilities"
			}
			defines {
				"HIGH_SHADE_GIF",
				"VERBOSE",
				"_LINUX",
				"WXVER"
			}
			files {
				-- Core
				"../core/core.c",
				"../core/alu.c",
				"../core/control.c",
				"../core/device.c",
				"../core/indexcb.c",
				-- Calc Interfacing
				"../interface/calc.c",
				"../interface/state.c",
				-- Utilities/Libraries
				"../utilities/exportvar.c",
				"../utilities/fileutilities.c",
				"../utilities/gif.c",
				"../utilities/gifhandle.c",
				"../utilities/label.c",
				"../utilities/savestate.c",
				"../utilities/sendfile.c",
				"../utilities/var.c",
				-- Hardware Emulation
				"../hardware/81hw.c",
				"../hardware/83hw.c",
				"../hardware/83phw.c",
				"../hardware/83psehw.c",
				"../hardware/86hw.c",
				"../hardware/keys.c",
				"../hardware/lcd.c",
				"../hardware/link.c",
				-- wxWidgets GUI
				"../gui/gui.cpp", -- This requires ../gui/gui.h
				"../gui/guiapp.cpp", -- This requires ../gui/guiapp.h
				"../gui/guiskinwindow.cpp", -- This requires ../gui/guiskinwindow.h
				"../gui/guilcd.cpp", -- This requires ../gui/guilcd.h
				"../gui/guivartree.cpp", -- This requires ../gui/guivartree.h
				"../gui/droptarget.cpp", -- This requires ../gui/droptarget.h
				"../gui/wizard/wizardstart.cpp", -- This requires ../gui/wizard/wizardstart.h
				"../gui/wizard/wizardcalctype.cpp", -- This requires ../gui/wizard/wizardcalctype.h
				"../gui/wizard/wizardos.cpp", -- This requires ../gui/wizard/wizardos.h
				"../gui/wizard/romwizard.cpp", -- This requires ../gui/wizard/romwizard.h
				"../debugger/guidebug.cpp", -- This requires ../debugger/guidebug.h
				-- Debugger and debugger GUI
				"../debugger/disassemblyview.cpp", -- This requires ../debugger/disassemblyview.h
				"../debugger/disassemble.c",
				"../debugger/panes/regpane.cpp", -- This requires ../debugger/panes/regpane.h
				"../debugger/panes/flagspane.cpp", -- This requires ../debugger/panes/flagspane.h
				"../debugger/panes/cpupane.cpp" -- This requires ../debugger/panes/cpupane.h
			}

newaction {
	trigger		= "install",
	description	= "Install wxWabbitemu",
	execute		= function ()
				os.copyfile("bin/wxwabbitemu", "/usr/bin/wxwabbitemu")
			end
}
