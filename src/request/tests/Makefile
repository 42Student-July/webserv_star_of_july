CXX = c++
CXXFLAGS = -std=c++11

GTEST_DIR = gtest
GTEST_SRC_DIR = $(GTEST_DIR)/src
GTEST_LIB_DIR = gtest/googletest-release-1.12.1/lib

GTEST_INCLUDES = -Igtest/googletest-release-1.12.1/googletest/include

GTEST_LIBS = -lgtest \
			 -lgtest_main \
			 -lpthread \
			 $(NULL)

GTEST_SRCS = $(shell find $(GTEST_SRC_DIR) -name '*.cpp')

googletest: $(GTEST_DIR)
	@$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) $(GTEST_SRCS) -L$(GTEST_LIB_DIR) $(GTEST_LIBS) -o test
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
