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
bool posicao_valida(const vector<int> &estado, int linha, int coluna){

    int tamanho_estados = estado.size(); //qtdade colunas preenchidas
  
    for (int i = 0; i < tamanho_estados; i++){
        int linhaRainha = estado[i]; 
        
        //verificação da linha
        if (linhaRainha == linha){
            return false;
        }
        //verificação da diagonal
        if (abs(coluna - i) == abs(linha - linhaRainha)){
            return false;
        }  
    }
    return true;
}

int conflitos_futuros(int N, const vector<int> &estado){
    int conflitos = 0;
    
    //primeira coluna vazia do tabuleiro
    int colunaAtual = estado.size();

    //apenas as colunas futuras que ainda nao possuem rainhas
    for (int coluna = colunaAtual; coluna < N; coluna++){
      
        for (int linha = 0; linha < N; linha++){
            
            if(!posicao_valida(estado, linha, coluna)){
                conflitos += 1;
            }
        }  
    }
    return conflitos;
}


MetricasAestrela a_estrela(int n_rainhas, const vector<int>&estadoInicial){

    //variaveis de metrica
    long long nosGerados = 1;      // nó inicial
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
        if(posicoesAtuais.size() == n_rainhas){
            //coleta resultados
            MetricasAestrela resultado;

            resultado.solucao = posicoesAtuais;
            resultado.nosGerados = nosGerados;
            resultado.nosExpandidos = nosExpandidos;

            return resultado;
        }
        //insere na listaFechada o estado atual
        listaFechada.insert(noAtual.estado);

        //COLUNA ESCOLHIDA PARA VERIFICAR
        int coluna = posicoesAtuais.size();

        //laço agora itera testando as LINHAS
        for (int linha = 0; linha < n_rainhas; linha++){

            No noFilho;

            //VERIFICA SE POSICAO É VALIDA PARA GERAR FILHOS
            if(posicao_valida(noAtual.estado, linha, coluna)){
                
                noFilho.estado = noAtual.estado;
                //Adiciona a LINHA escolhida no vetor daquela COLUNA
                noFilho.estado.push_back(linha);

                //Se filho não está na listaFechada
                if(listaFechada.find(noFilho.estado) == listaFechada.end()){
                    noFilho.g = noAtual.g + 1;
                    noFilho.h = conflitos_futuros(n_rainhas, noFilho.estado);
                    noFilho.f = noFilho.g + noFilho.h;

                    listaAberta.push(noFilho);
                    nosGerados++;
                }
            }  
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