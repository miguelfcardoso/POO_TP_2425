#include <iostream>
#include "caravana.h"

// Initialize static members
int Caravana::mapaLinhas = 10;
int Caravana::mapaColunas = 20;

Caravana::Caravana(int id, int x, int y, Tipo tipo) : id(id), x(x), y(y), tipo(tipo), 
    mercadoria(0), destruida(false), autogestao(false), ultimaDirecao('N') {
    switch (tipo) {
        case COMERCIO:
            tripulantes = 20;
            maxTripulantes = 30;
            capacidadeCarga = 40;
            maxAgua = 200;
            agua = maxAgua;
            break;
        case MILITAR:
            tripulantes = 40;
            maxTripulantes = 60;
            capacidadeCarga = 5;
            maxAgua = 400;
            agua = maxAgua;
            break;
        case SECRETA:
            // Definir características da caravana secreta
            tripulantes = 45;
            maxTripulantes = 45;
            break;
    }
}

Caravana::Caravana(int id, int x, int y)
    : id(id), x(x), y(y), tripulantes(10), capacidadeCarga(100),
      destruida(false), maxTripulantes(20), tipo(COMERCIO),
      mercadoria(0), agua(100) {}

Caravana::Caravana() : id(0), x(0), y(0), tripulantes(0), capacidadeCarga(0),
    destruida(false), maxTripulantes(0), tipo(COMERCIO), mercadoria(0), agua(0) {}

void Caravana::mover(char direcao) {
    int novoX = x, novoY = y;
    switch(direcao) {
        case 'D': novoY = (novoY + 1) % mapaColunas; break;
        case 'E': novoY = (novoY - 1 + mapaColunas) % mapaColunas; break;
        case 'C': novoX = (novoX - 1 + mapaLinhas) % mapaLinhas; break;
        case 'B': novoX = (novoX + 1) % mapaLinhas; break;
        // For diagonal moves, use separate chars
        case 'Q': // CE (cima-esquerda)
            novoX = (novoX - 1 + mapaLinhas) % mapaLinhas;
            novoY = (novoY - 1 + mapaColunas) % mapaColunas;
            break;
        case 'W': // CD (cima-direita)
            novoX = (novoX - 1 + mapaLinhas) % mapaLinhas;
            novoY = (novoY + 1) % mapaColunas;
            break;
        case 'A': // BE (baixo-esquerda)
            novoX = (novoX + 1) % mapaLinhas;
            novoY = (novoY - 1 + mapaColunas) % mapaColunas;
            break;
        case 'S': // BD (baixo-direita)
            novoX = (novoX + 1) % mapaLinhas;
            novoY = (novoY + 1) % mapaColunas;
            break;
    }
    x = novoX;
    y = novoY;
    ultimaDirecao = direcao;
}

void Caravana::moverAutonomo() {
    if (!autogestao || destruida) return;

    switch (tipo) {
        case COMERCIO:
            // Try to stay near other caravanas or go for items
            // Implementation here...
            break;
            
        case MILITAR:
            // Stay still unless there's a barbarian nearby
            // Implementation here...
            break;
            
        case SECRETA:
            // Custom behavior
            break;
    }
}

void Caravana::moverSemTripulantes() {
    if (tripulantes > 0) return;
    
    // Random movement for caravana without crew
    char directions[] = {'C', 'B', 'E', 'D'};
    char dir = directions[rand() % 4];
    mover(dir);
}

void Caravana::adicionarTripulantes(int n) {
    tripulantes += n;
}

void Caravana::venderMercadoria(int precoVenda) {
    mercadoria = 0;  // Clear merchandise after selling
}

void Caravana::comprarMercadoria(int quantidade, int precoCompra) {
    if (mercadoria + quantidade <= capacidadeCarga) {
        mercadoria += quantidade;
        // Subtrair quantidade * precoCompra das moedas do usuário
    }
}

void Caravana::consumirAgua() {
    if (destruida) return;
    
    int consumo = 1;  // Base consumption
    switch (tipo) {
        case COMERCIO:
            consumo = (tripulantes > maxTripulantes/2) ? 2 : 1;
            break;
        case MILITAR:
            consumo = (tripulantes > maxTripulantes/2) ? 3 : 2;
            break;
        case SECRETA:
            consumo = 1;
            break;
    }
    
    agua = std::max(0, agua - consumo);
    if (agua == 0 && tripulantes > 0) {
        adicionarTripulantes(-1);
    }
}

void Caravana::reabastecerAgua() {
    agua = maxAgua;
}

void Caravana::mostrarDetalhes() const {
    std::cout << "Caravana " << id << ":\n"
              << "Tripulantes: " << tripulantes << "/" << maxTripulantes << "\n"
              << "Mercadoria: " << mercadoria << "/" << capacidadeCarga << "\n"
              << "Água: " << agua << "\n";
}

int Caravana::getId() const {
    return id;
}

int Caravana::getTripulantes() const {
    return tripulantes;
}

Caravana::Tipo Caravana::getTipo() const {
    return tipo;
}

int Caravana::getMercadoria() const {
    return mercadoria;
}

int Caravana::getCapacidadeCarga() const {
    return capacidadeCarga;
}

void Caravana::destruir() {
    destruida = true;
    tripulantes = 0;
    agua = 0;
    mercadoria = 0;
}

bool Caravana::isDestruida() const {
    return destruida;
}

int Caravana::getMaxTripulantes() const {
    return maxTripulantes;
}

void Caravana::setPos(int novoX, int novoY) {
    x = novoX;
    y = novoY;
}

int Caravana::getAgua() const {
    return agua;
}

void Caravana::adicionarAgua(int quantidade) {
    agua += quantidade;
}
