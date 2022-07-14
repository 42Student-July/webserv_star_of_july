# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhirabay <mhirabay@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/13 18:47:47 by mhirabay          #+#    #+#              #
#    Updated: 2022/07/07 18:04:32 by mhirabay         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= a.out

SRCS			:=	main.cpp \
					HttpResponse.cpp \
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \

CXX				:= c++
RM				:= rm -rf
INC				:= -I . \

CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98 $(INC) -pedantic -Wshadow
DEBUG			:= -fsanitize=address


OBJSDIR	:= ./obj/
OBJS 	:= $(addprefix $(OBJSDIR), $(SRCS:%.cpp=%.o))

SRCSDIR := ./

all:			$(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

$(OBJS) : $(OBJSDIR)

$(OBJSDIR)%.o : ./%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@


$(OBJSDIR):
	mkdir -p $(OBJSDIR)

clean:
				$(RM) $(OBJS)
				$(RM) $(OBJSDIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY: debug
debug:			CXXFLAGS += $(DEBUG)
debug:			re

.PHONY:			all clean fclean re

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

test_compile = clang++ -std=c++11 HttpResponse.cpp \
					HttpResponseBuilder.cpp \
					HttpResponseSerializer.cpp \
					HttpResponsePlainText.cpp \
					CGI.cpp \
					CGIParser.cpp\
	$(testdir)/gtest.cpp $(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc \
	-I$(gtestdir) -I$(includes) -lpthread -o tester

.PHONY: cgitest
cgitest: $(gtest)
	$(test_compile)
	./tester # --gtest_filter=Vector.other
