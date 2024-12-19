#include <iostream>
#include "caravana.h"

Caravana::Caravana(int id, int x, int y) : id(id), x(x), y(y), tripulantes(10), capacidadeCarga(100) {}

void Caravana::mover(char direcao) {
    switch(direcao) {
        case 'D': x++; break;
        case 'E': x--; break;
        case 'C': y--; break;
        case 'B': y++; break;
        default: break;
    }
}

void Caravana::adicionarTripulantes(int n) {
    tripulantes += n;
}

void Caravana::venderMercadoria() {
    // Lógica de venda de mercadoria
}

void Caravana::comprarMercadoria(int quantidade) {
    // Lógica de compra de mercadoria
}

void Caravana::mostrarDetalhes() {
    std::cout << "Caravana ID: " << id << std::endl;
    std::cout << "Posição: (" << x << ", " << y << ")" << std::endl;
    std::cout << "Tripulantes: " << tripulantes << std::endl;
    std::cout << "Capacidade de carga: " << capacidadeCarga << std::endl;
}
