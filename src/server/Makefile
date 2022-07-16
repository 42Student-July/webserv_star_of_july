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

NAME = libserver.a

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic

HEADERS = . \
		  ../config \
		  ../request \
		  ../response \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

SRCS = $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp')

OBJS = $(SRCS:.cpp=.o)

DEPS = $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME):$(OBJS)
	@ar rc $(NAME) $(OBJS)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: clean
clean:
	@$(RM) $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

# gtestdir	=	./test
# gtest		=	$(gtestdir)/gtest $(gtestdir)/googletest-release-1.11.0

# testdir = ./gtest

# $(gtest):
# 	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
# 	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
# 	$(RM) -rf release-1.11.0.tar.gz
# 	python3 googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(gtestdir)
# 	mv googletest-release-1.11.0 $(gtestdir)

# test_compile = clang++ -std=c++11 ConfigParser.cpp\
# 	$(testdir)/gtest.cpp $(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc \
# 	-I$(gtestdir) -I$(includes) -I$(gtestdir)/googletest-release-1.11.0/googletest/include -lpthread -o tester

# .PHONY: test
# test: $(gtest)
# 	$(test_compile)
# 	./tester # --gtest_filter=Vector.other

#endif /* MAKEFILE */
