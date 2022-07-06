# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhirabay <mhirabay@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/13 18:47:47 by mhirabay          #+#    #+#              #
#    Updated: 2022/07/04 14:38:51 by mhirabay         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= a.out

SRCS			:=	main.cpp ConfigDTO.cpp HttpRequestData.cpp HttpResponse.cpp HttpResponseBuilder.cpp

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
