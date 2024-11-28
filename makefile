# project directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/tetris.out


# compiler collection
CC = gcc
CPP = $(CC)
LD = $(CC)


# Verbose output
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
	Q = @
endif


# Build configurations
CONFIG ?= DEBUG
ifeq ($(CONFIG), DEBUG)
    CFLAGS += -g -O #-save-temps=obj0
else ifeq ($(CONFIG), RELEASE)
    CFLAGS += -O2
else
    $(error Unsupported CONFIG value: $(CONFIG))
endif


# compiler flags
CPPFLAGS = -MMD $(addprefix -iquote,$(shell find $(INCLUDE_DIR) -type d))
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic 
LDFLAGS = -lncurses #-v


# source/object files
CFILES = $(shell find $(SRC_DIR) -type f -name '*.c')
CFILES := $(CFILES:$(SRC_DIR)/%=%)
OFILES = $(CFILES:%.c=$(BUILD_DIR)/%.o)


# targets
.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(OFILES) 
	@printf "LD\t$@\n"
	$(Q)$(LD) $(LDFLAGS) -o$@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "CC\t$@\n"
	@mkdir -p $(dir $@) # Ensure the directory exists
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(BUILD_DIR)

run:
	./$(TARGET)

help:
	@echo "Available targets:"
	@echo "  all	- Build the project"
	@echo "  clean	- Remove build artifacts"
	@echo "  run	- Execute the target" 

-include $(OFILES:%.o=%.d)
