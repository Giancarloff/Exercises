# Directories
SRC_DIR = src
BIN_DIR = bin

# Target
TARGET = $(BIN_DIR)/clam

# Rules
debug:
	@cargo build
	@mkdir -p $(BIN_DIR)
	@cp target/debug/clam $(TARGET)
	@rm -rf target

all:
	@cargo build --release
	@mkdir -p $(BIN_DIR)
	@cp target/release/clam $(TARGET)
	@rm -rf target

clean:
	@cargo clean
	@rm -rf $(BIN_DIR)
	@mkdir bin

.PHONY: all clean
