CC 			= gcc

CFLAGS		= -I./include -Wextra -Wall -Wno-unused-parameter

LINKFLAGS 	=
ifeq ($(OS), Windows_NT)
	LINKFLAGS += -L./lib/windows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lWinmm -lGdi32 -lRpcrt4 -lWs2_32 -lDbghelp
else
	LINKFLAGS += -L/usr/local/lib/ $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image SDL2_mixer) -lm
endif

# folder to store .o files
OUT			= build
SRC			= src

# finding all .c files and forming target .o filenames
C_FILES := $(subst src/,$(empty),$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
OBJS := \
	$(foreach file,$(C_FILES),$(OUT)/$(file:.c=.o))

all: | $(OUT) RayCaster 

RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

$(OUT):
	mkdir -p $(OUT)

clean:
	rm $(OUT)/*.o
