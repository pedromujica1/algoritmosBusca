# algoritmosBusca
Implementação de Algoritmos de Busca para a matéria de Inteligência Artificial


Como rodar servidor de API
```bash
g++ main.cpp a-star.cpp hill_climbing.cpp -o servidor -std=c++17 -lpthread -lboost_system
```

Para testar Hill-Climbing
```bash
http://127.0.0.1:18080/hill-climbing/8
```

Para testar A*
```bash
http://127.0.0.1:18080/a-estrela/8
```

