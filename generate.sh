#!/bin/bash

# Format
c_formatter_42 $(find . -type f \( -name "*.c" -o -name "*.h" \))

# Comprobar si se pasó un argumento para el nombre del programa
if [ -n "$1" ]; then
    NAME="$1"
else
    NAME="programa"
fi

# Comprobar si se pasó un segundo argumento para el subdirectorio (ej: libft/)
if [ -n "$2" ]; then
    SUBSYSTEM_PATH="$2"
    SUBSYSTEM_LIB="$(basename "$SUBSYSTEM_PATH").a"
else
    SUBSYSTEM_PATH=""
    SUBSYSTEM_LIB=""
fi

# Configuración del compilador y flags
CC="cc"
CFLAGS="-Wall -Wextra -Werror"

# Buscar directorios que contienen archivos .h y construir -I<dir>
INCLUDE_DIRS=$(find . -type f -name "*.h" -exec dirname {} \; | sort -u)
INCLUDES=""
for dir in $INCLUDE_DIRS; do
    INCLUDES+=" -I$dir"
done

# Directorios
SRC_DIR="./"
OBJ_DIR="obj"

# Generar la lista de fuentes
SRCS_LIST=($(find "$SRC_DIR" -type f -name "*.c"))  # Array con todos los archivos .c

# Construir la variable SRCS
SRCS="SRCS= "
for i in "${!SRCS_LIST[@]}"; do
    file="${SRCS_LIST[$i]}"
    SRCS+="$file "
done
SRCS="${SRCS% }"  # Quita el espacio final

# Generar Makefile
cat > Makefile <<EOL
CC      = $CC
CFLAGS  = $CFLAGS
INCLUDES= $INCLUDES

OBJ_DIR = $OBJ_DIR
NAME    = $NAME

$SRCS

OBJS = \$(SRCS:%.c=\$(OBJ_DIR)/%.o)

EOL

# Si hay un subdirectorio, añadimos reglas de subsystem
if [ -n "$SUBSYSTEM_PATH" ]; then
cat >> Makefile <<EOL
SUBSYSTEM_PATH = $SUBSYSTEM_PATH
SUBSYSTEM_LIB  = $SUBSYSTEM_LIB

all: subsystems \$(NAME)

subsystems:
	\$(MAKE) -C \$(SUBSYSTEM_PATH) all

\$(NAME): \$(OBJS) \$(SUBSYSTEM_LIB)
	\$(CC) \$(CFLAGS) \$(INCLUDES) -o \$@ \$^
EOL
else
cat >> Makefile <<EOL
all: \$(NAME)

\$(NAME): \$(OBJS)
	\$(CC) \$(CFLAGS) \$(INCLUDES) -o \$@ \$^
EOL
fi

# Reglas comunes
cat >> Makefile <<'EOL'

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
EOL

if [ -n "$SUBSYSTEM_PATH" ]; then
cat >> Makefile <<EOL
	\$(MAKE) -C \$(SUBSYSTEM_PATH) clean
	rm -rf \$(OBJ_DIR)
EOL
else
cat >> Makefile <<EOL
	rm -rf \$(OBJ_DIR)
EOL
fi

cat >> Makefile <<'EOL'

fclean: clean
EOL

if [ -n "$SUBSYSTEM_PATH" ]; then
cat >> Makefile <<EOL
	\$(MAKE) -C \$(SUBSYSTEM_PATH) fclean
	rm -f \$(NAME)
EOL
else
cat >> Makefile <<EOL
	rm -f \$(NAME)
EOL
fi

cat >> Makefile <<'EOL'

re: fclean all

.PHONY: all clean fclean re subsystems
EOL

echo "Makefile generado correctamente con nombre de programa: $NAME"
if [ -n "$SUBSYSTEM_PATH" ]; then
    echo "Incluyendo subdirectorio: $SUBSYSTEM_PATH"
fi
