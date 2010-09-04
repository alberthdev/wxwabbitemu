.PHONY: clean All

All:
	@echo ----------Building project:[ wxWabbitemu - Debug ]----------
	@"$(MAKE)" -f "wxWabbitemu.mk"
clean:
	@echo ----------Cleaning project:[ wxWabbitemu - Debug ]----------
	@"$(MAKE)" -f "wxWabbitemu.mk" clean
