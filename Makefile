NAME = webserv

CXX = c++
CXXFLAGS = -g -Wall -Werror -Wextra -fsanitize=address

SRCS_DIR = ./src
CONFIG_DIR = $(SRCS_DIR)/config
SERVER_DIR = $(SRCS_DIR)/server
MODULE_DIRS = $(CONFIG_DIR) $(SERVER_DIR)

SRCS = $(SRCS_DIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

HEADERS = $(CONFIG_DIR) \
		  $(SERVER_DIR) \
		  $(NULL)

INCLUDES = $(addprefix -I, $(HEADERS))

LIB_CONFIG = $(CONFIG_DIR)/libconfig.a
LIB_SERVER = $(SERVER_DIR)/libserver.a

LIBS = $(LIB_SERVER) $(LIB_CONFIG)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS) $(LIBS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $@
	@printf "$(GREEN)Compile done:)\n$(END)"

$(LIBS):
	@for dir in $(MODULE_DIRS); do make -C $$dir; done

.PHONY: clean
clean:
	@for dir in $(MODULE_DIRS); do make -C $$dir clean; done
	@$(RM) $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	@for dir in $(MODULE_DIRS); do make -C $$dir fclean; done
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

## Color
END		= \e[0m
BOLD	= \e[1m
THIN	= \e[2m
ITALIC	= \e[3m
U_LINE	= \e[4m
BLACK	= \e[30m
RED		= \e[31m
GREEN	= \e[32m
YELLOW	= \e[33m
BLUE	= \e[34m
PURPLE	= \e[35m
CYAN	= \e[36m
WHITE	= \e[37m
