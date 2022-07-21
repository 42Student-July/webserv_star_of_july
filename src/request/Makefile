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

HEADERS = . \
		  ../config \
		  ../response \
		  ../utility \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

SRCS = $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp')

OBJS = $(SRCS:.cpp=.o)

DEPS = $(OBJS:.o=.d)

UTILITY_DIR = ../utility
LIB_UTILITY = $(UTILITY_DIR)/libutility.a

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME):$(OBJS)
	@ar rc $(NAME) $(OBJS)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: run
run: $(OBJS) main.o $(LIB_UTILITY)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIB_UTILITY) main.o -MMD -MP -o test
	@./test
	@$(RM) test

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

.PHONY: dummy
dummy:

# テスト
TEST_DIR = gtest

.PHONY: test
test:
	@$(MAKE) -C $(TEST_DIR) test


#endif /* MAKEFILE */
