BLACK = \033[30m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
PURPLE = \033[35m
CYAN = \033[36m
WHITE = \033[37m

RESET = \033[0m

BLACK_BG = \033[40m
RED_BG = \033[41m
GREEN_BG = \033[42m
YELLOW_BG = \033[43m
BLUE_BG = \033[44m
PURPLE_BG = \033[45m
CYAN_BG = \033[46m
WHITE_BG = \033[47m

NAME = encap
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow -D_GLIBCXX_USE_CXX11_ABI=0 -g3

INC_DIR = ./ex00
INCS = $(wildcard $(INC_DIR)/*.hpp)

SRC_DIR = ./ex00
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR = ./ex00/obj
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@echo "${WHITE}Making the directory for object files...${RESET}"
	@mkdir -p $(OBJ_DIR)
	@echo "${GREEN}\\t$(OBJ_DIR) was successfully created.${RESET}"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS)
	@echo "${PURPLE}Compiling $< into $@...${RESET}"
	@mkdir -p $(@D)
	@$(CPP) $(FLAGS) -I$(INC_DIR) -c $< -o $@
	@echo "${GREEN}\\t$@ was successfully created.${RESET}"

$(NAME): $(OBJS)
	@echo "${WHITE_BG}${BLACK}Building $(NAME) and linking .o files...${RESET}"
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "${GREEN_BG}${WHITE}$(NAME) successfully built and .o files linked!${RESET}"

clean:
	@echo "${YELLOW}Deleting $(OBJ_DIR) and .o files...${RESET}"
	@rm -rf $(OBJ_DIR)
	@echo "${GREEN}\\t$(OBJ_DIR) and .o files deleted!${RESET}"

fclean: clean
	@echo "${RED}Deleting $(NAME)...${RESET}"
	@rm -f $(NAME)
	@echo "${GREEN}$(NAME) successfully deleted!${RESET}"

re: fclean all
	@echo "${BLUE}Rebuilding $(NAME)...${RESET}"
	@echo "${GREEN}\\t$(NAME) successfully rebuilt!${RESET}"

colors:
	@echo "${BLACK}${WHITE_BG}white background...${RESET}"
	@echo "${BLACK}${RED_BG}red background...${RESET}"
	@echo "${WHITE}${GREEN_BG}green background...${RESET}"
	@echo "${BLACK}${YELLOW_BG}yellow background...${RESET}"
	@echo "${WHITE}${BLUE_BG}blue background...${RESET}"
	@echo "${WHITE}${PURPLE_BG}purple background...${RESET}"
	@echo "${BLACK}${CYAN_BG}cyan background...${RESET}"
	@echo "${RED}red...${RESET}"
	@echo "${GREEN}green...${RESET}"
	@echo "${YELLOW}yellow...${RESET}"
	@echo "${BLUE}blue...${RESET}"
	@echo "${PURPLE}purple...${RESET}"
	@echo "${CYAN}cyan...${RESET}"
	@echo "${WHITE}white...${RESET}"
	@echo "${RESET}normal...${RESET}"

.PHONY: all clean fclean re colors
