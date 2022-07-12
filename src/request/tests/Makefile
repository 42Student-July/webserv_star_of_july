CXX = c++
CXXFLAGS = -std=c++11

SRC_DIR = ../
GTEST_DIR = gtest
GTEST_SRC_DIR = $(GTEST_DIR)/src
GTEST_LIB_DIR = gtest/googletest-release-1.12.1/lib

GTEST_SRCS = $(shell find $(GTEST_SRC_DIR) -name '*.cpp')

GTEST_INCLUDES = -Igtest/googletest-release-1.12.1/googletest/include \
				 -I$(SRC_DIR) \
				 $(NULL)

GTEST_LIBS = -lgtest \
			 -lgtest_main \
			 -lpthread \
			 $(NULL)

TEST_LIB = $(SRC_DIR)/librequest.a

googletest: $(GTEST_DIR) $(TEST_LIB)
	@$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) $(GTEST_SRCS) $(TEST_LIB) -L$(GTEST_LIB_DIR) $(GTEST_LIBS) -o test
	@./test
	@$(RM) test

$(GTEST_DIR): 
	mkdir $(GTEST_DIR)
	(cd $(GTEST_DIR) && \
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz && \
	tar -xvzf release-1.12.1.tar.gz && \
	rm -rf release-1.12.1.tar.gz)
	(cd $(GTEST_DIR)/googletest-release-1.12.1 && \
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
