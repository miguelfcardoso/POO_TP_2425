#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <map>
#include <utility>
#include "caravana.h"
#include "barbaro.h"

class Mapa {
private:
    int linhas, colunas;
    std::vector<std::vector<char>> mapa;
    std::map<char, std::pair<int, int>> cidades;
    std::map<int, Caravana> caravanas;
    std::vector<Barbaro> barbaros;

public:
    Mapa(int l, int c);
    void adicionarElemento(int x, int y, char tipo);
    void exibirMapa();
    void adicionarCaravana(int id, int x, int y);
    void adicionarBarbaro(int x, int y);
};

#endif
