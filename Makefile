CC 			= gcc

CFLAGS		= -g -I./include -Wextra -Wall -Wno-unused-parameter
FLECSFLAGS  = -g -I./include/flecs -std=gnu99
LINKFLAGS	= -g -L./lib -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -llogger64 -lWinmm -lGdi32 -lRpcrt4 -lWs2_32 -lDbghelp

# folder to store .o files
OUT			= build
SRC			= src

# finding all .c files and forming target .o filenames
C_FILES := $(subst src/,$(empty),$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
OBJS := \
	$(foreach file,$(C_FILES),$(OUT)/$(file:.c=.o))

OBJS += $(OUT)/flecs/flecs.o

all: | $(OUT) RayCaster 

RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

$(OUT)/flecs/flecs.o: $(SRC)/flecs/flecs.c
	$(CC) $(FLECSFLAGS) -c $< -o $@


$(OUT):
	mkdir -p $(OUT)
	mkdir -p $(OUT)/flecs

clean:
	rm $(OUT)/*.o
