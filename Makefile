CC 			= gcc

CFLAGS		= -g -I./include -Wextra -Wall -Wno-unused-parameter
LINKFLAGS	= -g -L./lib -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -llogger64 -lWinmm -lGdi32 -lRpcrt4 

# folder to store .o files
OUT			= build
SRC			= src

# finding all .c files and forming target .o filenames
C_FILES := $(subst src/,$(empty),$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
OBJS := \
	$(foreach file,$(C_FILES),$(OUT)/$(file:.c=.o)) \

all: | $(OUT) RayCaster 

RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

$(OUT):
	mkdir -p $(OUT)

clean:
	rm $(OUT)/*.o
