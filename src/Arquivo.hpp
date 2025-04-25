#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <string>
#include "Floresta.hpp"

class Arquivo
{
    public:
        static std::pair<int, int> obterDimensao(const std::string &nomeArquivo);

        static bool carregarMapa(const std::string &nomeArquivo, Floresta &floresta);

        static void salvarMapa(const std::string &nomeArquivo, const Floresta &floresta, int iteracao);

        static void salvarResultados(const std::string &nomeArquivo, const Floresta &floresta, int iteracoes, int passos, int aguaEncontrada, int iteracoesSegurasTotais, const std::string &caminho, bool sobreviveu);

    private:
        static const std::string DIR_ENTRADAS;
        static const std::string DIR_SAIDAS;
};

#endif
