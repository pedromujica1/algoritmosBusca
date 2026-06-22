#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

#include <vector>

struct MetricasHillClimbing {
    std::vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    int reinicios;
    long long tempoExecucaoMs;
};

// Função principal que executa o algoritmo completo
MetricasHillClimbing executaHill_climbing(int n_rainhas);

#endif