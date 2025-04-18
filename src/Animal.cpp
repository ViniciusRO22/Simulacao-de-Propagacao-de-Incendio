#include "Animal.hpp"

Animal::Animal(int posX, int posY) : posX(posX), posY(posY), passos(0), aguaEncontrada(0), iteracoesSeguras(0), iteracoesSegurasTotais(0), ultimaPosX(-1), ultimaPosY(-1), vivo(true)
{ }

bool Animal::estaVivo() const
{
    return vivo;
}


void Animal::mover(std::vector<std::vector<int>> &matriz) 
{
    if (!vivo) return;

    int nLinhas = matriz.size();
    int nColunas = matriz[0].size();
    
    std::vector<std::pair<int, int>> direcoes = { {-1,0}, {1,0}, {0,-1}, {0,1} };

    // ------------- Segunda Chance -------------
    if (matriz[posX][posY] == 2) 
    {
        bool cercado = true;
    
        for (const auto &d : direcoes)
        {
            int ni = posX + d.first;
            int nj = posY + d.second;
            
            if (ni >= 0 && ni < nLinhas && nj >= 0 && nj < nColunas)
            {
                if (matriz[ni][nj] != 2)
                {
                    cercado = false;
                    
                    break;
                }
            }
        }
        
        if (cercado)
        {
            vivo = false;
           
            return;
        }
  
    }

    if (matriz[posX][posY] == 0 && iteracoesSeguras < 3) 
    {
        atualizarSeguranca();
        
        return;
    }

    // Prioridade: 3 para água (4), 2 para área vazia (0) ou árvore saudável (1), 1 para árvore queimada (3)
    int melhorPrioridade = 0;
    int novaX = posX, novaY = posY;

    for (auto d : direcoes) 
    {
        int ni = posX + d.first;
        int nj = posY + d.second;
        
        if (ni >= 0 && ni < nLinhas && nj >= 0 && nj < nColunas) 
        {
            if (ni == ultimaPosX && nj == ultimaPosY && matriz[posX][posY] != 2) 
            {
                continue;
            }
            
            int valorCelula = matriz[ni][nj];
            int prioridadeCelula = 0;
            
            if (valorCelula == 4)
            {
                prioridadeCelula = 3;
            }
            else if (valorCelula == 0 || valorCelula == 1)
            {
                prioridadeCelula = 2;
            }
            else if (valorCelula == 3)
            {
                prioridadeCelula = 1;
            }
            
            if (prioridadeCelula == 3) 
            {
                novaX = ni;
                novaY = nj;
                melhorPrioridade = 3;
                
                break;
            } 
      
            else if (prioridadeCelula > melhorPrioridade) 
            {
                novaX = ni;
                novaY = nj;
                melhorPrioridade = prioridadeCelula;
            }
        }
    }

    // Se não encontrou nenhuma opção melhor, tenta voltar para a última posição
    if (melhorPrioridade == 0 && ultimaPosX != -1 && ultimaPosY != -1) 
    {
        int valorUltima = matriz[ultimaPosX][ultimaPosY];

        if (valorUltima != 2) 
        {
            novaX = ultimaPosX;
            novaY = ultimaPosY;
            melhorPrioridade = 1; 
        }
    }


    if ((novaX != posX || novaY != posY) && melhorPrioridade > 0)
    {
        ultimaPosX = posX;
        ultimaPosY = posY;
        
        posX = novaX;
        posY = novaY;

        rodadasSemMover = 0; 

        iteracoesSeguras = 0; 
        
        registrarPasso();

        if (melhorPrioridade == 3) 
        {
            encontrouAgua();

            matriz[posX][posY] = 0;

            for (auto d : direcoes) 
            {
                int adjX = posX + d.first;
                int adjY = posY + d.second;
                if (adjX >= 0 && adjX < nLinhas && adjY >= 0 && adjY < nColunas)
                {
                    matriz[adjX][adjY] = 1;
                }
            }
        }
    }
    else 
    {
        if (!(matriz[posX][posY] == 0 && iteracoesSeguras < 3))
        {
            rodadasSemMover++; 

            if (rodadasSemMover >= limiteRodadasSemMover)
            {
                vivo = false;
               
                return;
            }
        }
        
        if (matriz[posX][posY] == 0)
        {
            atualizarSeguranca();
        }
    }
}

int Animal::getPosX() const 
{ 
    return posX; 
}

int Animal::getPosY() const 
{ 
    return posY; 
}

void Animal::registrarPasso() 
{ 
    passos++; 
}

int Animal::getPassos() const 
{ 
    return passos; 
}

void Animal::encontrouAgua() 
{ 
    aguaEncontrada++;
}

int Animal::getAguaEncontrada() const 
{ 
    return aguaEncontrada; 
}

void Animal::atualizarSeguranca() 
{ 
    iteracoesSeguras++; 
    iteracoesSegurasTotais++;
}

int Animal::getIteracoesSegurasTotais() const 
{ 
    return iteracoesSegurasTotais; 
}
