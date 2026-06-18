#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <climits>
using namespace std;

//objetivo: colocar N rainhas em um tabuleiro NxN
//condição: Nenhuma das rainhas pode se atacar (LINHA/DIAGONAL/COLUNA)

int qtdade_rainhas = 8;
int n = 8; //fixado em 8x8
vector<int> pos;  //pos[r] = coluna da rainha na linha r (0-based)
vector<int> colC; //contagem de rainhas por coluna
vector<int> d1C;  //diagonal principal: (r - c + n - 1)
vector<int> d2C;  //diagonal secundaria: (r + c)

// Gera números aleatórios
mt19937 rng(random_device{}());

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

vector<int> geraPosicoesrainhas(int qtd){
    vector<int> linhas_geradas;
    linhas_geradas.reserve(qtd);

    //gera vetor aleatoria
    uniform_int_distribution<int> dist_1_a_8(1, 8);

    for (int i = 0; i < qtd; i++) {
        linhas_geradas.push_back(dist_1_a_8(rng));
    }

    return linhas_geradas;


}


void desenhar_tabuleiro(int max_print_n) {
    if (n > max_print_n) {
        cout << "\n(Tabuleiro " << n << "x" << n << " nao desenhado: N muito grande. Mostrando so o vetor.)\n";
        return;
    }

    if (!validar_posicoes()) {
        cout << "\nERRO: pos[] tem valores invalidos.\n";
        return;
    }

    cout << "\nTabuleiro (" << n << "x" << n << "):\n\n";
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            cout << (pos[r] == c ? "R " : ". ");
        }
        cout << "\n";
    }
}



int main() {
    // Inicialização dos vetores dinâmicos para tamanho N = 8
    pos.assign(n, -1);
    colC.assign(n, 0);
    d1C.assign(2 * n - 1, 0);
    d2C.assign(2 * n - 1, 0);

    /* Vetor fixo de entrada (8 posições).
       O índice representa a COLUNA e o valor representa a LINHA ocupada.
       Exemplo fornecido adaptado para 8 rainhas (0-based index):
       Coluna 0 -> Linha 4
       Coluna 1 -> Linha 2
       Coluna 2 -> Linha 4
       Coluna 3 -> Linha 4
       Coluna 4 -> Linha 2
       ... preenchido com valores de exemplo até completar as 8 colunas.
    */
    vector<int> entrada_fixa = {4, 2, 4, 4, 2, 0, 7, 5}; 

    // Mapeia a entrada fixa (coluna -> linha) para a estrutura interna (linha -> coluna)
    for (int c = 0; c < n; c++) {
        int r = entrada_fixa[c];
        pos[r] = c; 
    }

    // Se alguma linha ficou vazia devido a colisões na entrada, preenchemos com uma coluna livre
    vector<bool> col_usada(n, false);
    for (int r = 0; r < n; r++) {
        if (pos[r] != -1) {
            col_usada[pos[r]] = true;
        }
    }
    for (int r = 0; r < n; r++) {
        if (pos[r] == -1) {
            for (int c = 0; c < n; c++) {
                if (!col_usada[c]) {
                    pos[r] = c;
                    col_usada[c] = true;
                    break;
                }
            }
        }
    }

    // Adiciona as rainhas iniciais ao sistema de contagem de conflitos
    for (int r = 0; r < n; r++) adicionar(r, pos[r]);

    cout << "Configuracao Inicial Gerada:";
    desenhar_tabuleiro(30);

    int tentativas = 20;
    

    uniform_int_distribution<int> dist_tabuleiro(0, n - 1);

    

    if (passos >= 0) {
        cout << "\n==========================================";
        cout << "\nSolucao encontrada em " << passos << " passos\n";
        cout << "Vetor solucao (coluna por linha): ";
        for (int r = 0; r < n; r++) cout << pos[r] << " ";
        cout << "\n";

        desenhar_tabuleiro(30);
    } else {
        cout << "\nNao convergiu dentro do limite.\n";
    }

    return 0;
}