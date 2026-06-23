#define CROW_MAIN
#include "lib/crow_all.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>
#include <chrono>
using namespace std;
//incluindo arquivo de cabeçalho dos dois algoritmos
#include "src/astar.h"
#include "src/hill_climbing.h"

int main(){
    //definindo uma instancia app
    crow::SimpleApp app;

    //rota para executar a-estrela.
    CROW_ROUTE(app, "/a-estrela/<int>")([](int n){
        crow::json::wvalue resposta;

        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, insira um N válido.";
            return resposta;
        }

        //chamada de função
        MetricasAestrela resultado = executar_a_estrela(n); 

        resposta["status"] = "sucesso";
        resposta["n_rainhas"] = n;
        resposta["solucao"] = resultado.solucao;
        resposta["metricas"]["nos_gerados"] = resultado.nosGerados;
        resposta["metricas"]["nos_expandidos"] = resultado.nosExpandidos;
        resposta["metricas"]["tempo_execucao_ms"] = resultado.tempoExecucaoMs;

        return resposta;
    });

    CROW_ROUTE(app, "/hill-climbing/<int>")([](int n){
        crow::json::wvalue resposta;

        if (n <= 3 || n > 8) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, use um N válido (ex: entre 4 e 20).";
            return resposta;
        }

        //executa a busca estruturada
        MetricasHillClimbing resultado = executaHill_climbing(n);

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

        //auto é usado para dados complexos
        auto resultados = benchmarkHill_climbing();
        crow::json::wvalue resposta;
        resposta["status"] = "sucesso";

        //lista em json para salvar tudo
        crow::json::wvalue::list lista;

        for(const auto &resultado : resultados){

            crow::json::wvalue item;

            item["estado_inicial"] = resultado.solucaoInicial;
            item["solucao"] = resultado.solucao;
            item["seed"] = resultado.seed;
            item["tempo_ms"] = resultado.tempoExecucaoMs;
            item["nos_gerados"] = resultado.nosGerados;
            item["nos_expandidos"] = resultado.nosExpandidos;
            item["reinicios"] = resultado.reinicios;

            lista.push_back(move(item));
        }

        resposta["resultados"] = move(lista);

        return resposta;
    });



    app.port(8000).multithreaded().run();
    return 0;
}