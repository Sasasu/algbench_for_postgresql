MODULE_big = benchmark
EXTENSION = benchmark
DATA = benchmark--1.0.0.sql

FILES += $(shell find . -name '*.c' -type f)
OBJS += $(foreach FILE, $(FILES), $(subst .c,.o, $(FILE)))
CXX_FILES += $(shell find . -name '*.cc' -type f)
OBJS += $(foreach FILE, $(CXX_FILES), $(subst .cc,.o, $(FILE)))

PG_LDFLAGS += -lstdc++

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
