# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhirabay <mhirabay@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/13 18:47:47 by mhirabay          #+#    #+#              #
#    Updated: 2022/07/25 13:18:18 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= libresponse.a

SRCS			:=	HttpResponse.cpp \
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \
					MIMEType.cpp \
					CGI.cpp \
					CGIParser.cpp\
					Path.cpp

CXX				:= c++
RM				:= rm -rf
INC				:= -I . \
				   -I ../utility

CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98 $(INC) -pedantic -Wshadow
DEBUG			:= -fsanitize=address


OBJSDIR	:= ./obj/
OBJS 	:= $(addprefix $(OBJSDIR), $(SRCS:%.cpp=%.o))
DEPS	:= $(OBJS:.o=.d)

SRCSDIR := ./

UTILITY_DIR := ../utility
LIB_UTILITY := $(UTILITY_DIR)/libutility.a

all:			$(NAME)

$(NAME) : $(OBJSDIR) $(OBJS)
	ar -rc $(NAME) $(OBJS)

-include $(DEPS)

$(OBJSDIR)%.o : ./%.cpp
	${CXX} ${CXXFLAGS} -MMD -MP -c $< -o $@

$(OBJSDIR):
	mkdir -p $(OBJSDIR)

# mainを実行できるコマンド。allでアーカイブを作るようにしたので
run : main.o $(NAME)
	$(CXX) $(CXXFLAGS) $^ -o a.out
	./a.out
	$(RM) a.out

clean:
				$(RM) $(OBJS) $(DEPS)
				$(RM) $(OBJSDIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY: debug
debug:			CXXFLAGS += $(DEBUG)
debug:			re

.PHONY:			all clean fclean re run

$(LIB_UTILITY): dummy
	@$(MAKE) -C $(UTILITY_DIR)

.PHONY: dummy
dummy:

error_status_grep = grep 'HttpStatus::' $(shell find ../request -maxdepth 1 -name '*.cpp') \
					| grep -v OK \
					| cut -d : -f 4 \
					| cut -d , -f 1 \
					| sort \
					| uniq
# リクエストパース中に発生するエラーステータス一覧を表示する
.PHONY: parse_error_status
parse_error_status:
	@$(error_status_grep)
	@echo "###"
	@echo -n "種類は"
	@$(error_status_grep) | wc -l
	@echo "###"

# Googleテスト
ROOT_DIR = ../..
GTEST_DIR = $(ROOT_DIR)/tests/googletest-release-1.12.1

TEST_RESPONSE_SRCS = gtest/response_test.cpp
TEST_CGI_SRCS = gtest/gtest.cpp

LDFLAGS = -L$(GTEST_DIR)/lib \
		  -L. \
		  -L$(UTILITY_DIR) \
		  $(NULL)

LDLIBS = -lgtest \
		 -lgtest_main \
		 -lpthread \
		 $(NAME) \
		 $(LIB_UTILITY) \
		 $(NULL)

$(GTEST_DIR):
	$(MAKE) -C $(ROOT_DIR) setup_gtest

.PHONY: test
test: restest cgitest

.PHONY: restest
restest: INC += -I$(GTEST_DIR)/googletest/include
restest: $(GTEST_DIR) $(NAME) $(LIB_UTILITY)
	clang++ -std=c++11 $(INC) $(TEST_RESPONSE_SRCS) $(LDFLAGS) $(LDLIBS) -o tester
	-@./tester
	@$(RM) tester


.PHONY: cgitest
cgitest: INC += -I$(GTEST_DIR)/googletest/include
cgitest: $(GTEST_DIR) $(NAME) $(LIB_UTILITY)
	clang++ -std=c++11 $(INC) $(TEST_CGI_SRCS) $(LDFLAGS) $(LDLIBS) -o tester
	-@./tester
	@$(RM) tester

