CC := gcc
TARGET := chip-8

HEADERS := emulator.h display.h keypad.h
FILES := main.c emulator.c display.c keypad.c

$(TARGET): $(FILES) $(HEADERS)
	@echo "Compiling: " $(TARGET)
	@$(CC) $(FILES) -lSDL3 -o $(TARGET) -g

clean:
	@if [[ -f "$(TARGET)" ]]; then \
		echo "Cleaning: " $(TARGET); \
		rm $(TARGET); \
	else \
		echo "Nothing to clean..."; \
	fi

