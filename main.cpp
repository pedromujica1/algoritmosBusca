#define CROW_MAIN
#include "lib/crow_all.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>
#include <chrono>
#include <crow/middlewares/cors.h>
using namespace std;
using namespace crow;
//incluindo arquivo de cabeçalho dos dois algoritmos
#include "src/estruturas.h"


json::wvalue converteParaJson(const MetricasBusca &resultado, int id_algoritmo){
    json::wvalue item;
    item["estado_inicial"] = resultado.solucaoInicial;
    item["solucao"] = resultado.solucao;
    item["seed"] = resultado.sementeUsada;
    item["tempo_ms"] = resultado.tempoExecucaoMs;
    item["nos_gerados"] = resultado.nosGerados;
    item["nos_expandidos"] = resultado.nosExpandidos;
    
    //siginifica que está rodando o A*
    if(id_algoritmo  == 1){
        item["reinicios"] = 0; //A* não possui reinícios
    }
    else{//siginifica que está rodando o Hill Climbing
        item["reinicios"] = resultado.reiniciosHill; 
    }
    
    return item;
}

int main(){
    //definindo uma instancia app
    SimpleApp app;

    auto &cors = app.get_middleware<CORSHandler>();

    //configura CORS para permitir requisições no FRONT-END
    //permite todas as origens, metódos POST/GET/PUT/DELETE e header de Content-type  permitidos
    cors.global().origin("*").methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method).headers("Content-Type", "Authorization"); 

    CROW_ROUTE(app, "/estado-inicial/<int>")([](int n){

        //corpo JSON
        json::wvalue resposta;
        
        //verificação de erro
        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, insira um N válido.";
            return resposta;
        }

        //inicialização segura do gerador de números aleatórios via Hardware
        random_device rd;
        mt19937 rng(rd());
        //gerando estado inicial aleatorio
        vector<int> estadoAleatorio = gerarEstadoAleatorio(n, rng); 

        //monta JSON
        resposta["n_rainhas"] = n;
        resposta["estado_inicial"] = estadoAleatorio; // O Crow converte std::vector automaticamente para array JSON

        return resposta;
    });


    //rota para executar a-estrela.
    //recebe vetor json como entrada
    CROW_ROUTE(app, "/a-estrela/<int>").methods(HTTPMethod::POST)([](const request& req, int n_rainhas){
        json::wvalue resposta;

        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, insira um N válido.";
            return resposta;
        }

        auto dados_recebidos = json::load(req.body);
        if (!dados_recebidos || !dados_recebidos.has("estado_inicial")) {
            resposta["status"] = "erro";
            resposta["mensagem"] = "JSON inválido ou campo 'estado_inicial' ausente.";
            return resposta;
        }

        //extrai o vetor enviado do front-end
        vector<int> estadoInicial;
        for (const auto& item : dados_recebidos["estado_inicial"]) {
            estadoInicial.push_back(item.i());
        }
        
        //chamada da função modificada
        MetricasBusca resposta = executarA_star(n_rainhas,estadoInicial); 

        resposta["status"] = "sucesso";
        resposta["n_rainhas"] = n;
        resposta["solucao_inicial"] = resultado.solucaoInicial;
        resposta["solucao"] = resultado.solucao;
        resposta["metricas"]["nos_gerados"] = resultado.nosGerados;
        resposta["metricas"]["nos_expandidos"] = resultado.nosExpandidos;
        resposta["metricas"]["tempo_execucao_ms"] = resultado.tempoExecucaoMs;
        resposta["reinicios"] = 0;
        resposta["semente_registro"] = resultado.sementeUsada;

        return resposta;
    });


    CROW_ROUTE(app, "/hill-climbing/<int>").methods(HTTPMethod::POST)([](const request& req, int n_rainhas){
        //objeto json do crow
        json::wvalue resposta;

        //parse do Json enviado pelo front end
        auto dados_recebidos = json::load(req.body);
        if (!dados_recebidos || !dados_recebidos.has("estado_inicial")) {
            resposta["status"] = "erro";
            resposta["mensagem"] = "JSON inválido ou campo 'estado_inicial' ausente.";
            return resposta;
        }

        //converte array do json para vector
        vector<int> estadoInicial;
        for (const auto& item : dados_recebidos["estado_inicial"]) {
            estadoInicial.push_back(item.i());
        }
        
        // Executa a nossa função refatorada passando o vetor recebido
        MetricasBusca resultado = executaHill_climbing(n_rainhas, estadoInicial);
        
        resposta["status"] = "sucesso";
        resposta["estado_inicial"] = resultado.solucaoInicial;
        resposta["n_rainhas"] = n;
        resposta["solucao"] = resultado.solucao;
        resposta["metricas"]["nos_gerados"] = resultado.nosGerados;
        resposta["metricas"]["nos_expandidos"] = resultado.nosExpandidos;
        resposta["metricas"]["reinicios"] = resultado.reinicios;
        resposta["metricas"]["tempo_execucao_ms"] = resultado.tempoExecucaoMs;

        return resposta;
    });

    CROW_ROUTE(app, "/hill-climbing/benchmark")([](){

       vector<MetricasBusca> resultados = benchmarkHill_climbing();
        //objeto json
        json::wvalue resposta;
        resposta["status"] = "sucesso";

        json::wvalue::list lista;

        for(const MetricasBusca &resultado : resultados){
            //conversao do json
            lista.push_back(converteParaJson(resultado,0));
        }

        resposta["resultados"] = move(lista);

        return resposta;
    });

  
    CROW_ROUTE(app, "/a-estrela/benchmark")([](){
        // Roda os 5 testes isolados com geração própria de estados
        vector<MetricasBusca> resultados = benchmark_Aestrela();

        json::wvalue resposta;
        resposta["status"] = "sucesso";

        json::wvalue::list lista;

        for(const MetricasBusca &resultado : resultados){
            // Usa a sua função utilitária de conversão (certifique-se que ela aceita MetricasAestrela)
            lista.push_back(converteParaJson(resultado,1));
        }

        resposta["resultados"] = std::move(lista);

        return resposta;
    });



    app.port(8000).multithreaded().run();
    return 0;
}