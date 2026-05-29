CC 			= gcc
CPP			= g++

# C++ flags (keep C flags unchanged)
CXXFLAGS	= -I./include++ -Wextra -Wall -Wno-unused-parameter

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

SRCPP = src++

# finding all .c files and forming target .o filenames
C_FILES := $(subst src/,$(empty),$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
COBJS := \
	$(foreach file,$(C_FILES),$(OUT)/$(file:.c=.o))

CPP_FILES := $(subst src++/,$(empty),$(wildcard $(SRCPP)/*.cpp))
CPPOBJS := \
	$(foreach file,$(CPP_FILES),$(OUT)/$(file:.cpp=.opp))

# ensure OBJS points to the C object list (preserve original C build)
OBJS := $(COBJS)

all: RayCaster RayCaster_cpp

RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

$(OUT)/%.opp: $(SRCPP)/%.cpp
	$(CPP) -Wall -g $(CXXFLAGS) -c $< -o $@

RayCaster_cpp: $(CPPOBJS)
	$(CPP) -Wall -g $(CPPOBJS) $(LINKFLAGS) -o RayCaster_cpp

$(OUT):
	mkdir -p $(OUT)

clean:
	rm -f $(OUT)/*.o $(OUT)/*.opp RayCaster RayCaster_cpp
