#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <vector>
#include <utility>
#include <deque>
#include <set>

class Animal 
{
    public:
        Animal(int posX, int posY);

        bool estaVivo() const;

        void mover(std::vector<std::vector<int>> &matriz);

        int getPosX() const;
        int getPosY() const;

        void registrarPasso();
        int getPassos() const;

        void encontrouAgua();
        int getAguaEncontrada() const;

        void atualizarSeguranca();
        int getIteracoesSegurasTotais() const;

    private:
        int posX;
        int posY;
        int passos;
        int aguaEncontrada;
        int iteracoesSeguras;
        int iteracoesSegurasTotais;
        int ultimaPosX;
        int ultimaPosY;
        bool vivo; 
        int rodadasSemMover = 0;
        const int limiteRodadasSemMover = 2;
};

#endif
