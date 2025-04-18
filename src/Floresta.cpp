#include "Floresta.hpp"
#include "config.hpp"
#include <iostream>
#include <cstdlib>   
#include <thread>    
#include <chrono>    

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

    std::cout << "\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Floresta::atualizarFogo() 
{
    static int iter = 0;
    ++iter;

    std::vector<std::vector<int>> novaMatriz = matriz;

    if (fireGen.empty()) 
    {
        fireGen.resize(nLinhas, std::vector<int>(nColunas, -1));
    }

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
                            case CIMA:    direcoes.push_back({1, 0}); break;
                            case BAIXO:   direcoes.push_back({-1, 0});  break;
                            case ESQUERDA:direcoes.push_back({0, 1});  break;
                            case DIREITA: direcoes.push_back({0, -1});  break;
                        }
                    }
                }
                else 
                {
                    direcoes = {{-1,0},{1,0},{0,-1},{0,1}};
                }

                for (auto &d : direcoes) 
                {
                    int ni = i + d.first, nj = j + d.second;
                   
                    if (posicaoValida(ni,nj) && matriz[ni][nj] == 2) 
                    {
                        vizinhoEmFogo = true;
                        
                        break;
                    }
                }

                if (vizinhoEmFogo) 
                {
                    novaMatriz[i][j] = 2;
                    fireGen[i][j] = iter; 
                }
            }
        }
    }

    for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            if (matriz[i][j] == 2 && fireGen[i][j] <= iter - 2) 
            {
                novaMatriz[i][j] = 3;
            }
        }
    }

    matriz = std::move(novaMatriz);
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

bool Floresta::todaQueimada() const 
{
    for (const auto &linha : matriz) 
    {
        for (int valor : linha) 
        {
         
            if (valor == 2) 
            {
                return false;
            }
        }
    }
   
    return true;
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
