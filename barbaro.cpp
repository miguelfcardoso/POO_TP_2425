#include "barbaro.h"
#include <cstdlib>

Barbaro::Barbaro(int x, int y) : x(x), y(y), tripulantes(40), agua(0), destruido(false) {}

void Barbaro::moverAutonomo(int targetX, int targetY) {
    if (destruido) return;
    
    // Move towards target if within 8 positions
    if (abs(targetX - x) <= 8 && abs(targetY - y) <= 8) {
        if (x < targetX) x++;
        else if (x > targetX) x--;
        if (y < targetY) y++;
        else if (y > targetY) y--;
    } else {
        // Random movement
        int dir = rand() % 4;
        switch(dir) {
            case 0: x++; break;
            case 1: x--; break;
            case 2: y++; break;
            case 3: y--; break;
        }
    }
}

void Barbaro::adicionarTripulantes(int n) {
    tripulantes += n;
    if (tripulantes <= 0) {
        destruir();
    }
}

void Barbaro::adicionarAgua(int quantidade) {
    agua += quantidade;
}

void Barbaro::destruir() {
    destruido = true;
    tripulantes = 0;
    agua = 0;
}

bool Barbaro::isDestruido() const {
    return destruido;
}

int Barbaro::getTripulantes() const {
    return tripulantes;
}

int Barbaro::getAgua() const {
    return agua;
}

void Barbaro::setPos(int novoX, int novoY) {
    x = novoX;
    y = novoY;
}
