#include "Floresta.hpp"
#include "config.hpp"
#include <iostream>

Floresta::Floresta(int n, int m) : nLinhas(n), nColunas(m) 
{
    matriz.resize(nLinhas, std::vector<int>(nColunas, 0));
}

void Floresta::exibir() const 
{
    for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            std::cout << matriz[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Uma árvore saudável (1) se torna em chamas (2) se houver vizinho em chamas.
// Uma árvore em chamas (2) se torna queimada (3) após o ciclo.
void Floresta::atualizarFogo() 
{
    std::vector<std::vector<int>> novaMatriz = matriz;

    for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            if (matriz[i][j] == 1) 
            { 
                bool vizinhoEmFogo = false;
                std::vector<std::pair<int, int>> direcoes;
                
                if (VENTO_ATIVO) 
                {
                    for (auto dir : direcoesVento) 
                    {
                        switch (dir) 
                        {
                            case CIMA:    direcoes.push_back({-1, 0}); break;
                            case BAIXO:   direcoes.push_back({1, 0}); break;
                            case ESQUERDA: direcoes.push_back({0, -1}); break;
                            case DIREITA: direcoes.push_back({0, 1}); break;
                        }
                    }
                }
                else 
                {
                    direcoes = { {-1,0}, {1,0}, {0,-1}, {0,1} };
                }
                for (auto d : direcoes) 
                {
                    int ni = i + d.first;
                    int nj = j + d.second;
                    
                    if (posicaoValida(ni, nj) && matriz[ni][nj] == 2) 
                    {
                        vizinhoEmFogo = true;
                        break;
                    }
                }
                if (vizinhoEmFogo)
                {
                    novaMatriz[i][j] = 2;
                }
            } 
            else if (matriz[i][j] == 2) 
            { 
                novaMatriz[i][j] = 3;
            }
        }
    }
    matriz = novaMatriz;
}

int Floresta::obterValor(int linha, int coluna) const 
{
    if (posicaoValida(linha, coluna))
    {
        return matriz[linha][coluna];
    }
    return -1;
}

void Floresta::definirValor(int linha, int coluna, int valor) 
{
    if (posicaoValida(linha, coluna))
    {
        matriz[linha][coluna] = valor;
    }
}

int Floresta::linhas() const 
{
    return nLinhas;
}

int Floresta::colunas() const 
{
    return nColunas;
}

bool Floresta::posicaoValida(int linha, int coluna) const 
{
    return (linha >= 0 && linha < nLinhas && coluna >= 0 && coluna < nColunas);
}

const std::vector<std::vector<int>>& Floresta::getMatriz() const 
{
    return matriz;
}

std::vector<std::vector<int>>& Floresta::getMatriz() 
{
    return matriz;
}
