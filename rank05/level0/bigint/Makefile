CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp bigint.cpp
HDRs = *.hpp
NAME = test

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(NAME) $(OBJ)
