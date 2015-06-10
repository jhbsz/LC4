include ./config

ifndef PRJ_PATH
  PRJ_PATH=$(shell pwd)
endif
export PRJ_PATH

include ./make/config.mk
include ./make/tools.mk
include ./make/$(OS)_opt.mk

SUB_DIR=$(patsubst %/, %, $(dir $(wildcard src/*/Makefile)))
SUB_LIB=$(subst src/, , $(SUB_DIR))

shell:uslib shell_o
	$(CP) $(BLD_DIR)/SHELL/$(SHELLOBJ) $(BIN_DIR)/$(SHELLOBJ)
	$(STRIP) $(BIN_DIR)/$(SHELLOBJ)

uslib:uslib_o
	$(AR) -r $(BIN_DIR)/$(US_MOD)_$(RUNMODE).a $(wildcard $(BLD_DIR)/USLIB/*.o)

shell_o:
	$(foreach i, $(SUB_LIB), $(MAKE) MODULE_TYPE=SHELL -C src/$i all || exit 1;)

uslib_o:
	$(foreach i, $(SUB_LIB), $(MAKE) MODULE_TYPE=USLIB -C src/$i all || exit 1;)


$(BIN_DIR):
	$(MKDIR) -p $@

config2h:config
	$(PERL) $(PRJ_PATH)/board/config2h.pl $(PRJ_PATH)

release:
	@cd make; ./release.sh $(VER)

clean:
	$(RM) -f $(BLD_DIR)/KSLIB/*
	$(RM) -f $(BLD_DIR)/USLIB/*
	$(RM) -f $(BLD_DIR)/SHELL/*
	$(RM) -f $(BIN_DIR)/*
