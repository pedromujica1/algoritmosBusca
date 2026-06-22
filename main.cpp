#define CROW_MAIN
#include "lib/crow_all.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>
#include <chrono>
//incluindo arquivo de cabeçalho dos dois algoritmos
#include "src/astar.h"
#include "src/hill_climbing.h"
using namespace std;

int main(){
    //definindo uma instancia app
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        crow::json::wvalue resposta;
        resposta["mensagem"] = "Olá! Bem-vindo ao meu servidor dos algoritmos de busca!";
        resposta["status"] = "sucesso";
        return resposta;
    });

    //Dentro do escopo do servidor Crow...
    CROW_ROUTE(app, "/a-estrela/<int>")([](int n){
        crow::json::wvalue resposta;

        if (n <= 0 || n > 15) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, insira um N válido.";
            return resposta;
        }

        // Chama a nova função isolada que faz tudo!
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

        if (n <= 3 || n > 20) { 
            resposta["status"] = "erro";
            resposta["mensagem"] = "Por favor, use um N válido (ex: entre 4 e 20).";
            return resposta;
        }

        // Executa a busca estruturada
        MetricasHillClimbing resultado = executaHill_climbing(n);

        resposta["status"] = "sucesso";
        resposta["n_rainhas"] = n;
        resposta["solucao"] = resultado.solucao;
        resposta["metricas"]["nos_gerados"] = resultado.nosGerados;
        resposta["metricas"]["nos_expandidos"] = resultado.nosExpandidos;
        resposta["metricas"]["reinicios"] = resultado.reinicios;
        resposta["metricas"]["tempo_execucao_ms"] = resultado.tempoExecucaoMs;

        return resposta;
    });

    app.port(8000).multithreaded().run();
    return 0;
}