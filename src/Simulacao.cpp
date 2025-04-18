#include "Simulacao.hpp"
#include "Arquivo.hpp"
#include "config.hpp"
#include <iostream>
#include <sstream>

Simulacao::Simulacao(const std::string &arquivoEntrada, const std::string &arquivoSaida) : arquivoEntrada(arquivoEntrada), arquivoSaida(arquivoSaida), floresta(nullptr), animal(nullptr){}

bool Simulacao::inicializar() 
{
    auto dimensoes = Arquivo::obterDimensao(arquivoEntrada);
    int linhas = dimensoes.first;
    int colunas = dimensoes.second;
    
    if (linhas < MIN_LINHAS || colunas < MIN_COLUNAS) 
    {
        std::cerr << "Dimensões insuficientes para a simulação." << std::endl;
        
        return false;
    }
    
    floresta = new Floresta(linhas, colunas);
    
    if (!Arquivo::carregarMapa(arquivoEntrada, *floresta)) 
    {
        std::cerr << "Erro ao carregar o mapa." << std::endl;
        
        return false;
    }
    
    if (!inicializarAnimal()) 
    {
        std::cerr << "Não foi possível encontrar uma célula segura para o animal." << std::endl;
        
        return false;
    }
    
    return true;
}

bool Simulacao::inicializarAnimal() 
{
    const auto &matriz = floresta->getMatriz();
    int linhas = floresta->linhas();
    int colunas = floresta->colunas();
    
    for (int i = 0; i < linhas; i++) 
    {
        for (int j = 0; j < colunas; j++) 
        {
            if (matriz[i][j] == 0) 
            {
                animal = new Animal(i, j);
                
                return true;
            }
        }
    }
    
    return false;
}

void Simulacao::executar() 
{
    if (!inicializar()) 
    {
        std::cerr << "Falha na inicialização da simulação." << std::endl;
        
        return;
    }

    int iteracoes = 0;
    std::ostringstream caminho; 

    while (iteracoes < ITERACOES_MAX) 
    {
        iteracoes++;
   
        animal->mover(floresta->getMatriz());
        
        if (!animal->estaVivo())
        {
            std::cout << "O animal morreu na iteracao " << iteracoes << "!\n";
            break;
        }
        
        floresta->atualizarFogo();
        
        Arquivo::salvarMapa(arquivoSaida, *floresta, iteracoes);
        
        floresta->exibir();
        
        caminho << "(" << animal->getPosX() << "," << animal->getPosY() << ")[iteração " << iteracoes << "] ";
        
        if (floresta->todaQueimada()) 
        {
            std::cout << "A floresta está completamente queimada ou o fogo se extinguiu. Encerrando simulação." << std::endl;
            
            break;
        }
    }

    bool sobreviveu = animal->estaVivo();
    int passos = animal->getPassos();
    int aguaEncontrada = animal->getAguaEncontrada();
    int iteracoesSegurasTotais = animal->getIteracoesSegurasTotais();
    
    Arquivo::salvarResultados(arquivoSaida, *floresta, iteracoes, passos, aguaEncontrada, iteracoesSegurasTotais, caminho.str(), sobreviveu);
    
    limpar();
}

void Simulacao::limpar() 
{
    if (animal) 
    {
        delete animal;
        animal = nullptr;
    }
    
    if (floresta) 
    {
        delete floresta;
        floresta = nullptr;
    }
}
