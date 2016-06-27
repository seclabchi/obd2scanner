.PHONY: clean All

All:
	@echo "----------Building project:[ OBD2Scanner - Debug ]----------"
	@$(MAKE) -f  "OBD2Scanner.mk"
clean:
	@echo "----------Cleaning project:[ OBD2Scanner - Debug ]----------"
	@$(MAKE) -f  "OBD2Scanner.mk" clean
