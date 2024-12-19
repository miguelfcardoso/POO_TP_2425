#include <iostream>
#include "mapa.h"
#include "caravana.h"
#include "barbaro.h"

Mapa::Mapa(int l, int c) : linhas(l), colunas(c) {
    mapa.resize(linhas, std::vector<char>(colunas, '.'));
}

void Mapa::adicionarElemento(int x, int y, char tipo) {
    mapa[x][y] = tipo;
    if (tipo >= 'a' && tipo <= 'z') {
        cidades[tipo] = {x, y};
    } else if (tipo >= '0' && tipo <= '9') {
        int id = tipo - '0';
        caravanas[id] = Caravana(id, x, y); 
    } else if (tipo == '!') {
        barbaros.push_back(Barbaro(x, y));
    }
}

void Mapa::exibirMapa() {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            std::cout << mapa[i][j];
        }
        std::cout << std::endl;
    }
}
