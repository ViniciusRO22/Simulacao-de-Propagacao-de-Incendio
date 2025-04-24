# Simulação de Propagação de Incêndio
## Sumário

1. [Introdução](#introdução)
   - Propagação do Fogo
   - Comportamento do Animal
2. [Estrutura das Classes](#estrutura-das-classes)
   - Classe `config.hpp`  
   - Classe `Arquivo.hpp` / `Arquivo.cpp`  
   - Classe `Animal.hpp` / `Animal.cpp`  
   - Classe `Floresta.hpp` / `Floresta.cpp`  
   - Classe `Simulacao.hpp` / `Simulacao.cpp`  
3. [Análises dos Padrões de Propagação do Fogo](#análises-dos-padrões-de-propagação-do-fogo)  
   - Propagação sem Vento  
   - Propagação com Vento  
   - Tabela Comparativa  
4. [Capacidade Preditiva da Simulação](#capacidade-preditiva-da-simulação)  
   - Modelagem Temporal  
   - Fatores de Influência  
   - Equações Específicas  
5. [Algoritmos que poderiam ser utilizados para Melhoria de Desempenho](#algoritmos-que-poderiam-ser-utilizados-para-melhoria-de-desempenho)  
   - Busca em Largura (BFS)  
   - Autômato Celular Probabilístico  
   - A* Adaptativo  
   - Comparação de Desempenho  
7. [Conclusão](#conclusão)  
8. [Compilação e Execução](#compilação-e-execução)
9. [Referências](#referencias)
10. [Autores e Contatos](#autores-e-contatos)

---

## Introdução 
Este projeto é uma implementação de um simulador de propagação de incêndios em ambientes florestais, utilizando programação em C++. A simulação ocorre em uma matriz N×M, onde cada célula representa um estado específico da floresta: áreas vazias, árvores saudáveis, árvores em chamas, árvores queimadas e água. O objetivo principal é analisar a dinâmica do fogo sob diferentes condições, como a influência do vento, e incorporar a inteligência de um animal que busca sobreviver. As principais caracteristicas são:

### 1. Propagação do Fogo  
- **Transição de estados**:  
  - Árvore saudável (`1`) → Em chamas (`2`) → Queimada (`3`).  
  - Células em chamas (`2`) tornam-se queimadas (`3`) após 2 iterações.
    
- **Modos de propagação**:  
  - **Sem vento**: Expansão nas 4 direções ortogonais(cima, baixo, esquerda, direita).  
  - **Com vento**: Direções configuráveis (ex: `CIMA` e `DIREITA`) para simular uma propagação mais específica.    

- **Controle de execução**:  
  - Limite máximo de iterações de propagação de fogo de acordo com `config.hpp`
  - Fim da simulação caso o fogo seja extinto ou o animal morra

### 2. Comportamento do Animal  
- **Prioridades de movimento**:  
  - **Água (`4`)**: Movimento imediato 
  - **Área vazia (`0`) ou árvore saudável (`1`)**: Segunda opção.  
  - **Árvore queimada (`3`)**: Última opção.  

- **Mecanismo de sobrevivência**:  
  - **Segunda chance**: Se atingido pelo fogo, tenta se mover imediatamante antes da próxima propagação. Porém caso esteja cercado por fogo o animal morre  

- **Efeito da água**: Caso uma celúla com água seja alcançada pelo animal 
  - Célula com água (`4`) → Se tornam área segura (`0`).  
  - Células adjacentes → Regeneradas para árvores saudáveis (`1`).  

---

## Estrutura das Classes

### 1. Classe `config.hpp`
#### Objetivo
Define configurações globais para a simulação. Pode ser ajustado de acordo com o desejo do usuário em relação ao número de iterações máximas da simulção, tamanho da matriz a ser simulada, simulação contendo vento ou não. O código aqui apresentado aqui nesse repositório segue as definções apresentadas abaixo em componentes

#### Componentes:
- **ITERACOES_MAX**: Limite máximo de iterações (10.000).
- **MIN_LINHAS** e **MIN_COLUNAS**: Tamanho mínimo da matriz (100x100).
- **VENTO_ATIVO**: Ativa/desativa o vento(`true` ativo / `false`desativo).
- **Direcao (enum)**: Direções possíveis do vento (`CIMA`, `BAIXO`, `ESQUERDA`, `DIREITA`).
- **direcoesVento**: Vetor com direções para o vento caso ele esteja ativo, com ele podendo se propagar para uma ou mais direções (ex: `{CIMA, DIREITA}`).

#### Importância:
- Controla a complexidade e o comportamento dinâmico do fogo.
- Permite ajustar a simulação para diferentes cenários (ex: vento influenciando a propagação, áreas diferentes de floresta e limites diferentes de iterações máximas).

---

### 2. Classe `Arquivo.hpp`/`Arquivo.cpp`
#### Funcionalidades:
- **Carregar Mapa**: Lê o arquivo `input.dat` e inicializa a matriz da floresta.
- **Salvar Mapa**: Gera o arquivo `output.dat` com o estado da floresta a cada iteração.
- **Salvar Resultados**: Registra métricas finais (sobrevivência, passos do animal, iterações necessárias para encerrar a simulação, caminho percorrido pelo animal, número de águas encontradas e número de iterações seguras feitas pelo animal).
- `OBS`: o animal se move primeiro que a propagação de fogo, então a posição do animal em respectiva iteração representa a escolha de movimentação feita antes da propagação de fogo, mas na matriz impressa após a iteração completa a propagação de fogo já foi efetuada.

#### Formato do `input.dat`:
Exemplo de entrada em uma matriz 5x5, lembrando que o recomendado é considerar valores mínimos de tamanho da matriz 100x100
| 5  | 5 | 1 | 1 |  |
|---|---|---|---|---|
| 1 | 1 | 1 | 1 | 4 |
| 1 | 2 | 1 | 1 | 1 |
| 1 | 1 | 1 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |
- **Linha 1**: Dimensões da matriz (`N M`) e posição inicial do fogo (`X Y`).
- **Matriz**: Valores representando:
  - `0`: Área vazia (segura para o animal).
  - `1`: Árvore saudável.
  - `2`: Árvore em chamas.
  - `3`: Árvore queimada.
  - `4`: Água.

#### Formato do `output.dat`:
- **Iterações**: Snapshots da matriz a cada iteração(exemplo de acordo com a matriz exemplo do input.dat e sem vento)

Iteração 1:
| 1 | 2 | 1 | 1 | 4 |
|---|---|---|---|---|
| 2 | 3 | 2 | 1 | 1 |
| 1 | 2 | 1 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 2:
| 2 | 2 | 2 | 1 | 4 |
|---|---|---|---|---|
| 2 | 3 | 2 | 2 | 1 |
| 2 | 2 | 2 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 3:
| 2 | 3 | 2 | 2 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 2 | 2 |
| 2 | 3 | 2 | 2 | 4 |
| 0 | 0 | 2 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 4:
| 3 | 3 | 3 | 2 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 2 |
| 3 | 3 | 3 | 2 | 4 |
| 0 | 0 | 2 | 2 | 1 |
| 1 | 4 | 2 | 0 | 4 |


Iteração 5:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 2 | 3 | 2 | 2 |
| 1 | 0 | 2 | 0 | 4 |

Iteração 6:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 2 | 3 | 3 | 2 |
| 1 | 0 | 2 | 0 | 4 |


Iteração 7:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 3 | 3 | 3 | 3 |
| 1 | 0 | 3 | 0 | 4 |


- **Resultados Finais**: exemplo de acordo com a matriz exemplo do input.dat 

> **Simulação finalizada após 7 iterações.**  
> **Caminho percorrido pelo animal:** (3,0)[iteração 1] (3,0)[iteração 2] (3,0)[iteração 3] (4,0)[iteração 4] (4,1)[iteração 5] (4,1)[iteração 6] (4,1)[iteração 7]  
> **Passos do animal:** 2  
> **Água encontrada:** 1  
> **Interações seguras:** 5  
> **O animal sobreviveu.**

---

### 3. Classe `Animal.hpp`/`Animal.cpp`
#### Visão Geral:
A classe `Animal` modela um agente autônomo que tenta sobreviver ao incêndio na floresta. Suas principais responsabilidades incluem:  
- **Movimentação inteligente** com base no estado da matriz.  
- **Gestão de recursos** (ex: busca por fontes de água).  
- **Controle de sobrevivência** (detecção de perigo iminente).  

#### Construtor:
##### Parâmetros do Construtor

| Parâmetro | Inicialização           | Descrição                                                     |
|-----------|-------------------------|---------------------------------------------------------------|
| posX      | Primeiro 0 encontrado   | Coordenada X inicial  |
| posY      | Primeiro 0 encontrado   | Coordenada Y inicial.                                         |


##### Variáveis Inicializadas

| Variável                 | Valor Inicial | Função                                                    |
|--------------------------|---------------|-----------------------------------------------------------|
| passos                   | 0             | Contador de movimentos realizados.                        |
| aguaEncontrada           | 0             | Quantidade de células com água (4) acessadas.             |
| iteracoesSeguras         | 0             | Contador temporário para iterações consecutivas em células seguras(0). |
| iteracoesSegurasTotais   | 0             | Contador global para iterações consecutivas em células seguras(0). |            
| ultimaPosX / ultimaPosY  | -1            | Última posição antes do movimento atual (evita backtracking). |
| vivo                     | true          | Estado de vida do animal.                                 |


#### Método Mover:
##### Verificação de Segunda Chance:
```cpp
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
```
| Componente          | Funcionalidade                                                | Resposta Esperada                                                       |
|---------------------|---------------------------------------------------------------|--------------------------------------------------------------------------|
| Verificação de Fogo | Checa se o animal está em uma célula em chamas (`2`).         | Se sim, analisa as células adjacentes |
| Loop de Direções    | Analisa células adjacentes.                                   | Se cercado por fogo em todas as direções, o animal morre (`vivo = false`). Caso não esteja cercado, encerra a verificação e permite q o código responsavel pela movimentação do animal atue                 |

#### Atualização de Segurança:
```cpp
if (matriz[posX][posY] == 0 && iteracoesSeguras < 3) {
    atualizarSeguranca();
    return;
}
```
| Componente             | Funcionalidade                                      | Resposta Esperada                                                   |
|------------------------|-----------------------------------------------------|----------------------------------------------------------------------|
| Área Segura            | Detecta se está em célula segura (`0`)              | Permanece imóvel por até 3 iterações para se manter seguro contra o fogo     |
| atualizarSeguranca()   | Incrementa contadores de segurança                  | Aumenta `iteracoesSeguras` e `iteracoesSegurasTotais`               |

#### Lógica de Prioridades de Movimentação:
```cpp
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
```
| Componente                    | Funcionalidade                                                                                   | Resposta Esperada                                                         |
|-------------------------------|----------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------|
| Limites da matriz             | Garante que `ni` e `nj` estejam dentro dos limites da matriz                                 | Evita acessar posições inválidas (segfaults)                                      |
| Verificação de retrocesso     | Evita retornar à posição anterior imediatamente, exceto se estiver sobre fogo                | Melhora eficiência, previne movimento oscilante                                   |
| `valorCelula`                 | Armazena o conteúdo da célula adjacente                                                      | Utilizado para definir a prioridade de movimento                                  |
| Priorização de célula         | Define a importância de cada tipo de célula                                                  | Direciona o movimento do animal para as áreas mais desejáveis                     |
| - Água (4)                    | `prioridadeCelula = 3`                                                                       | Alta prioridade – movimento imediato para essa célula                             |
| - Área segura (0) ou árvore (1) | `prioridadeCelula = 2`                                                                     | Média prioridade – locais considerados seguros                                    |
| - Queimada (3)                | `prioridadeCelula = 1`                                                                       | Baixa prioridade – apenas se nada melhor for encontrado                           |
| Atualização de posição        | Se a célula tiver prioridade maior que a atual, torna-se o novo destino (`novaX`, `novaY`)   | O animal se prepara para mover na direção mais promissora                         |
| Break com prioridade máxima   | Interrompe o laço ao encontrar célula de água (prioridade 3)                                 | Otimiza a decisão de movimento, reduzindo o tempo de busca                        |

####Tratamento da Movimentação e da Água
```cpp
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
```
| Componente                | Funcionalidade                                                                 | Resultado Esperado                                                                 |
|--------------------------|--------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| Verificação de movimento | Garante que o animal só se mova se a nova posição for diferente da atual       | Evita contagem de movimento quando o animal não muda de posição                    |
| Atualização de posição   | Atualiza `posX` e `posY` com base na nova posição escolhida (`novaX`, `novaY`) | Move o animal para a nova célula com melhor prioridade                             |
| Reset de contadores      | Reinicia `rodadasSemMover` e `iteracoesSeguras`                               | Zera iteraçõesSeguras possibilitando que o animal permaneça novamente 3 iterações consecutivas em células seguras(0) e zera o tempo parado |
| registrarPasso()         | Registra um novo movimento                                                     | Incrementa o contador de passos                                                    |
| Verificação de água      | Se a célula destino era água (`prioridade == 3`), executa ações especiais      | Aumenta `aguaEncontrada` , altera célula atual de água (4) para segura (0) e converte células adjacentes em árvores saudáveis (valor 1) |

#### Controle de Inatividade:
```cpp
 if (!(matriz[posX][posY] == 0 && iteracoesSeguras < 3))
        {
            rodadasSemMover++; 

            if (rodadasSemMover >= limiteRodadasSemMover)
            {
                vivo = false;
               
                return;
            }
        }
```
| Componente         | Funcionalidade                                 | Resposta Esperada                               |
|--------------------|-----------------------------------------------|--------------------------------------------------|
| Verificação de Rodadas Sem Mover    | Conta iterações consecutivas sem movimento caso o animal não esteja efetuando iterações seguras em uma célula 0   | Após 2 rodadas imóvel, o animal morre (vivo = false) |


#### Método de Acesso e Registro:

##### `registrarPasso(), getPassos()`:

-Funcionamento: Incrementa e retorna o contador de passos.

-Importância: Mede a atividade do animal durante a simulação.

##### `encontrouAgua(), getAguaEncontrada()`:

-Funcionamento: Registra cada encontro com água.

-Importância: Quantifica a eficiência do animal em localizar recursos.

##### `atualizarSeguranca(), getIteracoesSegurasTotais()`:

-Funcionamento: Conta iterações consecutivas em células seguras (0).

-Importância: Indica períodos de segurança prolongada.



---

### 4. Classe `Floresta.hpp`/`Floresta.cpp`
#### Visão Geral:
A classe `Floresta` modela a dinâmica do ambiente e a propagação do incêndio.Suas principais responsabilidades incluem:  
- **Gerenciamento da matriz** representando o estado da floresta  
- **Propagação controlada do fogo** com ou sem influência do vento  
- **Atualização temporal** dos estados das células (árvores saudáveis, em chamas, queimadas)  

#### Construtor:
##### Parâmetros do Construtor

| Parâmetro | Inicialização           | Descrição                                                     |
|-----------|-------------------------|---------------------------------------------------------------|
| n         | Definido por input.dat  | Número de linhas da matriz                                    |
| m         | Definido por input.dat  | Número de colunas da matriz                                   |

##### Variáveis Inicializadas

| Variável                 | Valor Inicial        | Função                                                       |
|--------------------------|----------------------|--------------------------------------------------------------|
| nLinhas                  | n                    | Armazena as linhas da matriz                                  |
| nColunas                 | m                    | Armazena as colunas da matriz                                 |
| matriz                   | Grid n×m inicializado com 0 | Representa o estado atual de cada célula (0-4)         |
| fireGen                  | Vetor vazio          | Registro temporal do surgimento do fogo em cada célula       |

#### Método Exibir:
```cpp
  for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            std::cout << matriz[i][j] << " ";
        }
        
        std::cout << "\n";
    }

    std::cout << "\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
```
| Componente                     | Funcionalidade                                                                 | Resposta Esperada                                                                 |
|--------------------------------|-------------------------------------------------------------------------------|----------------------------------------------------------------------------------|
| Loop pelas linhas              | `for (int i = 0; i < nLinhas; i++)`                                           | Percorre todas as linhas da matriz, começando do topo (linha 0) até o fundo.      |
| Loop pelas colunas             | `for (int j = 0; j < nColunas; j++)`                                          | Percorre todas as colunas de cada linha, da esquerda (coluna 0) à direita.       |
| Impressão do valor             | `std::cout << matriz[i][j] << " ";`                                           | Exibe o valor da célula atual seguido de um espaço (ex: `2` para fogo).          |
| Quebra de linha                | `std::cout << "\n";`                                                          | Finaliza a exibição de cada linha da matriz, garantindo formato grid.            |
| Atraso controlado              | `std::this_thread::sleep_for(std::chrono::milliseconds(500));`                | Pausa de 500ms entre cada atualização para visualização passo a passo.           |

**Importância**:
- **Visualização em Tempo Real**: Permite acompanhar a dinâmica do incêndio e a resposta do animal iterativamente.
- **Debugging**: Facilita a identificação de erros na propagação do fogo ou na lógica do animal.
- **Experiência Interativa**: Cria uma simulação "animada" no console.


#### Método AtualizarFogo:
##### Inicialização do FireGen:
```cpp
 static int iter = 0;
    ++iter;

    std::vector<std::vector<int>> novaMatriz = matriz;

    if (fireGen.empty()) 
    {
        fireGen.resize(nLinhas, std::vector<int>(nColunas, -1));
    }
```
| Componente       | Funcionalidade                                                 | Resposta Esperada                                                            |
|------------------|----------------------------------------------------------------|----------------------------------------------------------------------------- |
| `static int iter`| Mantém contagem global de iterações entre chamadas do método   | Incrementado a cada atualização (ex: iter = 1, 2, 3, …)                      |
| `++iter`         | Atualiza o contador temporal                                   | Garante sincronia com o tempo simulado                                       |
| `novaMatriz`     | Cópia do estado atual da floresta                              | Evita alterações conflitantes durante o processamento                        |
| `fireGen`        | Vetor 2D paralelo à matriz principal                           | Permite rastrear há quantas iterações cada célula está em chamas             |
| Valor -1         | Valor padrão em `fireGen` indicando que nunca queimou          | Serve como marcador para células não afetadas pelo fogo até o momento atual  |

##### Propagação do Fogo:
```cpp
 for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            if (matriz[i][j] == 1) 
            { 
                bool vizinhoEmFogo = false;
                std::vector<std::pair<int, int>> direcoes;

                if (VENTO_ATIVO) 
                {
                    for (auto dir : direcoesVento) 
                    {
                        switch (dir) 
                        {
                            case CIMA:    direcoes.push_back({1, 0}); break;
                            case BAIXO:   direcoes.push_back({-1, 0});  break;
                            case ESQUERDA:direcoes.push_back({0, 1});  break;
                            case DIREITA: direcoes.push_back({0, -1});  break;
                        }
                    }
                }
                else 
                {
                    direcoes = {{-1,0},{1,0},{0,-1},{0,1}};
                }

                for (auto &d : direcoes) 
                {
                    int ni = i + d.first, nj = j + d.second;
                   
                    if (posicaoValida(ni,nj) && matriz[ni][nj] == 2) 
                    {
                        vizinhoEmFogo = true;
                        
                        break;
                    }
                }

                if (vizinhoEmFogo) 
                {
                    novaMatriz[i][j] = 2;
                    fireGen[i][j] = iter; 
                }
            }
        }
    }
```
| Componente            | Funcionalidade                                                 | Resposta Esperada                                                           |
|-----------------------|----------------------------------------------------------------|-----------------------------------------------------------------------------|
| **Loop Principal**    | Itera por todas as células da matriz (`i` = linha, `j` = coluna)| Processa cada célula individualmente                                            |
| **Filtro de Árvores** | `if (matriz[i][j] == 1)`                                       | Apenas árvores saudáveis podem pegar fogo                                        |
| **Configuração de Direções** | Define direções de propagação com base no vento          | Direções personalizadas ou padrão (todas as 4 adjacências)                      |
| **Cálculo de Vizinhos**      | `ni = i + d.first, nj = j + d.second`                       | Calcula coordenadas do vizinho na direção especificada                       |
| **Validação de Posição**     | `posicaoValida(ni, nj)`                                      | Garante que o vizinho esteja dentro dos limites da matriz                   |
| **Detecção de Fogo**         | `matriz[ni][nj] == 2`                                        | Verifica se o vizinho está em chamas                                        |
| **Atualização de Estado**    | `novaMatriz[i][j] = 2`, `fireGen[i][j] = iter`               | Marca a árvore como em chamas e registra o momento exato da ignição         |


**Tabela de Direções do Vento:** os valores que representam as posições ortogonais (cima, baixo, direita e esquerda) teoricamente estão invertidos, mas o código só funciona corretamente dessa maneira, não consegui descobrir o motivo.
| Direção Configurada | Vetor de Propagação | Comportamento Real (Exemplo)       |
|---------------------|---------------------|------------------------------------|
| **CIMA**            | `(1, 0)`            | Propaga para cima                  |
| **BAIXO**           | `(-1, 0)`           | Propaga para baixo                 |
| **ESQUERDA**        | `(0, 1)`            | Propaga para direita               |
| **DIREITA**         | `(0, -1)`           | Propaga para esquerda              |

##### Queima de Árvore:
```cpp
for (int i = 0; i < nLinhas; i++) 
    {
        for (int j = 0; j < nColunas; j++) 
        {
            if (matriz[i][j] == 2 && fireGen[i][j] <= iter - 2) 
            {
                novaMatriz[i][j] = 3;
            }
        }
    }
```
| Componente               | Funcionalidade                                                                 | Resposta Esperada                                                                 |
|--------------------------|-------------------------------------------------------------------------------|----------------------------------------------------------------------------------|
| `matriz[i][j] == 2`      | Verifica se a célula está em chamas no estado atual.                          | Identifica células que estão queimando no momento, garantindo que apenas essas sejam candidatas a virar cinzas. |
| `fireGen[i][j]`          | Armazena a iteração em que a célula pegou fogo (ex: `fireGen[2][3] = 5`).     | Permite calcular há quantas iterações a célula está em chamas, comparando com o `iter` atual. |
| `iter`                   | Contador global de iterações (incrementado a cada passo da simulação).        | Fornece o "tempo atual" da simulação, usado como referência para determinar a duração do fogo. |
| `iter - 2`               | Representa duas iterações atrás no tempo.                                     | Define o limite temporal, se uma célula está em chamas há 2+ iterações (`fireGen[i][j] <= iter - 2`), ela se torna queimada |
| `novaMatriz[i][j] = 3`   | Marca a célula como queimada                                                  | Transforma a celulá em 3 |


#### Método Toda Queimada: 
```cpp
bool Floresta::todaQueimada() const 
{
    for (const auto &linha : matriz) 
    {
        for (int valor : linha) 
        {
         
            if (valor == 2) 
            {
                return false;
            }
        }
    }
   
    return true;
}
```
| Componente            | Funcionalidade                                                            | Resposta Esperada                                                         |
|-----------------------|---------------------------------------------------------------------------|---------------------------------------------------------------------------|
| Loop for por linhas   | Percorre cada linha da matriz (`linha : matriz`)                          | Garante verificação completa de toda a floresta                            |
| Loop for por valores  | Itera sobre cada célula da linha (`valor : linha`)                        | Acessa o estado individual de cada célula                                  |
| Verificação `valor == 2` | Verifica se a célula está em chamas                                    | Identifica focos de fogo ativos                                            |
| Retorno `false`       | Interrompe a execução imediatamente se encontrar fogo                     | Indica que há fogo ativo na floresta                                       |
| Retorno `true`        | Executado apenas se nenhum `2` for encontrado em toda a matriz            | Indica que não há fogo ativo (floresta totalmente queimada ou  fogo extinto) , garantindo a interrupção da simulação 

---
### 5. Classe `Simulacao.hpp`/`Simulacao.cpp`
#### Visão Geral:
A classe Simulacao orquestra todo o processo de simulação. Principais responsabilidades:
- **Coordenação** entre floresta, animal e arquivos 
- **Ciclo** principal de execução
- **Gerenciamento** de recursos e estados  

#### Construtor:
##### Parâmetros do Construtor
| Parâmetro        | Inicialização            | Descrição                                            |
|------------------|--------------------------|------------------------------------------------------|
| `arquivoEntrada` | Definido pelo usuário    | Caminho do arquivo de entrada (`input.dat`)          |
| `arquivoSaida`   | Definido pelo usuário    | Caminho do arquivo de saída (`output.dat`)           |

##### Variáveis Inicializadas
| Variável   | Valor Inicial | Função                                                    |
|------------|---------------|-----------------------------------------------------------|
| `floresta` | `nullptr`     | Ponteiro para objeto `Floresta` (alocado durante a inicialização) |
| `animal`   | `nullptr`     | Ponteiro para objeto `Animal` (alocado durante a inicialização)   

#### Método Executar:
##### Fluxo Principal:
```cpp
int iteracoes = 0;
    std::ostringstream caminho; 

    while (iteracoes < ITERACOES_MAX) 
    {
        iteracoes++;
   
        animal->mover(floresta->getMatriz());
        
        if (!animal->estaVivo())
        {
            std::cout << "O animal morreu na iteracao " << iteracoes << "!\n";
            break;
        }
        
        floresta->atualizarFogo();
        
        Arquivo::salvarMapa(arquivoSaida, *floresta, iteracoes);
        
        floresta->exibir();
        
        caminho << "(" << animal->getPosX() << "," << animal->getPosY() << ")[iteração " << iteracoes << "] ";
        
        if (floresta->todaQueimada()) 
        {
            std::cout << "A floresta está completamente queimada ou o fogo se extinguiu. Encerrando simulação." << std::endl;
            
            break;
        }
    }
```
| Componente               | Frequência     | Funcionalidade                                              |
|--------------------------|----------------|-------------------------------------------------------------|
| Movimento do animal      | Por iteração   | Atualiza posição com base na lógica de prioridades          |
| Propagação do fogo       | Por iteração   | Atualiza estado da floresta conforme regras de propagação   |
| Salvamento do estado     | Por iteração   | Gera snapshot no `output.dat`                               |

**Condições de Término**
| Condição                        | Ação                                                        |
|---------------------------------|-------------------------------------------------------------|
| Animal morto                    | Quebra loop e registra “O animal não sobreviveu”            |
| Floresta totalmente queimada    | Interrompe simulação e registra estado final                |
| Limite de iterações             | Encerra após `ITERACOES_MAX` (10.000) iterações             |

#### Métodos Suporte:
##### Inicializar

| Etapa              | Funcionalidade                                         | Critério de Sucesso                      |
|--------------------|--------------------------------------------------------|------------------------------------------|
| Carregar dimensões | Valida contra `MIN_LINHAS` / `MIN_COLUNAS`            | Dimensões >= 100x100                     |
| Carregar mapa      | Preenche matriz da floresta a partir do `input.dat`   | Arquivo existe e é legível               |
| Posicionar animal  | Encontra primeira célula segura (valor 0)             | Pelo menos uma célula segura disponível  |

##### Limpar

| Recurso   | Ação     | Motivo                                          |
|-----------|----------|-------------------------------------------------|
| `animal`  | `delete` | Libera memória alocada dinamicamente            |
| `floresta`| `delete` | Libera matriz potencialmente grande             |

---

## Análises dos Padrões de Propagação do Fogo

### 1. Propagação sem Vento
#### Comportamento:
- **Expansão exponencial**: Cada célula em chamas inflama até 4 vizinhos (todas as direções).
- **Formula**: A(t) = A0 * 4^t (em matriz sem obstáculos) / `A0` = Células iniciais em chamas `t` = Número de iterações


#### Exemplo Visual(de acordo com a matriz de exemplo input.dat):

Iteração 1:
| 1 | 2 | 1 | 1 | 4 |
|---|---|---|---|---|
| 2 | 3 | 2 | 1 | 1 |
| 1 | 2 | 1 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 2:
| 2 | 2 | 2 | 1 | 4 |
|---|---|---|---|---|
| 2 | 3 | 2 | 2 | 1 |
| 2 | 2 | 2 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 3:
| 2 | 3 | 2 | 2 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 2 | 2 |
| 2 | 3 | 2 | 2 | 4 |
| 0 | 0 | 2 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |


Iteração 4:
| 3 | 3 | 3 | 2 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 2 |
| 3 | 3 | 3 | 2 | 4 |
| 0 | 0 | 2 | 2 | 1 |
| 1 | 4 | 2 | 0 | 4 |


Iteração 5:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 2 | 3 | 2 | 2 |
| 1 | 0 | 2 | 0 | 4 |

Iteração 6:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 2 | 3 | 3 | 2 |
| 1 | 0 | 2 | 0 | 4 |


Iteração 7:
| 3 | 3 | 3 | 3 | 4 |
|---|---|---|---|---|
| 3 | 3 | 3 | 3 | 3 |
| 3 | 3 | 3 | 3 | 4 |
| 0 | 3 | 3 | 3 | 3 |
| 1 | 0 | 3 | 0 | 4 |


### 2. Propagação com Vento
#### Comportamento:
#### Tipos de Crescimento:
| Direções do Vento  | Tipo de Crescimento | Fórmula                  | Exemplo (t=3)           |
|--------------------|---------------------|--------------------------|-------------------------|
| 1 Direção          | Linear              | `A(t) = A0 + t`          | 1 → 2 → 3 → 4 células   |
| 2 Direções         | Exponencial         | `A(t) = A0 * 2^t`        | 1 → 2 → 4 → 8 células   |
| 3+ Direções        | Exponencial         | `A(t) = A0 * k^t` (2<k<4)| Padrão complexo         |

#### Exemplo Visual / Vento = DIREITA + BAIXO (de acordo com a matriz de exemplo input.dat):

Iteração 1:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 2 | 1 | 1 |
| 1 | 2 | 1 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |

Iteração 2:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 2 | 2 | 1 |
| 1 | 2 | 2 | 1 | 4 |
| 0 | 0 | 1 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |

Iteração 3:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 3 | 2 | 2 |
| 1 | 3 | 2 | 2 | 4 |
| 0 | 0 | 2 | 1 | 1 |
| 1 | 4 | 1 | 0 | 4 |

Iteração 4:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 3 | 3 | 2 |
| 1 | 3 | 3 | 2 | 4 |
| 0 | 0 | 2 | 2 | 1 |
| 1 | 4 | 2 | 0 | 4 |

Iteração 5:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 3 | 3 | 3 |
| 1 | 3 | 3 | 3 | 4 |
| 0 | 0 | 3 | 2 | 2 |
| 1 | 4 | 2 | 0 | 4 |

Iteração 6:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 3 | 3 | 3 |
| 1 | 3 | 3 | 3 | 4 |
| 0 | 0 | 3 | 3 | 2 |
| 1 | 4 | 3 | 0 | 4 |

Iteração 7:
| 1 | 1 | 1 | 1 | 4 |
|---|---|---|---|---|
| 1 | 3 | 3 | 3 | 3 |
| 1 | 3 | 3 | 3 | 4 |
| 0 | 0 | 3 | 3 | 3 |
| 1 | 4 | 3 | 0 | 4 |

### Tabela Comparativa:
| Característica       | Sem Vento            | Com Vento (1 Direção) | Com Vento (2+ Direções) |
|----------------------|----------------------|------------------------|--------------------------|
| Direções de Expansão | 4 (todas)            | 1 direção fixa         | 2+ direções configuráveis |
| Tipo de Crescimento  | Exponencial (4^t)    | Linear (t+1)           | Exponencial (2^t a 3^t)  |
| Velocidade           | 4 células/iteração   | 1 célula/iteração      | 2-3 células/iteração      |
| Padrão Geométrico    | Circular             | Linear                 | Elíptico/Setorial        |
| Previsibilidade      | Alta                 | Muito Alta             | Moderada                 |

Taxa teórica em matriz livre, na prática pode reduzida por obstáculos, como as dimensões da matriz.

---

## Capacidade Preditiva da Simulação

### Modelagem Temporal

- **Unidade de Tempo:** cada iteração equivale a 1 unidade de tempo (por exemplo, 1 hora).

### Fatores de Influência

| Fator      | Efeito na Propagação                              | Exemplo Prático                                                                                     |
|------------|---------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| Água (4)  | Reduz a área queimada final                        | A água se transforma em 0 e todas as 4 células vizinhas em 1 |
| Vento     | Define direção predominante                        | 1 direção ativa → propagação linear  / 2 ou mais direções → propagação exponencial       |


### Equações Específicas
A previsão da área queimada ao longo do tempo pode ser modelada de forma simplificada por:

```python
# Crescimento Linear (1 direção)
def previsao_linear(t,r):
    total_regenerado = sum(R(i) for i in range(1, t+1))
    return (t + 1) - total_regenerado

# Crescimento Exponencial (2+ direções)
def previsao_exponencial(t, k, r):
    total_regenerado = sum(R(i) for i in range(1, t+1))
    return (k ** t) - total_regenerado
```
R(i) pode ser modelado como E(i) * r, onde E(i) é o número de encontros com água na iteração i e r é a média de células regeneradas por encontro (até 4 adjacentes).

Taxa teórica em matriz livre, na prática pode reduzida por obstáculos, como as dimensões da matriz.



---

## Algoritmos que poderiam ser utilizados para Melhoria de Desempenho

### 1. Busca em Largura (BFS)
#### Aplicação:
- Otimização de rotas do animal para evitar fogo e encontrar água.  
- **Vantagem**: Encontra caminhos seguros mais curtos em `O(N+M)`.  

### 2. Autômato Celular Probabilístico
#### Aplicação:
- **Modelagem realista de ignição**: 70% de chance de propagação com vento favorável, 30% contra, por exemplo.  
- **Vantagem**: Simula imprevisibilidade climática sem sobrecarga computacional.  

### 3. A* (A-Estrela) Adaptativo
#### Aplicação:
- **Fuga estratégica do animal**: Considera risco futuro de propagação priorizando caminhos promissores e evitando explorar caminhos desnecessários.  
- **Vantagem**: Reduziria a taxa de mortalidade do animal nas simulações.  
  
#### Comparação de Desempenho:
| Algoritmo         | Complexidade | Aplicabilidade          | Impacto na Simulação      |
|-------------------|--------------|-------------------------|---------------------------|
| BFS               | O(N+M)       | Movimento do animal     | + eficiência em rotas de movimentação   |
| Autômato Celular  | O(N²)        | Propagação do fogo      | + realismo                |
| A*                | O(N log N)   | Fuga estratégica        | + sobrevivência do animal       |

---

## Conclusão
Este projeto entrega uma maneira de se estudar a propagação de incêndios florestais sob diferentes condições —  ambiente sem vento ou com vento em direções específicas . Ao modelar cada árvore, foco de fogo e célula de água em uma matriz 2D, a simulação revela padrões geométricos claros que facilitam a compreensão de processos complexos de difusão e queima.

Além de seu valor acadêmico no estudo de dinâmica de incêndios, serve como uma excelente fonte de estudos para testar e comparar algoritmos de busca (BFS, A*, etc.) e modelos probabilísticos de autômatos celulares. Com isso, sendo possível:
- **Avaliar heurísticas de rota** para agentes móveis em ambientes perigosos.  
- **Analisar impactos do vento** e da distribuição de recursos hídricos na contenção do fogo.  
- **Experimentar variações paramétricas** (tamanho da matriz, número de iterações, direções do vento) em questão de segundos.

Graças à arquitetura modular — dividida em classes de configuração, leitura/gravação de arquivos, lógica de animal e dinâmica da floresta — você pode estender facilmente o código para caso queira:
- Incluir múltiplos animais com comportamentos cooperativos.  
- Adotar modelos climáticos mais realistas (mudança de direção e intensidade do vento ao longo do tempo).  
- Integrar interfaces gráficas ou visualizações em tempo real mais elaboradas.


---

## Compilação e Execução
Essa simulação de propagação de incêndios possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

| Comando       | Função                                                                 |
|---------------|------------------------------------------------------------------------|
| `make clean`  | Apaga a última compilação realizada contida na pasta `build`           |
| `make`        | Executa a compilação do programa utilizando o `gcc`, gerando em `build`|
| `make run`    | Executa o programa compilado na pasta `build`                          |

---
## Referências
- https://pt.stackoverflow.com/questions/146976/por-que-o-custo-de-complexidade-de-uma-bfs-é-onm
- https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/bfs.html
- https://www.esalq.usp.br/lepse/imgs/conteudo_thumb/Aut-matos-Celulares.pdf
- https://pt.stackoverflow.com/questions/328048/como-é-o-funcionamento-básico-do-algoritmo-a
- https://www.datacamp.com/pt/tutorial/a-star-algorithm

---
## Autores e Contatos
Autor: Vinícius Ramalho de Oliveira
- E-mail: ramalhooliveiravini@gmail.com
- Instagram: @_ vinicius.ro_





