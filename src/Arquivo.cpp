#include "Arquivo.hpp"
#include <fstream>
#include <iostream>

std::pair<int, int> Arquivo::obterDimensao(const std::string &nomeArquivo) 
{
    std::ifstream entrada(nomeArquivo);
    
    if (!entrada.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "\n";
        exit(1);
    }
    
    int n, m, fogoLinha, fogoColuna;
    entrada >> n >> m >> fogoLinha >> fogoColuna;
    entrada.close();
    
    return {n, m};
}

bool Arquivo::carregarMapa(const std::string &nomeArquivo, Floresta &floresta) 
{
    std::ifstream entrada(nomeArquivo);
    
    if (!entrada.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "\n";
       
        return false;
    }
    
    int n, m, fogoLinha, fogoColuna;
    entrada >> n >> m >> fogoLinha >> fogoColuna;

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < m; j++) 
        {
            int valor;
            entrada >> valor;
            floresta.definirValor(i, j, valor);
        }
    }
    entrada.close();
    
    return true;
}

void Arquivo::salvarMapa(const std::string &nomeArquivo, const Floresta &floresta, int iteracao) 
{
    std::ofstream saida(nomeArquivo, std::ios::app);
    
    if (!saida.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "\n";
        
        return;
    }
    saida << "Iteração " << iteracao << ":\n";
    
    for (int i = 0; i < floresta.linhas(); i++) 
    {
        for (int j = 0; j < floresta.colunas(); j++) 
        {
            saida << floresta.obterValor(i, j) << " ";
        }
        saida << "\n";
    }
    saida << "\n";
    saida.close();
}

void Arquivo::salvarResultados(const std::string &nomeArquivo, const Floresta &floresta, int iteracoes, int passos, int aguaEncontrada, int interacoesSeguras) 
{
    std::ofstream saida(nomeArquivo, std::ios::app);
    
    if (!saida.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "\n";
        
        return;
    }
    
    saida << "Simulação finalizada após " << iteracoes << " iterações.\n";
    saida << "Passos do animal: " << passos << "\n";
    saida << "Água encontrada: " << aguaEncontrada << "\n";
    saida << "Interações seguras: " << interacoesSeguras << "\n";
    saida.close();
}
