#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

#include <vector>
using namespace std;

struct MetricasHillClimbing {
    vector<int> solucaoInicial;
    vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    long long tempoExecucaoMs;
    int reinicios;
    unsigned int sementeUsada;
};

// Função principal que executa o algoritmo completo
MetricasHillClimbing executaHill_climbing(int n_rainhas);
vector<MetricasHillClimbing> benchmarkHill_climbing();

#endif