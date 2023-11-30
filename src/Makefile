MODULE_big	= chess
OBJS = \
	$(WIN32RES) \
	chess.o

EXTENSION   = chess
DATA        = chess--0.1.sql
HEADERS_chess = smallchesslib.h

PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)