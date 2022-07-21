# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhirabay <mhirabay@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/13 18:47:47 by mhirabay          #+#    #+#              #
#    Updated: 2022/07/21 16:43:22 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= libresponse.a

SRCS			:=	HttpResponse.cpp \
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \
					MIMEType.cpp \
					CGI.cpp \
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

#cgiをgoogleテストで追加
gtestdir	=	./cgitest
gtest		=	$(gtestdir)/gtest $(gtestdir)/googletest-release-1.11.0

testdir = ./gtest

$(gtest):
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	$(RM) -rf release-1.11.0.tar.gz
	python3 googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(gtestdir)
	mv googletest-release-1.11.0 $(gtestdir)

test_compile = clang++ -std=c++11 $(INC) \
					HttpResponse.cpp \
					CGI.cpp \
					CGIParser.cpp\
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \
					Path.cpp \
					MIMEType.cpp \
	$(testdir)/gtest.cpp \
	$(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc \
	-I$(gtestdir) -lpthread $(LIB_UTILITY) -o tester
	
res_test_compile = clang++ -std=c++11 $(INC) \
					HttpResponse.cpp \
					CGI.cpp \
					CGIParser.cpp\
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \
					MIMEType.cpp \
					Path.cpp \
	$(testdir)/response_test.cpp \
	$(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc \
	-I$(gtestdir) -lpthread $(LIB_UTILITY) -o res_tester

.PHONY: cgitest
cgitest: $(gtest) $(LIB_UTILITY)
	$(test_compile)
	./tester # --gtest_filter=Vector.other

.PHONY: restest
restest: $(gtest) $(LIB_UTILITY)
	$(res_test_compile)
	./res_tester # --gtest_filter=Vector.other
