#ifndef MANIPULADOR_ARQUIVOS_HPP
#define MANIPULADOR_ARQUIVOS_HPP

#include <string>
#include "Floresta.hpp"

class ManipuladorArquivos {
public:
    static std::pair<int, int> obterDimensao(const std::string &nomeArquivo); //obter dimensões da matriz

    static bool carregarMapa(const std::string &nomeArquivo, Floresta &floresta); //carrega o mapa

    static void salvarMapa(const std::string &nomeArquivo, const Floresta &floresta, int iteracao); //salva o estado atual da floresta

    static void salvarResultados(const std::string &nomeArquivo, const Floresta &floresta, int iteracoes, int passos, int aguaEncontrada, int interacoesSeguras); //salva final
};

#endif
