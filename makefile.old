.SUFFIXES:
.SUFFIXES: .o .c

WXVERSION=2.8
CFLAGS=-I. -I./core -I./debugger -I./gui -I./hardware -I./interface -I./utilities -x c++ -ggdb -c\
		-DHIGH_SHADE_GIF -DVERBOSE -D_LINUX -DWXVER `wx-config --version=$(WXVERSION) --unicode=yes --cflags` -Wall
CXXFLAGS=-I. -I./core -I./debugger -I./gui -I./hardware -I./interface -I./utilities -x c++ -ggdb -c\
		-DHIGH_SHADE_GIF -DVERBOSE -D_LINUX -DWXVER `wx-config --version=$(WXVERSION) --unicode=yes --cppflags` -Wall
LDFLAGS=`wx-config --version=$(WXVERSION) --unicode=yes --libs` -lstdc++
CC=gcc
CXX=g++
LD=g++
PROGRAM=wxwabbitemu

.SUFFIXES: .o .cpp .c
.c.o:
	$(CC) -c $(CFLAGS) $*.c
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $*.c

all: directory wabbitemu

directory:
	mkdir -p ./obj

# Apparently some versions of GCC (like 4.7.020120324) don't like the order
# $(LD) $(LDFLAGS) *.o -o $(PROGRAM), and will want to commit suicide instead.
# This fixes this issue.
wabbitemu: core.o alu.o control.o device.o indexcb.o calc.o state.o gif.o\
			gifhandle.o label.o savestate.o sendfile.o var.o 81hw.o 83hw.o\
			83phw.o 83psehw.o 86hw.o keys.o lcd.o link.o gui.o guilcd.o\
			droptarget.o guidebug.o disassemblyview.o disassemble.o romwizard.o\
			wizardstart.o wizardcalctype.o wizardos.o fileutilities.o exportvar.o\
			regpane.o flagspane.o guivartree.o cpupane.o guiskinwindow.o guiapp.o
	$(LD) *.o $(LDFLAGS) -o $(PROGRAM)

core.o: ./core/core.c
	$(CXX) $(CXXFLAGS) ./core/core.c

alu.o: ./core/alu.c
	$(CXX) $(CXXFLAGS) ./core/alu.c

device.o: ./core/device.c
	$(CXX) $(CXXFLAGS) ./core/device.c

control.o: ./core/control.c
	$(CXX) $(CXXFLAGS) ./core/control.c
	
indexcb.o: ./core/indexcb.c
	$(CXX) $(CXXFLAGS) ./core/indexcb.c

calc.o: ./interface/calc.c
	$(CXX) $(CXXFLAGS) ./interface/calc.c

state.o: ./interface/state.c
	$(CXX) $(CXXFLAGS) ./interface/state.c

exportvar.o: ./utilities/exportvar.c
	$(CXX) $(CXXFLAGS) ./utilities/exportvar.c

fileutilities.o: ./utilities/fileutilities.c
	$(CXX) $(CXXFLAGS) ./utilities/fileutilities.c
	
gif.o: ./utilities/gif.c
	$(CXX) $(CXXFLAGS) ./utilities/gif.c

gifhandle.o: ./utilities/gifhandle.c
	$(CXX) $(CXXFLAGS) ./utilities/gifhandle.c
	
label.o: ./utilities/label.c
	$(CXX) $(CXXFLAGS) ./utilities/label.c

savestate.o: ./utilities/savestate.c
	$(CXX) $(CXXFLAGS) ./utilities/savestate.c

var.o: ./utilities/var.c
	$(CXX) $(CXXFLAGS) ./utilities/var.c

sendfile.o: ./utilities/sendfile.c
	$(CXX) $(CXXFLAGS) ./utilities/sendfile.c

81hw.o: ./hardware/81hw.c
	$(CXX) $(CXXFLAGS) ./hardware/81hw.c
	
83hw.o: ./hardware/83hw.c
	$(CXX) $(CXXFLAGS) ./hardware/83hw.c

83phw.o: ./hardware/83phw.c
	$(CXX) $(CXXFLAGS) ./hardware/83phw.c
	
83psehw.o: ./hardware/83psehw.c
	$(CXX) $(CXXFLAGS) ./hardware/83psehw.c

86hw.o: ./hardware/86hw.c
	$(CXX) $(CXXFLAGS) ./hardware/86hw.c

keys.o: ./hardware/keys.c
	$(CXX) $(CXXFLAGS) ./hardware/keys.c

lcd.o: ./hardware/lcd.c
	$(CXX) $(CXXFLAGS) ./hardware/lcd.c

link.o: ./hardware/link.c
	$(CXX) $(CXXFLAGS) ./hardware/link.c
	
gui.o: ./gui/gui.cpp ./gui/gui.h
	$(CXX) $(CXXFLAGS) ./gui/gui.cpp

guiapp.o: ./gui/guiapp.cpp ./gui/guiapp.h
	$(CXX) $(CXXFLAGS) ./gui/guiapp.cpp
	
guiskinwindow.o: ./gui/guiskinwindow.cpp ./gui/guiskinwindow.h
	$(CXX) $(CXXFLAGS) ./gui/guiskinwindow.cpp

guilcd.o: ./gui/guilcd.cpp ./gui/guilcd.h
	$(CXX) $(CXXFLAGS) ./gui/guilcd.cpp

guivartree.o: ./gui/guivartree.cpp ./gui/guivartree.h
	$(CXX) $(CXXFLAGS) ./gui/guivartree.cpp
	
droptarget.o: ./gui/droptarget.cpp ./gui/droptarget.h
	$(CXX) $(CXXFLAGS) ./gui/droptarget.cpp
	
wizardstart.o: ./gui/wizard/wizardstart.cpp ./gui/wizard/wizardstart.h
	$(CXX) $(CXXFLAGS) ./gui/wizard/wizardstart.cpp

wizardcalctype.o: ./gui/wizard/wizardcalctype.cpp ./gui/wizard/wizardcalctype.h
	$(CXX) $(CXXFLAGS) ./gui/wizard/wizardcalctype.cpp
	
wizardos.o: ./gui/wizard/wizardos.cpp ./gui/wizard/wizardos.h
	$(CXX) $(CXXFLAGS) ./gui/wizard/wizardos.cpp
	
romwizard.o: ./gui/wizard/romwizard.cpp ./gui/wizard/romwizard.h
	$(CXX) $(CXXFLAGS) ./gui/wizard/romwizard.cpp
	
guidebug.o: ./debugger/guidebug.cpp ./debugger/guidebug.h
	$(CXX) $(CXXFLAGS) ./debugger/guidebug.cpp
	
disassemblyview.o: ./debugger/disassemblyview.cpp ./debugger/disassemblyview.h
	$(CXX) $(CXXFLAGS) ./debugger/disassemblyview.cpp

disassemble.o: ./debugger/disassemble.c
	$(CXX) $(CXXFLAGS) ./debugger/disassemble.c
	
regpane.o: ./debugger/panes/regpane.cpp ./debugger/panes/regpane.h
	$(CXX) $(CXXFLAGS) ./debugger/panes/regpane.cpp

flagspane.o: ./debugger/panes/flagspane.cpp ./debugger/panes/flagspane.h
	$(CXX) $(CXXFLAGS) ./debugger/panes/flagspane.cpp
	
cpupane.o: ./debugger/panes/cpupane.cpp ./debugger/panes/cpupane.h
	$(CXX) $(CXXFLAGS) ./debugger/panes/cpupane.cpp

clean :
	rm -f *.o $(PROGRAM)
