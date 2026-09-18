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

# PROGRAMS #########################

SERVER_NAME = server 
CLIENT_NAME = client 
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow -D_GLIBCXX_USE_CXX11_ABI=0 -g3

# SERVER #########################

SERVER_DIR = ./bank_server

SERVER_INCS = $(wildcard $(SERVER_DIR)/*.hpp)
SERVER_SRCS = $(wildcard $(SERVER_DIR)/*.cpp)

SERVER_OBJ_DIR = $(SERVER_DIR)/obj
SERVER_OBJS = $(SERVER_SRCS:$(SERVER_DIR)/%.cpp=$(SERVER_OBJ_DIR)/%.o)

# CLIENT #########################

CLIENT_DIR = ./customer_client

CLIENT_INCS = $(wildcard $(CLIENT_DIR)/*.hpp)
CLIENT_SRCS = $(wildcard $(CLIENT_DIR)/*.cpp)


CLIENT_OBJ_DIR = $(CLIENT_DIR)/obj
CLIENT_OBJS = $(CLIENT_SRCS:$(CLIENT_DIR)/%.cpp=$(CLIENT_OBJ_DIR)/%.o)

# RULES ###############################

all: $(SERVER_NAME) $(CLIENT_NAME)

# SERVER ##############################

$(SERVER_NAME): $(SERVER_OBJS)
	@echo "${WHITE_BG}${BLACK}Building $(SERVER_NAME) and linking .o files...${RESET}"
	$(CPP) $(FLAGS) $(SERVER_OBJS) -o $(SERVER_NAME)
	@echo "${GREEN_BG}${WHITE}$(SERVER_NAME) successfully built and .o files linked!${RESET}"

$(SERVER_OBJ_DIR)/%.o: $(SERVER_DIR)/%.cpp $(SERVER_INCS)
	@echo "${PURPLE_BG}${WHITE}Compiling $< into $@...${RESET}"
	@mkdir -p $(@D)
	@$(CPP) $(FLAGS) -I$(SERVER_DIR) -c $< -o $@
	@echo "${GREEN_BG}${WHITE}$@ was successfully created.${RESET}"

# CLIENT ##############################

$(CLIENT_NAME): $(CLIENT_OBJS)
	@echo "${WHITE_BG}${BLACK}Building $(CLIENT_NAME) and linking .o files...${RESET}"
	$(CPP) $(FLAGS) $(CLIENT_OBJS) -o $(CLIENT_NAME)
	@echo "${GREEN_BG}${WHITE}$(CLIENT_NAME) successfully built and .o files linked!${RESET}"

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_DIR)/%.cpp $(CLIENT_INCS)
	@echo "${PURPLE_BG}${WHITE}Compiling $< into $@...${RESET}"
	@mkdir -p $(@D)
	@$(CPP) $(FLAGS) -I$(CLIENT_DIR) -c $< -o $@
	@echo "${GREEN_BG}${WHITE}$@ was successfully created.${RESET}"

# CLEAN ##############################

clean:
	@echo "${YELLOW_BG}${WHITE}Deleting $(SERVER_OBJ_DIR) and $(CLIENT_OBJ_DIR) .o files...${RESET}"
	@rm -rf $(SERVER_OBJ_DIR)
	@rm -rf $(CLIENT_OBJ_DIR)
	@echo "${GREEN_BG}${WHITE}$(SERVER_OBJ_DIR) and $(CLIENT_OBJ_DIR) .o files deleted!${RESET}"

fclean: clean
	@echo "${RED_BG}${WHITE}Deleting $(SERVER_NAME) and $(CLIENT_NAME)...${RESET}"
	@rm -f $(SERVER_NAME)
	@rm -f $(CLIENT_NAME)
	@echo "${GREEN_BG}${WHITE}$(SERVER_NAME) and $(CLIENT_NAME) successfully deleted!${RESET}"

re: fclean all
	@echo "${BLUE}Rebuilding $(SERVER_NAME) and $(CLIENT_NAME)...${RESET}"
	@echo "${GREEN_BG}${WHITE}$(SERVER_NAME) and $(CLIENT_NAME) successfully rebuilt!${RESET}"

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
