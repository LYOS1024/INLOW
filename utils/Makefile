ARCH ?= i686

REPO_ROOT = ..
include $(REPO_ROOT)/build-config/config.mk
include $(REPO_ROOT)/build-config/version.mk

BUILD = $(BUILD_DIR)/utils

CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -Wall -Wextra
CPPFLAGS += -D_INLOW_SOURCE -DINLOW_VERSION=\"$(VERSION)\"

PROGRAMS = \
		   cat \
		   clear \
		   echo \
		   false \
		   ls \
		   sh \
		   sleep \
		   snake \
		   true \
		   uname \
		   version \
		   miv

all: $(addprefix $(BUILD)/, $(PROGRAMS))

install: $(addprefix $(BUILD)/, $(PROGRAMS))
	@mkdir -p $(BIN_DIR)
	cp -f $^ $(BIN_DIR)
	touch $(SYSROOT)
	
$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all install clean
