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
//incluindo arquivo de cabeçalho dos dois algoritmos
#include "src/astar.h"
#include "src/hill_climbing.h"


//função que converte para json
crow::json::wvalue converteParaJson(const MetricasHillClimbing &resultado){

    crow::json::wvalue item;

    item["estado_inicial"] = resultado.solucaoInicial;
    item["solucao"] = resultado.solucao;
    item["seed"] = resultado.seed;
    item["tempo_ms"] = resultado.tempoExecucaoMs;
    item["nos_gerados"] = resultado.nosGerados;
    item["nos_expandidos"] = resultado.nosExpandidos;
    item["reinicios"] = resultado.reinicios;

    return item;
}

int main(){
    //definindo uma instancia app
    crow::SimpleApp app;

    auto& cors = app.get_middleware<crow::CORSHandler>();

    //configura CORS para permitir requisições no FRONT-END
    //permite todas as origens, metódos POST/GET/PUT/DELETE e header de Content-type  permitidos
    cors.global().origin("*").methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method).headers("Content-Type", "Authorization"); 

    CROW_ROUTE(app, "/estado-inicial/<int>")([](int n){
        
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
    CROW_ROUTE(app, "/a-estrela/<int>").methods(crow::HTTPMethod::POST)([](const crow::request& req)([](int n){
        crow::json::wvalue resposta;

        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, insira um N válido.";
            return resposta;
        }

        auto dados_recebidos = crow::json::load(req.body);
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
        
        int n = estadoInicial.size();
        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Tamanho do tabuleiro inválido.";
            return resposta;
        }

        //chamada da função modificada
        MetricasAestrela resultado = executar_a_estrela(n, estadoInicial); 

        resposta["status"] = "sucesso";
        resposta["n_rainhas"] = n;
        resposta["solucao_inicial"] = resultado.solucaoInicial;
        resposta["solucao"] = resultado.solucao;
        resposta["metricas"]["nos_gerados"] = resultado.nosGerados;
        resposta["metricas"]["nos_expandidos"] = resultado.nosExpandidos;
        resposta["metricas"]["tempo_execucao_ms"] = resultado.tempoExecucaoMs;
        resposta["semente_registro"] = resultado.sementeUsada;

        return resposta;
    });

    CROW_ROUTE(app, "/hill-climbing/<int>").methods(crow::HTTPMethod::POST)([](const crow::request& req)([](int n){
        //objeto json do crow
        crow::json::wvalue resposta;

        //parse do Json enviado pelo front end
        auto dados_recebidos = crow::json::load(req.body);
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
        
        int n = estadoInicial.size();

        // Executa a nossa função refatorada passando o vetor recebido
        MetricasHillClimbing resultado = executaHill_climbing(n, estadoInicial);
        
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

       vector<MetricasHillClimbing> resultados = benchmarkHill_climbing();
        //objeto json
        crow::json::wvalue resposta;
        resposta["status"] = "sucesso";

        crow::json::wvalue::list lista;

        for(const MetricasHillClimbing &resultado : resultados){
            //conversao do json
            lista.push_back(converteParaJson(resultado));
        }

        resposta["resultados"] = move(lista);

        return resposta;
    });

  
    CROW_ROUTE(app, "/a-estrela/benchmark")([](){
        // Roda os 5 testes isolados com geração própria de estados
        vector<MetricasAestrela> resultados = benchmark_Aestrela();

        crow::json::wvalue resposta;
        resposta["status"] = "sucesso";

        crow::json::wvalue::list lista;

        for(const MetricasAestrela &resultado : resultados){
            // Usa a sua função utilitária de conversão (certifique-se que ela aceita MetricasAestrela)
            lista.push_back(converteParaJson(resultado));
        }

        resposta["resultados"] = std::move(lista);

        return resposta;
    });



    app.port(8000).multithreaded().run();
    return 0;
}