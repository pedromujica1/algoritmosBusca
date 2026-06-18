#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <climits>

//objetivo: coloca N rainhas um NxN tabuleiro (matriz)
//condição: Nenhuma das rainhas pode se atacar / não podem compartilhar LINHA/DIAGONAL/COLUNA
//conflito: rainhas não podem compartillhar linha/coluna e diagonal


/*
================================================================================
   HEURÍSTICA UTILIZADA: ALGORITMO DOS MÍNIMOS CONFLITOS (MIN-CONFLICTS)
================================================================================
   O código resolve o problema das N-Rainhas utilizando uma heurística de busca
   local chamada "Mínimos Conflitos". A premissa é simples e extremamente eficiente
   para problemas de Satisfação de Restrições (CSP):

   1. Seleção da Variável: Escolhe-se, de forma aleatória, uma rainha que esteja
      atualmente sofrendo algum conflito (ataque de outra rainha).
   2. Escolha do Valor (Heurística): Para essa rainha escolhida, avalia-se todas
      as colunas possíveis na sua linha atual. Movemos a rainha para a coluna
      que resultar no MENOR número de conflitos possíveis. Se houver empate, 
      escolhe-se uma entre as melhores de forma aleatória.

   -----------------------------------------------------------------------------
   MAPEAMENTO DAS DIAGONAIS EM TEMPO O(1):
   -----------------------------------------------------------------------------
   Para contar os conflitos instantaneamente sem varrer o tabuleiro, o algoritmo
   utiliza vetores de contagem para colunas e para as duas diagonais.

   * Diagonal Principal (Ido do topo-esquerdo para o fundo-direito):
     Em uma matriz, a diferença entre a linha e a coluna (r - c) é CONSTANTE 
     para qualquer célula na mesma diagonal principal.
     Como (r - c) pode variar de -(N-1) até (N-1), somamos (N - 1) para deslocar
     os valores para um índice de vetor válido (0 até 2N-2).
     Fórmula: d1 = r - c + (N - 1)

   * Diagonal Secundária (Indo do topo-direito para o fundo-esquerdo):
     A soma da linha e da coluna (r + c) é CONSTANTE para qualquer célula na 
     mesma diagonal secundária.
     Os valores variam de 0 até 2N-2.
     Fórmula: d2 = r + c
================================================================================
*/

int n;
std::vector<int> pos;  // pos[r] = coluna da rainha na linha r (0-based)
std::vector<int> colC; // contagem de rainhas por coluna
std::vector<int> d1C;  // diagonal principal: (r - c + n - 1)
std::vector<int> d2C;  // diagonal secundaria: (r + c)

// Gerador de números aleatórios moderno do C++
std::mt19937 rng(std::random_device{}());

inline int d1(int r, int c) { return r - c + (n - 1); }
inline int d2(int r, int c) { return r + c; }

int conflitos_linha(int r) {
    int c = pos[r];
    return (colC[c] - 1) + (d1C[d1(r, c)] - 1) + (d2C[d2(r, c)] - 1);
}

int conflitos_se_colocar(int r, int c) {
    return colC[c] + d1C[d1(r, c)] + d2C[d2(r, c)];
}

void remover(int r, int c) {
    colC[c]--;
    d1C[d1(r, c)]--;
    d2C[d2(r, c)]--;
}

void adicionar(int r, int c) {
    colC[c]++;
    d1C[d1(r, c)]++;
    d2C[d2(r, c)]++;
}

bool validar_posicoes() {
    for (int r = 0; r < n; r++) {
        if (pos[r] < 0 || pos[r] >= n) return false;
    }
    return true;
}

void desenhar_tabuleiro(int max_print_n) {
    if (n > max_print_n) {
        std::cout << "\n(Tabuleiro " << n << "x" << n << " nao desenhado: N muito grande. Mostrando so o vetor.)\n";
        return;
    }

    if (!validar_posicoes()) {
        std::cout << "\nERRO: pos[] tem valores invalidos (provavelmente desenhando antes da solucao).\n";
        for (int r = 0; r < n; r++) {
            std::cout << "Linha " << r << " -> " << pos[r] << "\n";
        }
        return;
    }

    std::cout << "\nTabuleiro (" << n << "x" << n << "):\n\n";
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            std::cout << (pos[r] == c ? "R " : ". ");
        }
        std::cout << "\n";
    }
}

int min_conflicts(int max_passos) {
    for (int passo = 0; passo < max_passos; passo++) {
        
        // Lista de linhas conflituosas
        std::vector<int> linhas;
        linhas.reserve(n);

        for (int r = 0; r < n; r++) {
            if (conflitos_linha(r) > 0) {
                linhas.push_back(r);
            }
        }

        // Se não há conflitos, o problema está resolvido
        if (linhas.empty()) { 
            return passo;
        }

        // Escolhe uma linha conflituosa aleatoriamente
        std::uniform_int_distribution<int> dist_linhas(0, static_cast<int>(linhas.size()) - 1);
        int r = linhas[dist_linhas(rng)];

        int atual = pos[r];
        remover(r, atual);

        int melhorConf = INT_MAX;
        std::vector<int> cands;
        cands.reserve(n);

        // Avalia a heurística para cada coluna da linha escolhida
        for (int c = 0; c < n; c++) {
            int conf = conflitos_se_colocar(r, c);
            if (conf < melhorConf) {
                melhorConf = conf;
                cands.clear();
                cands.push_back(c);
            } else if (conf == melhorConf) {
                cands.push_back(c);
            }
        }

        // Escolhe aleatoriamente entre as colunas com a menor quantidade de conflitos
        std::uniform_int_distribution<int> dist_cands(0, static_cast<int>(cands.size()) - 1);
        int melhor = cands[dist_cands(rng)];

        pos[r] = melhor;
        adicionar(r, melhor);
    }

    return -1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " entrada.txt\n";
        return 1;
    }

    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << argv[1] << "\n";
        return 1;
    }

    if (!(f >> n) || n < 1) {
        std::cout << "Arquivo invalido: nao li N.\n";
        return 1;
    }

    if (n == 2 || n == 3) {
        std::cout << "Sem solucao para N=2 ou N=3.\n";
        return 0;
    }

    // Inicialização dos vetores dinâmicos do C++ (alocação e limpeza automáticas)
    pos.assign(n, -1);
    colC.assign(n, 0);
    d1C.assign(2 * n - 1, 0);
    d2C.assign(2 * n - 1, 0);

    int minv = INT_MAX;
    int maxv = INT_MIN;

    for (int i = 0; i < n; i++) {
        if (!(f >> pos[i])) {
            std::cout << "Arquivo invalido: faltou valor na segunda linha (esperava " << n << ").\n";
            return 1;
        }
        if (pos[i] < minv) minv = pos[i];
        if (pos[i] > maxv) maxv = pos[i];
    }
    f.close();

    // Consertar se a entrada for baseada em 1 (1-based index) em vez de 0
    if (minv >= 1 && maxv <= n) {
        for (int i = 0; i < n; i++) pos[i]--;
    }

    if (!validar_posicoes()) {
        std::cout << "Entrada contem coluna fora de 0.." << n - 1 << ". Verifique o arquivo.\n";
        return 1;
    }

    for (int r = 0; r < n; r++) adicionar(r, pos[r]);

    int tentativas = 20;
    int max_passos = 200000;
    int passos = -1;

    std::uniform_int_distribution<int> dist_tabuleiro(0, n - 1);

    for (int t = 0; t < tentativas && passos < 0; t++) {
        passos = min_conflicts(max_passos);

        if (passos < 0) {
            std::fill(colC.begin(), colC.end(), 0);
            std::fill(d1C.begin(), d1C.end(), 0);
            std::fill(d2C.begin(), d2C.end(), 0);

            for (int r = 0; r < n; r++) {
                pos[r] = dist_tabuleiro(rng);
                adicionar(r, pos[r]);
            }
        }
    }

    if (passos >= 0) {
        std::cout << "\nSolucao encontrada em " << passos << " passos\n";
        std::cout << "Vetor solucao (coluna por linha): ";
        for (int r = 0; r < n; r++) std::cout << pos[r] << " ";
        std::cout << "\n";

        // Definir o tamanho máximo para exibição gráfica do tabuleiro
        const int MAX_PRINT_N = 30;
        desenhar_tabuleiro(MAX_PRINT_N);
    } else {
        std::cout << "\nNao convergiu dentro do limite.\n";
    }

    return 0;
}
