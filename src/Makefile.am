bin_PROGRAMS = tfg

# set the include path found by configure
AM_CPPFLAGS =  $(LIBSDL_CFLAGS) $(all_includes)

# the library search path.
noinst_HEADERS = classe_mapa.h classe_mina.h classe_pai.h gfx_sdl.h main.h
tfg_SOURCES = classe_general.h classe_mapa.h classe_mina.h classe_pai.h \
	gfx_sdl.h tfg.cpp
tfg_LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx
