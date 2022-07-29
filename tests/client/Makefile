NAME=tester

SRCS = client.cpp test.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

MAKER_SRCS = response_maker.cpp utils.cpp
MAKER_OBJS = $(MAKER_SRCS:.cpp=.o)

CXX = c++
CXXFLAGS=-Wall -Werror -Wextra -std=c++11

.PHONY: all
all: $(NAME) response_maker

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

.PHONY: run
run: $(NAME)
	./tester 127.0.0.1 4242

.PHONY: response_maker
response_maker: $(MAKER_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o response_maker $(MAKER_OBJS)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(MAKER_OBJS)

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME) response_maker

.PHONY: re
re: fclean all

