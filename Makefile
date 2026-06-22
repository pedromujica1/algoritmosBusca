#Nome do executável final
TARGET = servidor

#compilador utilizado
CXX = g++

#flags de compilação (-Ilib adiciona a pasta lib para busca de headers)
CXXFLAGS = -std=c++17 -Ilib -Wall -Wextra

#bibliotecas necessárias para a ligação (linker)
LIBS = -lpthread -lboost_system

#arquivos fonte (.cpp)
SRCS = main.cpp src/a-star.cpp src/hill_climbing.cpp

#regra padrão
all:
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LIBS)

#regra para limpar o executável gerado anteriormente
clean:
	rm -f $(TARGET)