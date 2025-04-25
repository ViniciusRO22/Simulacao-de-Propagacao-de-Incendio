#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>

const int ITERACOES_MAX = 10000;

const int MIN_LINHAS = 100;
const int MIN_COLUNAS = 100;

const bool VENTO_ATIVO = false; // true: com vento , false: sem vento

enum Direcao 
{
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
};

const std::vector<Direcao> direcoesVento = { DIREITA , BAIXO }; // Caso o vento esteja ativo, é preciso configurar as direções específicas para a propagação.

#endif
