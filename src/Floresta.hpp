#ifndef FLORESTA_HPP
#define FLORESTA_HPP

#include <vector>
#include <string>

class Floresta 
{
    public:
   
        Floresta(int n, int m);
        
        void exibir() const;
        
        void atualizarFogo();

        int obterValor(int linha, int coluna) const;

        void definirValor(int linha, int coluna, int valor);

        int linhas() const;

        int colunas() const;

        const std::vector<std::vector<int>>& getMatriz() const;

        std::vector<std::vector<int>>& getMatriz();

    private:
        int nLinhas;
        int nColunas;
        std::vector<std::vector<int>> matriz;

        bool posicaoValida(int linha, int coluna) const;
};

#endif
