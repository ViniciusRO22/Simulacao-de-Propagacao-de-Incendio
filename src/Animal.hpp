#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <vector>

class Animal 
{
    public:
        Animal(int posX, int posY);

        void mover(const std::vector<std::vector<int>> &matriz);

        int getPosX() const;

        int getPosY() const;

        void registrarPasso();

        int getPassos() const;

        void encontrouAgua();

        int getAguaEncontrada() const;

        void atualizarSeguranca();

        int getInteracoesSeguras() const;

    private:
        int posX;
        int posY;
        int passos;
        int aguaEncontrada;
        int interacoesSeguras;
};

#endif
