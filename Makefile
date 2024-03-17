ifeq ($(OS), Windows_NT)
	FLAGS = \
		-I"/mingw64/include/gtk-3.0" \
		-I"/mingw64/include/pango-1.0" \
		-I"/mingw64/include/cairo" \
		-I"/mingw64/include/gdk-pixbuf-2.0" \
		-I"/mingw64/include/harfbuzz" \
		-I"/mingw64/include/glib-2.0" \
		-I"/mingw64/lib/glib-2.0/include" \
		-I"/mingw64/include/freetype2" \
		-I"/mingw64/include/atk-1.0" \
		-I"/mingw64/include/webp" \
		-DLIBDEFLATE_DLL \
		-I"/mingw64/include/libpng16" \
		-I"/mingw64/include/pixman-1" \
		-I"/mingw64/include/fribidi" \
		-Wall \
		-Wextra \

	LIBS = \
		-L"/usr/lib/gcc/x86_64-pc-msys/13.2.0" \
		-L"/mingw64/lib" \
		-lgtk-3 \
		-lgdk-3 \
		-lz \
		-lgdi32 \
		-limm32 \
		-lshell32 \
		-lole32 \
		-luuid \
		-lwinmm \
		-ldwmapi \
		-lsetupapi \
		-lcfgmgr32 \
		-lhid \
		-lwinspool \
		-lcomctl32 \
		-lcomdlg32 \
		-lpangowin32-1.0 \
		-lharfbuzz \
		-lpangocairo-1.0 \
		-lpango-1.0 \
		-latk-1.0 \
		-lcairo \
		-lcairo-gobject \
		-lgdk_pixbuf-2.0 \
		-lgio-2.0 \
		-lglib-2.0 \
		-lintl \
		-lgobject-2.0

else
	FLAGS = $$(pkg-config --cflags gtk+-3.0) -Wall -Wextra
	LIBS = $$(pkg-config --libs gtk+-3.0)
endif

COMPILER = gcc
NAME = hermes
INCLUDE = -I"include/"
FILES = main pedido produto sessao utilidades

SOURCES = $(FILES:%=source/%.c)
OBJECTS = $(FILES:%=objects/%.o)

all: main

run:
	@./$(NAME)

objects/%.o: source/%.c
	@$(COMPILER) $(LIBS) $(FLAGS) $(INCLUDE) -c $< -o $@

main: $(OBJECTS)
	@$(COMPILER) $(OBJECTS) $(LIBS) $(FLAGS) $(INCLUDE) -o $(NAME)

clean:
	@rm -f $(NAME)
	@rm $(OBJECTS)