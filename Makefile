# Quick reference:
# @ silences the line
# - ignores errors on the line

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

.PHONY: all help install uninstall distclean distclean-real

# Wildcard rule
.DEFAULT:
	@$(MAKE) -s -C premake
	$(MAKE) -C build $@

all: bin/wxWabbitemu

bin/wxWabbitemu:
	@$(MAKE) wxWabbitemu

help:
	@$(MAKE) -s -C build help
	@echo ""
	@echo "ADDITIONAL TARGETS:"
	@echo "   install"
	@echo "   uninstall"
	@echo "   distclean"

install: bin/wxWabbitemu
	@echo "Installing wxWabbitemu to: $(BINDIR)"
	install bin/wxWabbitemu $(BINDIR)

uninstall:
	@echo "Uninstalling wxWabbitemu from: $(BINDIR)"
	rm -f $(BINDIR)/wxWabbitemu

clean:
	$(MAKE) -C build clean
	-if [ -d build/obj ];then rmdir build/obj; fi
	-if [ -d bin ]; then rmdir bin; fi

distclean:
	@echo "WARNING: Running distclean will require you to regenerate"
	@echo "your makefiles with Premake. Only do this if you suspect"
	@echo "that the Makefiles are corrupt."
	@echo ""
	@echo "Run make distclean-real to actually distclean."

distclean-real: clean
	$(MAKE) -C premake distclean
