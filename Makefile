NAME = server

CXX = c++
CXXFLAGS = -g -Wall -Werror -Wextra -fsanitize=address

SRCS_DIR = ./src
OBJS_DIR = ./obj

SRCS = $(shell find src -name '*.cpp')

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.cpp=.o)))

DEPS = $(OBJS:.o=.d)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

.PHONY: all
all: $(OBJS_DIR) $(NAME)

-include $(DEPS)

$(OBJS_DIR):
	mkdir $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: lint
LINT_TARGET = $(shell find src -type f \( -name '*.cpp' -o -name '*.hpp' \))
lint: cpplint tidylint

.PHONY: cpplint
cpplint:
	@cpplint --filter=-build/include_subdir,-legal/copyright \
			$(LINT_TARGET) > /dev/null
	@printf "$(GREEN)Pass cpplint:)$(END)"
	@printf "$(GREEN)Wao, What a clean code:)$(END)"

.PHONY: tidylint
tidylint:
	@clang-tidy $(LINT_TARGET) -- $(CXXFLAGS)
	@printf "$(GREEN)Pass clang-tidy:)$(END)"
	@printf "$(GREEN)Wao, What a clean code:)$(END)"

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
