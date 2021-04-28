NAME = webserv

SRCS = AutoIndexGenerate.cpp\
 	   ChunkedRequest.cpp\
 	   HttpRequest.cpp\
 	   Location.cpp\
 	   Server.cpp\
 	   base64.cpp\
 	   CGI.cpp\
 	   Client.cpp\
 	   HttpResponse.cpp\
 	   Parser.cpp\
 	   WebServer.cpp\
 	   main.cpp


HEADERS = ./includes/

SRCS_DIR = ./srcs

SRCS_PATH = $(addprefix $(SRCS_DIR)/, $(SRCS))

RM = rm -f

CC = clang++

CFLAGS = -Wall -Wextra -Werror

RED	  = \x1B[31m

CYAN  = \x1B[36m

RESET = \x1B[0m

OBJS	= $(SRCS_PATH:.cpp=.o)

.cpp.o:
			@$(CC) $(CFLAGS) -I$(HEADERS) -c $< -o $(<:.cpp=.o)
			@echo  "$(CYAN)Create Object!$(RESET)"

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -I$(HEADERS) $(OBJS) -o $(NAME)
			@make clean

all:		$(NAME)
			@echo "$(RED)WEBSERVER CREATED!$(RESET)"
clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re