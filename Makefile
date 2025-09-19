PROJECT_NAME := typetest

ASSETS_DIR := assets
SOURCE_DIR := src
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/obj

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

CC := gcc
HFLAGS := -I$(SOURCE_DIR)

CFLAGS := -Wall -Wextra -Wshadow -Wconversion -Wfloat-equal -Wno-unused-const-variable -Wno-sign-conversion -Wfatal-errors -pedantic -std=c11
LDFLAGS := -lncursesw

RELEASE_CFLAGS := -O3 -march=native -flto -fno-plt -pipe
RELEASE_LDFLAGS := -flto -fno-plt -s

DEBUG_CFLAGS := -Og
# DEBUG_CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer

DEBUG_LDFLAGS :=
# DEBUG_LDFALGS += -fsanitize=address,undefined

TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

all: CFLAGS += $(DEBUG_CFLAGS)
all: LDFLAGS += $(DEBUG_LDFLAGS)
all: $(BUILD_DIR) $(OBJECT_DIR) $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJECT_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c files into .o files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(HFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	@printf "\tRunning $(PROJECT_NAME) ...\n"
	@$(TARGET)

debug: all
	./debug.sh $(TARGET)

release: CFLAGS += $(RELEASE_CFLAGS)
release: LDFLAGS += $(RELEASE_LDFLAGS)
release: all

$(PREFIX):
	mkdir -p $(PREFIX)

.PHONY: all clean run debug release
