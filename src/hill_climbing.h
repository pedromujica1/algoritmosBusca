#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

#include <vector>
using namespace std;

struct MetricasHillClimbing {
    int seed;
    vector<int> solucaoInicial;
    vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    int reinicios;
    long long tempoExecucaoMs;
};

// Função principal que executa o algoritmo completo
MetricasHillClimbing executaHill_climbing(int n_rainhas, int seed = 42);
vector<MetricasHillClimbing> benchmarkHill_climbing();

#endif