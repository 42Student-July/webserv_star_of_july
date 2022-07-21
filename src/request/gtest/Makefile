CXX = c++
CXXFLAGS = -std=c++11

SRC_DIR = ../
GTEST_DIR = ../../../tests/googletest-release-1.12.1
TEST_SRC_DIR = src
GTEST_LIB_DIR = $(GTEST_DIR)/lib
UTILITY_DIR = ../../utility
INCLUDE_DIR = $(GTEST_DIR)/googletest/include \
 			  $(SRC_DIR) \
 			   ../../config \
 			   ../../response \
 			   ../../utility \
 			   $(NULL)

TEST_SRCS = $(shell find $(TEST_SRC_DIR) -name '*.cpp')

GTEST_INCLUDES = $(addprefix -I, $(INCLUDE_DIR))

GTEST_LIBS = -lgtest \
			 -lgtest_main \
			 -lpthread \
			 $(NULL)

TEST_LIB = $(SRC_DIR)/librequest.a
LIB_UTILITY = $(UTILITY_DIR)/libutility.a

all: test

$(LIB_UTILITY): dummy
	@$(MAKE) -C $(UTILITY_DIR)

$(TEST_LIB): dummy
	@$(MAKE) -C $(SRC_DIR)

.PHONY: test
test: $(GTEST_DIR) $(TEST_LIB) $(LIB_UTILITY)
	@$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) $(TEST_SRCS) $(TEST_LIB) $(LIB_UTILITY) -L$(GTEST_LIB_DIR) $(GTEST_LIBS) -o test
	-@./test
	@$(RM) test

$(GTEST_DIR): 
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
	tar -xvzf release-1.12.1.tar.gz
	mv googletest-release-1.12.1 $(GTEST_DIR)
	rm -rf release-1.12.1.tar.gz
	(cd $(GTEST_DIR) && \
	cmake . && \
	make)

.PHONY: clean
clean:
	@make -C $(SRC_DIR) clean

.PHONY: fclean
fclean: clean
	@make -C $(SRC_DIR) fclean
	@make -C $(UTILITY_DIR) fclean

.PHONY: dummy
dummy:
