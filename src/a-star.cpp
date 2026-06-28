#include "estruturas.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <map>
#include <chrono>
using namespace std;
//g++ a-star.cpp estruturas.h -o a-star

vector<int> gerarEstadoAleatorio(int n, mt19937 &rng) {
    vector<int> vetor(n);

    uniform_int_distribution<int> dist(0, n - 1);

    for(int i = 0; i < n; i++) {
        vetor[i] = dist(rng);
    }

    return vetor;
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

int contarRainhasConflitantes(const  vector<int> &estado){
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

//estado objetivo quando existirem 0 RAINHAS COM CONFLITO
bool estadoObjetivo(const vector<int> &estado){
    return contarRainhasConflitantes(estado) == 0;
}

vector<No> gerarVizinhos(No *atual){
    vector<No> vizinhos;

    int N = atual->estado.size();
    //percorre cada linha
    for(int linha=0; linha<N; linha++){
        //descobre a coluna atual;
        int colunaAtual = atual->estado[linha];

        //percorre pelas colunas da linha
        for(int novaColuna=0; novaColuna<N; novaColuna++){
            
            //pula a coluna que rainha já está colocada
            if(novaColuna == colunaAtual){
                continue;
            }
      
            //geramos um novo vizinho posicionado em uma nova coluna
            No novoVizinho;

            novoVizinho.estado = atual->estado;

            novoVizinho.estado[linha] = novaColuna;

            vizinhos.push_back(novoVizinho);
        }
    }

    return vizinhos;
}

void atualizarCustos(No &vizinho, No *atual){
    vizinho.g = atual->g + 1;

    vizinho.h = contarRainhasConflitantes(vizinho.estado);

    vizinho.f = vizinho.g + vizinho.h;
}

MetricasBusca a_estrela(const vector<int>& estadoInicial){

    //variaveis de metrica
    long long nosGerados = 0;      // nó inicial
    long long nosExpandidos = 0;

    //FILA DE PRIORIDADE q ordena de acordo com o f
    priority_queue<No, vector<No>,Comparador> filaPrioridade;

    //LISTA FECHADA E ABERTA contendo o nó
    map<vector<int>, No> listaFechada;
    map<vector<int>, No> listaAberta;

    //NÓ INICIAL
    No noInicial;
    
    //definindo valores do estado inicial
    noInicial.estado = estadoInicial;
    noInicial.g = 0;
    noInicial.h = contarRainhasConflitantes(estadoInicial);
    noInicial.f = noInicial.g + noInicial.h;
    noInicial.paiEstado.clear();

    //inserindo estado inicial na fila de prioridade e lista aberta
    filaPrioridade.push(noInicial);
    listaAberta[noInicial.estado] = noInicial;


    while (!filaPrioridade.empty()){
        //pega o nó inicial com a menor ponutação em f
        No noAtual = filaPrioridade.top();
        //retira o nó do topo
        filaPrioridade.pop();

        //apagando da lista aberta tbm
        listaAberta.erase(noAtual.estado);

        //coloque o nó atual na lista fechada 
        listaFechada[noAtual.estado] = noAtual;

        nosExpandidos++;

        //pega nó atual que acabou de ser removido do topo da pilha
        const vector<int> &posicoesAtuais = noAtual.estado;
        
        //verifica se atingiu o estado objetivo
        //estado objetivo quando existirem 0 RAINHAS COM CONFLITO
        if(estadoObjetivo(posicoesAtuais)){
            
            //coleta resultados
            MetricasBusca resultado;
            resultado.solucaoInicial = estadoInicial;
            resultado.solucao = posicoesAtuais;
            resultado.nosGerados = nosGerados;
            resultado.nosExpandidos = nosExpandidos;
            return resultado;
        }
        

        //observe todos os seus vizinhos
        //gerar vizinhos
        vector<No>vizinhos = gerarVizinhos(&noAtual);

        //processamento de vizinhos
        //inteiro sem sinal para comparar com tamanho do vector
        for (size_t i = 0; i < vizinhos.size(); i++){

            //atualiza custos dos vizinhos
            atualizarCustos(vizinhos[i], &noAtual);

            //verifica se vizinho pertence a lista fechada
            if (listaFechada.find(vizinhos[i].estado) != listaFechada.end()){
                
                No &noFechado = listaFechada[vizinhos[i].estado];
                //substitua o vizinho pelo novo valor mais baixo de g
                if(vizinhos[i].g < noFechado.g){
                    //no atual agora é pai do vizinho
                    vizinhos[i].paiEstado = noAtual.estado;
                    
                    //atualiza estado e lista fechada
                    listaFechada[vizinhos[i].estado] = vizinhos[i];
                    //colocando novamente na lista aberta
                    listaAberta[vizinhos[i].estado] = vizinhos[i];

                    //ATUALIZA FILA PRIORIDADE
                    filaPrioridade.push(vizinhos[i]);

                }
            }
            //verifica se vizinho pertence a lista aberta
            else if (listaAberta.find(vizinhos[i].estado) != listaAberta.end()){

                No &noAberto = listaAberta[vizinhos[i].estado];

                if(vizinhos[i].g < noAberto.g){
                    //altera pai do vizinho para nó atual
                    vizinhos[i].paiEstado = noAtual.estado;
                    //atualiza lista aberta
                    listaAberta[vizinhos[i].estado] = vizinhos[i];
                    
                    //ATUALIZA FILA DE PRIORIDADE
                    filaPrioridade.push(vizinhos[i]);
                    
                    
                }
                
            }
            //caso nó não esteja em nenhuma das listas
            else{
                //atualizar o pai
                vizinhos[i].paiEstado = noAtual.estado;

                //adciona na lista aberta e na fila de prioridades 
                listaAberta[vizinhos[i].estado] = vizinhos[i];
                        
                filaPrioridade.push(vizinhos[i]);


            }
            nosGerados++;
        }

    }
    //coleta resultados caso não tenha sido gerado nada 
    MetricasBusca resultado;
    resultado.solucao = estadoInicial;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;
    return resultado;
}

//função para medir tempo de execução
MetricasBusca executarA_star(const vector<int> &estadoInicial) {
    auto inicio = chrono::high_resolution_clock::now();
    
    random_device rd;
    //semente com Hardware para garantir alta randomicidade
    unsigned int sementeGerada = rd();
    
    //chamada de função
    MetricasBusca resultado = a_estrela(estadoInicial);    

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    resultado.tempoExecucaoMs = duracao.count();
    resultado.sementeUsada = sementeGerada;
    return resultado;
}

vector<MetricasBusca> benchmark_Aestrela(){
    vector<MetricasBusca> resultados;
    int n_rainhas = 8;

    random_device rd;
    mt19937 rng(rd());

    for(int i = 0; i < 5; i++) {       
        // Cria um cenário de testes para o A*
        vector<int> estadoInicialParaTeste = gerarEstadoAleatorio(n_rainhas, rng);

        // CORRIGIDO: Estava chamando executaHill_climbing por engano!
        MetricasBusca resultado = executarA_star(estadoInicialParaTeste);
        resultados.push_back(resultado);
    }
    return resultados;
}