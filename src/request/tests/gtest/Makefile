CXX = c++
CXXFLAGS = -std=c++11

SRC_DIR = ../../
GTEST_DIR = googletest-release-1.12.1
TEST_SRC_DIR = src
GTEST_LIB_DIR = $(GTEST_DIR)/lib
INCLUDE_DIR = $(GTEST_DIR)/googletest/include \
 			  $(SRC_DIR) \
 			   ../../../config \
 			   $(NULL)

TEST_SRCS = $(shell find $(TEST_SRC_DIR) -name '*.cpp')

GTEST_INCLUDES = $(addprefix -I, $(INCLUDE_DIR))

GTEST_LIBS = -lgtest \
			 -lgtest_main \
			 -lpthread \
			 $(NULL)

TEST_LIB = $(SRC_DIR)/librequest.a

.PHONY: test
test: $(GTEST_DIR) $(TEST_LIB)
	@$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) $(TEST_SRCS) $(TEST_LIB) -L$(GTEST_LIB_DIR) $(GTEST_LIBS) -o test
	@./test
	@$(RM) test

$(GTEST_DIR): 
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
	tar -xvzf release-1.12.1.tar.gz
	rm -rf release-1.12.1.tar.gz
	(cd $(GTEST_DIR) && \
	cmake . && \
	make)

$(TEST_LIB):
	@make -C $(SRC_DIR)

.PHONY: clean
clean:
	@make -C $(SRC_DIR) clean

.PHONY: fclean
fclean: clean
	@make -C $(SRC_DIR) fclean
