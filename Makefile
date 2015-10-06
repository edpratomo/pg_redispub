
MODULE_big = pg_redispub
OBJS = pg_redispub.o
DATA = pg_redispub--1.0.sql
DOCS =

EXTENSION = pg_redispub

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

SHLIB_LINK += -lhiredis
