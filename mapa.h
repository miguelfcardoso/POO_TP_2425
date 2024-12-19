#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <map>
#include <utility>
#include "caravana.h"
#include "barbaro.h"
#include "cidade.h"
#include "item.h"
#include "buffer.h"
#include <cstring>

class Simulacao; // Forward declaration

class Mapa {
private:
    char** mapa;  // Replace vector<vector<char>>
    int linhas, colunas;
    std::map<char, Cidade> cidades;  // Changed from std::pair<int, int>
    std::map<int, Caravana> caravanas;
    std::vector<Barbaro> barbaros;
    std::vector<Item> itens;

public:
    Mapa(int l, int c);
    void adicionarElemento(int x, int y, char tipo);
    void exibirMapa();
    void exibirParaBuffer(Buffer& buffer) const;
    void adicionarCaravana(int id, int x, int y);
    void adicionarCaravana(int id, int x, int y, Caravana::Tipo tipo);
    void adicionarBarbaro(int x, int y);
    void moverCaravana(int id, char direcao);
    std::map<int, Caravana>& getCaravanas() { return caravanas; }  // Non-const version
    const std::map<int, Caravana>& getCaravanas() const;  // Const version
    void tempestadeDeAreia(int centroX, int centroY, int raio);
    bool posicaoValida(int x, int y) const;
    void adicionarCidade(char nome, int x, int y);
    bool isCidade(int x, int y) const;
    Cidade* getCidade(char nome);
    void adicionarItem(Item::Tipo tipo, int x, int y, int duracao);
    void atualizarItens();
    Item* getItemAdjacente(int x, int y);
    int getLinhas() const;
    int getColunas() const;
    void removerItem(int x, int y);
    const std::vector<Barbaro>& getBarbaros() const;
    bool isPosicaoVazia(int x, int y) const;
    int contarItens() const;
    void verificarInteracoesItens(Simulacao& simulacao);
    virtual ~Mapa();
    Mapa(const Mapa& other);
    Mapa& operator=(const Mapa& other);
};

#endif
