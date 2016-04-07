# Modified wxWidgets environment setter to allow both 3.0 and 2.8
# versions to build
WX_CFLAGS  = `wx-config --unicode=yes --version=3.0 --cxxflags 2>/dev/null || wx-config --unicode=yes --version=2.8 --cxxflags 2>/dev/null`
WX_LDFLAGS = `wx-config --unicode=yes --version=3.0 --libs richtext,aui,xrc,qa,html,adv,core,xml,net 2>/dev/null || wx-config --unicode=yes --version=2.8 --libs richtext,aui,xrc,qa,html,adv,core,xml,net 2>/dev/null`
