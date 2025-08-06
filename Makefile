# Nome do executável final
NAME = philo

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# Cores para uma saída mais legível no terminal
RM = rm -f
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

# Diretórios
INC_DIR = includes/
SRC_DIR = src/
OBJ_DIR = obj/

# Arquivos fonte (.c)
SRC_FILES = main.c \
            init.c \
            utils.c \
            monitor.c \
            actions.c
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

# Arquivos objeto (.o)
OBJS = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

# Regra principal que compila o projeto
all: $(NAME)

# Regra para criar o executável
$(NAME): $(OBJS)
	@echo $(Y)Compilando [$(NAME)]...$(X)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo $(G)Projeto [$(NAME)] compilado com sucesso!$(X)

# Regra para compilar os arquivos objeto
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@echo $(Y)Compilando [$<]...$(X)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	@printf $(UP)$(CUT)

# Regra para criar o diretório de objetos, se não existir
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Regra para limpar os arquivos objeto
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo $(R)Arquivos objeto removidos.$(X)

# Regra para limpar tudo (objetos e executável)
fclean: clean
	@$(RM) $(NAME)
	@echo $(R)Executável [$(NAME)] removido.$(X)

# Regra para recompilar o projeto do zero
re: fclean all

# Regra para verificar a norma
norm:
	@norminette $(SRC_DIR) $(INC_DIR)

# Define regras que não geram arquivos
.PHONY: all clean fclean re norm
