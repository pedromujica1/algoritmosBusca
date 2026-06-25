#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>
#include <chrono>
using namespace std;


//esturtura do No
struct No{
    vector<int> estado; //vetor das linhas em que cada rainha está
    int g; //custo para chegar em determinado estado
    int h; //representa conflitos futuros
    int f; //soma de g + f
    No *pai;
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

struct MetricasAestrela{
    vector<int> solucao;
    long long nosGerados;
    long long nosExpandidos;
    long long tempoExecucaoMs;
};

void desenhar_tabuleiro(const vector<int> &estado){
    int n = estado.size();

    cout << "\nTabuleiro (" << n << "x" << n << ")\n\n";

    for(int linha = 0; linha < n; linha++)
    {
        for(int coluna = 0; coluna < n; coluna++)
        {
            if(estado[linha] == coluna)
            {
                cout << "R ";
            }
            else
            {
                cout << ". ";
            }
        }

        cout << endl;
    }
}

int calcularHeuristica(vector<int> estado){
    int N = estado.size();

    //inicialização do vetor com false
    vector<bool> conflito(N, false);

    //encontra conflitos
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            
            //verificação de mesma linha
            if (estado[i] == estado[j]) {
                conflito[i] = true;
                conflito[j] = true;
            }

            //verificação de diagonais
            if (abs(i - j) == abs(estado[i] - estado[j])) {
                conflito[i] = true;
                conflito[j] = true;
            }
        }
    }

    //Contagem de quantas rainhas estão em conflito
    int cont_conflitos = 0;
    for (int i = 0; i < N; ++i) {
        if (conflito[i]) {
            cont_conflitos++;
        }
    }

    return cont_conflitos;
}

bool estadoObjetivo(vector<int> estado){
    return calcularHeuristica(estado) == 0;
}

void atualizarCustos(No &vizinho, No *atual){
    vizinho.g = atual->g + 1;

    vizinho.h = calcularHeuristica(vizinho.estado);

    vizinho.f = vizinho.g + vizinho.h;

    vizinho.pai = atual;
}

vector<No> gerarVizinhos(No *atual){

    vector<No> vizinhos;
    int N = atual->estado.size();

    for(int linha=0; linha<N; linha++)
    {
        int colunaAtual = atual->estado[linha];

        for(int coluna=0; coluna<N; coluna++)
        {
            if(coluna == colunaAtual)
                continue;

            No novoVizinho;

            novoVizinho.estado = atual->estado;

            novoVizinho.estado[linha] = coluna;

            vizinhos.push_back(novoVizinho);
        }
    }

    return vizinhos;

}






MetricasAestrela a_estrela(int n_rainhas, const vector<int>&estadoInicial){

    //variaveis de metrica
    long long nosGerados = 0;      // nó inicial
    long long nosExpandidos = 0;

    //FILA DE PRIORIDADE q ordena de acordo com o f
    priority_queue<No, vector<No>,Comparador> listaAberta;

    //CONJUNTO FECHADO
    set<vector<int>> listaFechada;

    //NÓ INICIAL
    No noInicial;
    
    //definindo valores do estado inicial
    noInicial.estado = estadoInicial;
    noInicial.g = 0;
    noInicial.h = conflitos_futuros(n_rainhas,estadoInicial);
    noInicial.f = noInicial.g + noInicial.h;

    //inserindo estado inicial
    listaAberta.push(noInicial);

    while (!listaAberta.empty()){
        //pega o nó inicial inserido
        No noAtual = listaAberta.top();
        //retira o nó do topo
        listaAberta.pop();
        nosExpandidos++;

        //pegando as posiçoes das rainhas do nó atual
        const vector<int>&posicoesAtuais = noAtual.estado;
        
        //encerramento do programa quando preenche as n_rainhas
        //converte em int comum para compilador não avisar erro
        if(estadoObjetivo(posicoesAtuais)){
            //coleta resultados
            MetricasAestrela resultado;

            resultado.solucao = posicoesAtuais;
            resultado.nosGerados = nosGerados;
            resultado.nosExpandidos = nosExpandidos;

            return resultado;
        }
        //insere na listaFechada o estado atual
        listaFechada.insert(noAtual.estado);

        vector<No> vizinhos = gerarVizinhos(&noAtual);

        for (int i = 0; i < vizinhos.size(); i++)
        {

            /* code */
        }
        
        

        
        

        
    }
    //cout << "Solução não encontrada!";
    //coleta resultados
    MetricasAestrela resultado;

    resultado.solucao = estadoInicial;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;

    return resultado;
}

//função para medir tempo de execução
MetricasAestrela executar_a_estrela(int n_rainhas) {
    vector<int> estadoInicial;

    //cronômetro inicia aqui
    auto inicio = chrono::high_resolution_clock::now();

    //executa busca
    MetricasAestrela resultado = a_estrela(n_rainhas, estadoInicial);

    //cronometro para aqui
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    //injeçao do tempo medido em Ms
    resultado.tempoExecucaoMs = duracao.count();

    return resultado;
}