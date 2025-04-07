#include "Animal.hpp"

Animal::Animal(int posX, int posY) : posX(posX), posY(posY), passos(0), aguaEncontrada(0), interacoesSeguras(0) { }

void Animal::mover(const std::vector<std::vector<int>> &matriz) 
{
    int nLinhas = matriz.size();
    int nColunas = matriz[0].size();
    
    std::vector<std::pair<int, int>> direcoes = { {-1,0}, {1,0}, {0,-1}, {0,1} };

    int melhorValor = -1;
    int novaX = posX, novaY = posY;

    for (auto d : direcoes) 
    {
        int ni = posX + d.first;
        int nj = posY + d.second;
        
        if (ni >= 0 && ni < nLinhas && nj >= 0 && nj < nColunas) 
        {
            int valorCelula = matriz[ni][nj];
            // Prioridade: 4 > 0 e 1 > 3; ignora fogo (2) e queimada (3) se não houver outra opção
            if (valorCelula == 4) 
            {
                novaX = ni; novaY = nj;
                melhorValor = 4;
                
                break;
            } 
            else if ((valorCelula == 0 || valorCelula == 1) && melhorValor < 0) 
            {
                novaX = ni; novaY = nj;
                melhorValor = valorCelula;
            }
        }
    }

    if (melhorValor != -1 && (novaX != posX || novaY != posY)) 
    {
        posX = novaX;
        posY = novaY;
        registrarPasso();
        
        if (melhorValor == 4) 
        {
            encontrouAgua();
        }
    } 
    else 
    {
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
    interacoesSeguras++; 
}

int Animal::getInteracoesSeguras() const 
{ 
    return interacoesSeguras; 
}
