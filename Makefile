CC ?= cc
PKG_CONFIG ?= pkg-config

TARGET := allegro
SOURCE := allegro.c
ALLEGRO_PACKAGES := allegro-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 \
	allegro_acodec-5 allegro_font-5 allegro_ttf-5

CPPFLAGS += $(shell $(PKG_CONFIG) --cflags $(ALLEGRO_PACKAGES) 2>/dev/null)
CFLAGS ?= -std=gnu11 -Wall -Wextra -O2
LDLIBS += $(shell $(PKG_CONFIG) --libs $(ALLEGRO_PACKAGES) 2>/dev/null) -lm

.PHONY: all check-deps run clean

all: $(TARGET)

check-deps:
	@$(PKG_CONFIG) --exists $(ALLEGRO_PACKAGES) || { \
		printf '%s\n' 'Allegro 5 development files are required. On Ubuntu/Debian: sudo apt install build-essential pkg-config liballegro5-dev'; \
		exit 1; \
	}

$(TARGET): $(SOURCE) | check-deps
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET) ranking.txt
