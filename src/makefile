# If the tool is built out of the kit, PIN_ROOT must be specified in the make invocation and point to the kit root.

# For precise measurement.

PIN_ROOT = ../pin-3.2-81205-gcc-linux
CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
include $(CONFIG_ROOT)/makefile.config
include makefile.rules
include $(TOOLS_ROOT)/Config/makefile.default.rules

noinst:
	make "CXX=g++ -DNO_INST"

noargs:
	make "CXX=g++ -DNO_ARGS"
