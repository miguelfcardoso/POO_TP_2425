#ifndef CARAVANA_H
#define CARAVANA_H

#include <string>
#include <vector>

class Caravana {
private:
    int id;
    int x, y;
    int tripulantes;
    int capacidadeCarga;

public:
    Caravana(int id, int x, int y);
    void mover(char direcao);
    void adicionarTripulantes(int n);
    void venderMercadoria();
    void comprarMercadoria(int quantidade);
    void mostrarDetalhes();
};

#endif
