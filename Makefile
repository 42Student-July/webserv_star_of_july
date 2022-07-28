NAME = webserv

SHELL = /bin/bash

CXX = c++
DEBUG			:= -fsanitize=address
CXXFLAGS = -Wall -Werror -Wextra -Wshadow -std=c++98 -pedantic

SRCS_DIR = ./src
CONFIG_DIR = $(SRCS_DIR)/config
SERVER_DIR = $(SRCS_DIR)/server
REQUEST_DIR = $(SRCS_DIR)/request
RESPONSE_DIR = $(SRCS_DIR)/response
UTILITY_DIR = $(SRCS_DIR)/utility
MODULE_DIRS = $(CONFIG_DIR) \
			  $(SERVER_DIR) \
			  $(REQUEST_DIR) \
			  $(RESPONSE_DIR) \
			  $(UTILITY_DIR) \
			  $(NULL)

SRCS = $(SRCS_DIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = $(addprefix -I, $(MODULE_DIRS))

LIB_CONFIG = $(CONFIG_DIR)/libconfig.a
LIB_SERVER = $(SERVER_DIR)/libserver.a
LIB_REQUEST = $(REQUEST_DIR)/librequest.a
LIB_RESPONSE = $(RESPONSE_DIR)/libresponse.a
LIB_UTILITY = $(UTILITY_DIR)/libutility.a
LIBS = $(LIB_SERVER) \
	   $(LIB_CONFIG) \
	   $(LIB_REQUEST) \
	   $(LIB_RESPONSE) \
	   $(LIB_UTILITY) \
	   $(NULL)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(NAME): $(OBJS) $(LIBS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $@
	@printf "$(GREEN)Compile done:)\n$(END)"

.PHONY: debug
debug: $(OBJS) 
	for dir in $(MODULE_DIRS); do $(MAKE) -C $$dir debug; done
	$(CXX) $(DEBUG) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)
	printf "$(GREEN)Compile with -g flag done:)\n$(END)"

$(LIB_DEBUG): dummy
	@$(MAKE) -C $(SERVER_DIR)

$(LIB_SERVER): dummy
	@$(MAKE) -C $(SERVER_DIR)

$(LIB_CONFIG): dummy
	@$(MAKE) -C $(CONFIG_DIR)

$(LIB_REQUEST): dummy
	@$(MAKE) -C $(REQUEST_DIR)

$(LIB_RESPONSE): dummy
	@$(MAKE) -C $(RESPONSE_DIR)

$(LIB_UTILITY): dummy
	@$(MAKE) -C $(UTILITY_DIR)

.PHONY: dummy
dummy:

.PHONY: clean
clean:
	@for dir in $(MODULE_DIRS); do $(MAKE) -C $$dir clean; done
	@$(RM) $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	@for dir in $(MODULE_DIRS); do $(MAKE) -C $$dir fclean; done
	@$(RM) $(NAME)

.PHONY: re
re: fclean all


## Test
TEST_DIRS = $(CONFIG_DIR) \
			$(REQUEST_DIR) \
			$(RESPONSE_DIR) \
			$(NULL)

.PHONY: unit_test
unit_test:
	@for dir in $(TEST_DIRS); do $(MAKE) -C $$dir test; done

.PHONY: chunk_test
chunk_test:
	# (cd tests/sandbox/chunked_req/ && ./test_chunked_request.sh)
	cd tests/sandbox/chunked_req/ && ./test_chunked_request.sh


GTEST_DIR = tests/googletest-release-1.12.1

.PHONY:setup_gtest
setup_gtest: $(GTEST_DIR)

$(GTEST_DIR):
	mkdir -p tests
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
	tar -xvzf release-1.12.1.tar.gz
	rm -rf release-1.12.1.tar.gz
	(cd googletest-release-1.12.1 && \
	cmake . && \
	make)
	mv googletest-release-1.12.1 tests

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
