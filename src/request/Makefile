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
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

HEADERS = . \
		  ../config \
		  ../response \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

SRCS = $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp')

OBJS = $(SRCS:.cpp=.o)

DEPS = $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

$(NAME):$(OBJS)
	@ar rc $(NAME) $(OBJS)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

run: $(OBJS) main.o
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) main.o -MMD -MP -o test
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

# テスト
TEST_DIR = tests

.PHONY: gtest
gtest:
	make -C $(TEST_DIR) googletest


#endif /* MAKEFILE */
