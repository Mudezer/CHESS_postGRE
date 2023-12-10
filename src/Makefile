EXTENSION   = chess
MODULES 	= chess
DATA        = chess--0.1.sql chess.control

LDFLAGS=-lrt

PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)