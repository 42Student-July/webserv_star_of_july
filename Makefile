NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic

SRCS_DIR = ./src
CONFIG_DIR = $(SRCS_DIR)/config
SERVER_DIR = $(SRCS_DIR)/server
REQUEST_DIR = $(SRCS_DIR)/request
RESPONSE_DIR = $(SRCS_DIR)/response
MODULE_DIRS = $(CONFIG_DIR) \
			  $(SERVER_DIR) \
			  $(REQUEST_DIR) \
			  $(RESPONSE_DIR) \
			  $(NULL)

SRCS = $(SRCS_DIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = $(addprefix -I, $(MODULE_DIRS))

LIB_CONFIG = $(CONFIG_DIR)/libconfig.a
LIB_SERVER = $(SERVER_DIR)/libserver.a
LIB_REQUEST = $(REQUEST_DIR)/librequest.a
LIB_RESPONSE = $(RESPONSE_DIR)/libresponse.a
LIBS = $(LIB_SERVER) \
	   $(LIB_CONFIG) \
	   $(LIB_REQUEST) \
	   $(LIB_RESPONSE) \
	   $(NULL)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS) $(LIBS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $@
	@printf "$(GREEN)Compile done:)\n$(END)"

$(LIB_SERVER): dummy
	@make -C $(SERVER_DIR)

$(LIB_CONFIG): dummy
	@make -C $(CONFIG_DIR)

$(LIB_REQUEST): dummy
	@make -C $(REQUEST_DIR)

$(LIB_RESPONSE): dummy
	@make -C $(RESPONSE_DIR)

.PHONY: dummy
dummy:

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
