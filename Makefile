CC 			= gcc

CFLAGS		= -I./include -Wextra -Wall -pedantic
LINKFLAGS	= -L./lib -lSDL2main -lSDL2 -lSDL2_image -lm -l:logger64.a

OUT			= build
SRC			= src

OBJS = \
		$(OUT)/main.o\
		$(OUT)/p_funcs.o\
		$(OUT)/v_funcs.o\
		$(OUT)/r_draw.o\
		$(OUT)/m_map.o\
		$(OUT)/i_init.o\
		$(OUT)/settings.o

$(OUT)/RayCaster: $(OBJS)
	$(CC) -Wall -g $(OBJS) $(LINKFLAGS) -o $(OUT)/RayCaster

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -Wall -g $(CFLAGS) -c $< -o $@

clean:
	rm $(OUT)/*.o
