#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include <string>
#include "Floresta.hpp"
#include "Animal.hpp"

class Simulacao 
{
public:
    Simulacao(const std::string &arquivoEntrada, const std::string &arquivoSaida);

    void executar();

private:
    std::string arquivoEntrada;
    std::string arquivoSaida;
    Floresta *floresta;
    Animal *animal;

    bool inicializar();

    bool inicializarAnimal();

    void limpar();
};

#endif
