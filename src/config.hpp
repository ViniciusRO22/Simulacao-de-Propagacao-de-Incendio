#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>

const int INTERACOES_MAX = 100;

const bool VENTO_ATIVO = false; //vento ativo true, fogo propaga apenas nas direções escolhidas

enum Direcao 
{
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
};

const std::vector<Direcao> direcoesVento = { CIMA, DIREITA };

#endif
