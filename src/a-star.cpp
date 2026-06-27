#include "astar.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>
#include <chrono>
using namespace std;

vector<int> gerarEstadoAleatorio(int n, mt19937& rng) {
    vector<int> vetor(n);

    uniform_int_distribution<int> dist(0, n - 1);

    for(int i = 0; i < n; i++) {
        vetor[i] = dist(rng);
    }

    return vetor;
}


bool estadoObjetivo(const vector<int> &estado){
    return calcularHeuristica(estado) == 0;
}

void atualizarCustos(No &vizinho, No *atual){
    vizinho.g = atual->g + 1;

    vizinho.h = calcularHeuristica(vizinho.estado);

    vizinho.f = vizinho.g + vizinho.h;
}


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

int calcularHeuristica(const  vector<int> &estado){
    int N = estado.size();

    //inicialização do vetor com false
    vector<bool> conflito(N, false);

    //encontra conflitos
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            
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

    //contagem de quantas rainhas estão em conflito
    int cont_conflitos = 0;
    for (int i = 0; i < N; ++i) {
        if (conflito[i]) {
            cont_conflitos++;
        }
    }

    return cont_conflitos;
}


vector<No> gerarVizinhos(No *atual){

    vector<No> vizinhos;

    int N = atual->estado.size();

    for(int linha=0; linha<N; linha++){
        int colunaAtual = atual->estado[linha];

        for(int coluna=0; coluna<N; coluna++){
            
            if(coluna == colunaAtual){
                continue;
            }

            No novoVizinho;

            novoVizinho.estado = atual->estado;

            novoVizinho.estado[linha] = coluna;

            vizinhos.push_back(novoVizinho);
        }
    }

    return vizinhos;

}

MetricasAestrela a_estrela(int n_rainhas, vector<int>&estadoInicial){

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
    noInicial.h = calcularHeuristica(estadoInicial);
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
            resultado.solucaoInicial = estadoInicial;
            resultado.solucao = posicoesAtuais;
            resultado.nosGerados = nosGerados;
            resultado.nosExpandidos = nosExpandidos;
            return resultado;
        }
        //insere na listaFechada o estado atual
        listaFechada.insert(noAtual.estado);

        vector<No> vizinhos = gerarVizinhos(&noAtual);

        //processamento de vizinhos

        for (int i = 0; i < vizinhos.size(); i++){

            //verifica se vizinho pertence a lista fechada
            if (listaFechada.find(vizinhos[i].estado) != listaFechada.end()){
                continue;
            }
           

            atualizarCustos(vizinhos[i],&noAtual);
            listaAberta.push(vizinhos[i]);
            nosGerados++;
        }
        
        
    }
    //coleta resultados caso não tenha sido gerado nada 
    MetricasAestrela resultado;
    resultado.solucao = estadoInicial;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;
    return resultado;
}

//função para medir tempo de execução
MetricasAestrela executar_a_estrela(int n_rainhas) {
    auto inicio = chrono::high_resolution_clock::now();
    
    random_device rd;
    //semente com Hardware para garantir alta randomicidade
    unsigned int sementeGerada = rd();
    //inicializa o gerador usando essa semente capturada
    mt19937 rng(sementeGerada);

    
    vector<int> estadoInicial = gerarEstadoAleatorio(n_rainhas, rng);
    MetricasAestrela resultado = a_estrela(8,estadoInicial);    

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    resultado.tempoExecucaoMs = duracao.count();
    resultado.sementeUsada = sementeGerada;
    return resultado;
}

vector<MetricasAestrela> benchmark_Aestrela(){

    vector <MetricasAestrela> resultados;
    int n_rainhas = 8;

    for(int i = 0; i < 5; i++) {       
        MetricasAestrela resultado = executaHill_climbing(n_rainhas);
        resultados.push_back(resultado);
    }

    return resultados;
}


int main() {

    int N = 8;

    MetricasAestrela resultado = executar_a_estrela(N);

    cout << "===== RESULTADO A* =====\n\n";

    cout << "Semente: " << resultado.sementeUsada << endl;

    cout << "\nEstado inicial:\n";
    for (int valor : resultado.solucaoInicial)
        cout << valor << " ";
    cout << endl;

    desenhar_tabuleiro(resultado.solucaoInicial);

    cout << "\nEstado final:\n";
    for (int valor : resultado.solucao)
        cout << valor << " ";
    cout << endl;

    desenhar_tabuleiro(resultado.solucao);

    cout << "\nHeurística final: "
         << calcularHeuristica(resultado.solucao)
         << endl;

    cout << "\nNós gerados: " << resultado.nosGerados << endl;
    cout << "Nós expandidos: " << resultado.nosExpandidos << endl;
    cout << "Tempo: " << resultado.tempoExecucaoMs << " ms" << endl;

    if (estadoObjetivo(resultado.solucao))
        cout << "\nSolução encontrada!\n";
    else
        cout << "\nNenhuma solução encontrada.\n";

    return 0;
}