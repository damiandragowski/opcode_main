TARGET    = opcode_ddr

CFLAGS   := -Wall -pedantic -Wextra
CXXFLAGS := -Wall -pedantic
LDFLAGS  := -m32 -lportal -lpcmcpp60 -lcmpin -lcm_main -lpsiu -lboost_regex
APP_OBJS := 

DEPS     := 

deps:	$(TARGET)
	@$(foreach DEP, $(DEPS), cd $(shell dirname $(DEP)) && $(MAKE) dist && cd -)
