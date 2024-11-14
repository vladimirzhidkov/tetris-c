SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
PROJECT = $(BUILD_DIR)/tetris.out
VERBOSE = 0

CC = gcc
CPP = $(CC)
AS = $(CC)
LD = $(CC)

CPPFLAGS = -MD -I$(INCLUDE_DIR)
CFLAGS = -g -O0 -c -std=c99 -Wall #-save-temps=obj
LDFLAGS = -lncurses #-v

CFILES = main.c model.c view.c 
SFILES =
IFILES = $(CFILES:%.c=$(BUILD_DIR)/%.i)
OFILES = $(CFILES:%.c=$(BUILD_DIR)/%.o)
OFILES += $(SFILES:%.s=$(BUILD_DIR)/%.o)

ifeq ($(VERBOSE), 0)
	Q = @
endif

.PHONY: all clean
all: $(PROJECT)
run:
	./$(PROJECT)
clean:
	rm -rf $(BUILD_DIR)

$(PROJECT): $(OFILES)
	@printf "LD\t$@\n"
	$(Q)$(LD) $(LDFLAGS) -o$@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "CC\t$@\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o$@ $<

-include $(OFILES:%.o=%.d)
