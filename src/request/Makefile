#ifndef MAKEFILE
#define MAKEFILE

# all:
# 	c++ -Werror -Wall -Wextra -g *.cpp -o server

# run:
# 	./server tests/listen.conf

# test:
# 	g++ gtest.cpp ConfigParser.cpp -pthread -Llib -lgtest_main -Llib -lgtest -std=gnu++11 -o tester
# 	./tester
# 	rm tester

NAME = librequest.a

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic
DEBUG			:= -fsanitize=address

INCLUDE_PATH = . \
		  ../config \
		  ../response \
		  ../utility \
		  $(NULL)

INCLUDES = $(addprefix -I, $(INCLUDE_PATH))

SRCS = $(notdir $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp'))

OBJSDIR	= ./obj
OBJS 	= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))

DEPS = $(OBJS:.o=.d)

UTILITY_DIR = ../utility
CONFIG_DIR = ../config
LIB_UTILITY = $(UTILITY_DIR)/libutility.a
LIB_CONFIG = $(CONFIG_DIR)/libconfig.a

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJSDIR) $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(OBJSDIR):
	mkdir -p $(OBJSDIR)

$(OBJSDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: run
run: $(OBJS) main.o $(LIB_UTILITY)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIB_UTILITY) main.o -MMD -MP -o test
	@./test
	@$(RM) test

.PHONY: debug
debug:			CXXFLAGS += $(DEBUG)
debug:			re

.PHONY: clean
clean:
	@$(RM) $(OBJS) $(DEPS) main.o main.d

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

$(LIB_UTILITY): dummy
	@$(MAKE) -C $(UTILITY_DIR)

$(LIB_CONFIG): dummy
	@$(MAKE) -C $(CONFIG_DIR)

.PHONY: dummy
dummy:

# Googleテスト
ROOT_DIR = ../..
GTEST_DIR = $(ROOT_DIR)/tests/googletest-release-1.12.1

TEST_SRCS = $(shell find gtest/src -name '*.cpp')

LDFLAGS = -L$(GTEST_DIR)/lib \
		  -L. \
		  -L$(UTILITY_DIR) \
		  $(NULL)

LDLIBS = -lgtest \
		 -lgtest_main \
		 -lpthread \
		 $(NAME) \
		 $(LIB_UTILITY) \
		 $(LIB_CONFIG) \
		 $(NULL)

$(GTEST_DIR):
	$(MAKE) -C $(ROOT_DIR) setup_gtest

.PHONY: test
test: INCLUDES += -I$(GTEST_DIR)/googletest/include
test: $(GTEST_DIR) $(NAME) $(LIB_UTILITY) $(LIB_CONFIG)
	clang++ -std=c++11 $(INCLUDES) $(TEST_SRCS) $(LDFLAGS) $(LDLIBS) -o test
	-@./test
	@$(RM) test

.PHONY: test
body_test: TEST_SRCS = gtest/src/body_parser_test.cpp
body_test: test

# test: $(GTEST_DIR) $(NAME) $(LIB_UTILITY)
# 	$(CXX) -std=c++11 $(INCLUDES) $(TEST_SRCS) $(NAME) $(LIB_UTILITY) -L$(GTEST_LIB_DIR) $(LDLIBS) -o test
# 	-@./test
# 	@$(RM) test

#endif /* MAKEFILE */
