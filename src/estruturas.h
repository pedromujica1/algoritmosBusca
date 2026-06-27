#ifndef ASTAR_H
#define ASTAR_H
#include <vector>
using namespace std;

//esturtura do No
struct No{
    vector<int> estado; //vetor com as colunas em que cada rainha está
    int g; //custo para chegar em determinado estado
    int h; //representa conflitos futuros
    int f; //soma de g + f
};

//comparador para fila de prioridade
struct Comparador{
    bool operator()(const No &a,const No &b){
        //prioriidade para nó mais profundo
        if (a.f == b.f) {
            return a.g < b.g; 
        }
        return a.f > b.f;
    }
};

struct MetricasBusca {
    vector<int> solucaoInicial;
    vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    long long tempoExecucaoMs;
    unsigned int sementeUsada;
    int reiniciosHill;
};

//FUNÇÃO Q executa o algoritmo A*
vector<int> gerarEstadoAleatorio(int n, mt19937 &rng);
MetricasBusca executarA_star(int n_rainhas, const vector<int> &estadoInicial);
vector<MetricasBusca> benchmark_Aestrela();
//função que executa Hill Climbing
MetricasBusca executaHill_climbing(int n_rainhas);
vector<MetricasBusca> benchmarkHill_climbing();


#endif