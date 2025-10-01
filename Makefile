APP := typetest
BUILD_DIR := build

BINARY := $(BUILD_DIR)/$(APP)

all: $(BUILD_DIR)
	make -C $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

run: all
	./$(BINARY)

clean:
	rm -r $(BUILD_DIR)

debug: all
	./debug.sh $(BINARY)

.PHONY: all clean run debug
