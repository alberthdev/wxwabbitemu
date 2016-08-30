# Quick reference:
# @ silences the line
# - ignores errors on the line

.PHONY: all install uninstall distclean distclean-real

# Wildcard rule
.DEFAULT:
	@$(MAKE) -s -C premake
	$(MAKE) -C build $@

all: bin/wxWabbitemu

bin/wxWabbitemu:
	@$(MAKE) wxWabbitemu

install: bin/wxWabbitemu
	install bin/wxWabbitemu /usr/bin

uninstall:
	rm -f /usr/bin/wxWabbitemu

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
