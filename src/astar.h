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
//stuct para guardas as metricas
struct MetricasAestrela{
    vector<int> solucaoInicial;
    vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    long long tempoExecucaoMs;
    int reinicios;
    unsigned int sementeUsada;
};

//FUNÇÃO Q executa o algoritmo
MetricasAestrela executar_a_estrela(int n_rainhas);
#endif