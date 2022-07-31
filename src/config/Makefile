NAME = libconfig.a

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic
DEBUG			:= -fsanitize=address

HEADERS = . \
		  ../response \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

SRCS = $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp')

OBJSDIR	= ./obj
OBJS 	= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))

DEPS = $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJSDIR) $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(OBJSDIR):
	mkdir -p $(OBJSDIR)

$(OBJSDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: debug
debug:			CXXFLAGS += $(DEBUG)
debug:			re

.PHONY: clean
clean:
	@$(RM) $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

# Googleテスト
ROOT_DIR = ../..
GTEST_DIR = $(ROOT_DIR)/tests/googletest-release-1.12.1

TEST_SRCS = $(shell find gtest -name '*.cpp')

LDFLAGS = -L$(GTEST_DIR)/lib \
		  -L. \
		  $(NULL)

LDLIBS = -lgtest \
		 -lgtest_main \
		 -lpthread \
		 $(NAME) \
		 $(NULL)

$(GTEST_DIR):
	$(MAKE) -C $(ROOT_DIR) setup_gtest

.PHONY: test
test: INCLUDES += -I$(GTEST_DIR)/googletest/include
test: $(GTEST_DIR) $(NAME)
	clang++ -std=c++11 $(INCLUDES) $(TEST_SRCS) $(LDFLAGS) $(LDLIBS) -o tester
	-@./tester
	@$(RM) tester
