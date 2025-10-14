CC 			= gcc

CFLAGS		= -I./include -Wextra -Wall -pedantic
LINKFLAGS	= -L./lib -lSDL2main -l:libSDL2_mixer.a -lWinmm -lSDL2 -lSDL2_image -lm -l:logger64.a 

OUT			= build
SRC			= src

# finding all .c files and forming target .o filenames
C_FILES := $(subst src/,$(empty),$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
OBJS := \
	$(foreach file,$(C_FILES),$(OUT)/$(file:.c=.o)) \

all: $(OUT)/RayCaster

$(OUT)/RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o $(OUT)/RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

clean:
	rm $(OUT)/*.o
