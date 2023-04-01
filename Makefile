# Source files
SRCS = ./main.cpp
#HEADS = ./src/example.cpp
#HEADS_t = ./test/example.hpp

# Name of generate file: 生成ファイル名
TARGET = exe

# remove files
RMs  = *.stackdump tmpDir

# Options: コンパイルオプション
CFLAGS  = -L./googletest-master/build/lib -I./googletest-master/googletest/include -lgtest -pthread # google test
CFLAGS += -std=c++11 # CFLAGS += -std=gnu++0x
CFLAGS += -Wall
CFLAGS += -O3
CFLAGS += -fopenmp


# when you need to check the change of files in lib, you need to change file name to a not-existing name like "FORCE_XXX".
#LIB_GOOGLETEST = FORCE_GOOGLETEST
LIB_GOOGLETEST = ./googletest-master/build/lib/libgtest.a

# Tests execute
TEMP_DIR = tmpMake
TEST_DIR   := test
TEST_SRCS  := $(shell find $(TEST_DIR) -type f -name '*.cpp')
TEST_HEADS := $(shell find $(TEST_DIR) -type f -name '*.hpp')
TEST_EXES  := $(addprefix $(TEMP_DIR)/, $(patsubst %.cpp, %.exe, $(TEST_SRCS)))

GTEST_PARALLEL      = ./gtest_parallel/gtest_parallel.hpp
GTEST_PARALLEL_MAIN = ./gtest_parallel/test_main.hpp

$(TARGET): $(LIB_GOOGLETEST) $(SRCS) $(HEADS) $(HEADS_t) $(TEST_EXES) $(GTEST_PARALLEL)
	@echo "\n============================================================\n"
	@echo "SRCS: \n$(SRCS)\n"
	@echo "CFLAGS: \n$(CFLAGS)"
	@echo "\n============================================================\n"
	$(CXX) -o $(TARGET) $(SRCS) $(CFLAGS)
	@echo ""


$(LIB_GOOGLETEST):
	@echo ""
	@unzip -n googletest-master.zip
	@(cd ./googletest-master; mkdir -p build; cd build; cmake ..; make)


# 各ファイルの分割コンパイル
$(TEMP_DIR)/%.exe: %.cpp $(TEST_HEADS) $(LIB_GOOGLETEST) $(GTEST_PARALLEL_MAIN) ./src/example_math.cpp ./src/example_strEdit.cpp
	@echo ""
	mkdir -p $(dir $@);\
	$(CXX) -o $@ $< $(CFLAGS)


.PHONY: all
all:
	@(make clean)
	@(make -j)


patch            := *.stackdump
patch_txt_exists := $(shell find -name $(patch))
.PHONY: clean
clean:
	-rm -rf $(TARGET)
	-rm -rf googletest-master
	@(find . -name "__pycache__" -type d | xargs rm -rf)
	-rm -rf $(RMs)
	-rm -rf $(TEMP_DIR)


.PHONY: steps
steps: $(SRCS) $(HEADS_t)
	@(cd ./sstd; make steps)
	@echo ""
	@echo "$^" | xargs wc -l

