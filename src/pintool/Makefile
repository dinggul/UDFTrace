all: setting udftrace

setting: mkfile_pin mkfile_rule_pin
	mkdir -p ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace
	cp mkfile_pin ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace/makefile
	cp mkfile_rule_pin ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace/makefile.rules

udftrace: udftrace.cpp
	cp $< ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace/UDFTrace.cpp
	cd ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace/ && $(MAKE)
	cp ../../pin-3.2-81205-gcc-linux/source/tools/UDFTrace/obj-intel64/UDFTrace.so ./

.PHONY: all setting clean
