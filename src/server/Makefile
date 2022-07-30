NAME = libserver.a

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic
DEBUG			:= -fsanitize=address

HEADERS = . \
		  ../config \
		  ../request \
		  ../response \
		  ../utility \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

SRCS = $(shell find . -maxdepth 1 -name '*.cpp' -not -name 'main.cpp')

OBJSDIR	= ./obj
OBJS 	= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))

DEPS = $(OBJS:.o=.d)

UTILITY_DIR = ../utility
LIB_UTILITY = $(UTILITY_DIR)/libutility.a

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

# $(LIB_UTILITY): dummy
# 	@$(MAKE) -C $(UTILITY_DIR)

# .PHONY: dummy
# dummy:
