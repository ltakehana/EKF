# Compilador
CC = gcc

# Flags de compilação
LDFLAGS = -lm  -lnlopt# Linkar a biblioteca matemática

# Nome do executável
TARGET = ekf_simulator

# Arquivos fonte
SRCS = main.c ekf.c battery_model.c

# Arquivos objeto (gerados automaticamente)
OBJS = $(SRCS:.c=.o)

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Regra para compilar cada arquivo fonte em um objeto
%.o: %.c
	$(CC) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Recompilar tudo
rebuild: clean all