#files with source code
SOURCE_DIR = src/
EXEC_DIR = executable/
SOURCE= $(addprefix $(SOURCE_DIR), cache_2q.cpp cache_ideal.cpp)
CMP_SOURCE = $(addprefix $(SOURCE_DIR), main.cpp)

PROJECT=cache
#compilation options
DBGFLAGS = -Wall -Wfloat-equal -Wundef -DDEBUG
BLDFLAGS = $(CXXFLAGS) -O3
TSTFLAGS = -Wall -Wfloat-equal -Wundef -lgtest -lgmock -pthread -DTEST
#strange define, but it works for foreach
.PHONY: all debug test cmp_cache
define \n


endef

define BUILD_LIST
	$(foreach elem, $(1), $(CXX) $(elem) $(2)  -o $(EXEC_DIR)$(basename $(notdir $(elem))).out ${\n})
endef

all: $(SOURCE)
	mkdir -p $(EXEC_DIR)
	$(call BUILD_LIST, $(SOURCE), $(BLDFLAGS))

debug: $(SOURCE)
	mkdir -p $(EXEC_DIR)
	$(call BUILD_LIST, $(SOURCE), $(DBGFLAGS))


test: $(SOURCE)
	mkdir -p $(EXEC_DIR)
	$(call BUILD_LIST, $(SOURCE), $(TSTFLAGS))

cmp_cache: $(CMP_SOURCE)
	mkdir -p $(EXEC_DIR)
	$(CXX) $(BLDFLAGS) $(CMP_SOURCE) -o $(EXEC_DIR)$(basename $(notdir $(CMP_SOURCE))).out


clean_all:
	rm -rf *.o $(foreach elem, $(SOURCE) $(CMP_SOURCE), $(basename $(elem)).out)

clean:
	rm -rf *.o 