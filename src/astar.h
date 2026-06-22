#ifndef ASTAR_H
#define ASTAR_H
#include <vector>

struct No {
    std::vector<int> estado;
    int g;
    int h;
    int f;
};

struct MetricasAestrela {
    std::vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    long long tempoExecucaoMs; //adicionado para o Crow
};

//FUNÇÃO Q executa o algoritmo
MetricasAestrela executar_a_estrela(int n_rainhas);
#endif