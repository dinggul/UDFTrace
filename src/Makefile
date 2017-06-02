PROJECT_DIR = ../pin-3.2-81205-gcc-linux/source/tools/UDFTrace
all: setting udftrace

setting: mkfile_pin mkfile_rule_pin
	mkdir -p $(PROJECT_DIR)
	cp mkfile_pin $(PROJECT_DIR)/makefile
	cp mkfile_rule_pin $(PROJECT_DIR)/makefile.rules

udftrace: udftrace.cpp
	cp $< $(PROJECT_DIR)/UDFTrace.cpp
	cd $(PROJECT_DIR) && $(MAKE)
	cp $(PROJECT_DIR)/obj-intel64/UDFTrace.so ./

clean:
	rm -rf UDFTrace.so

.PHONY: all setting clean
