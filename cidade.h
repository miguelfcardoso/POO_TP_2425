#ifndef CIDADE_H
#define CIDADE_H

#include <iostream>  // Add this for std::cout and std::endl
#include <string>
#include <vector>
#include <map>  // Add this include
#include <algorithm>  // Add for std::remove_if
#include "caravana.h"

// Remove mapa.h include to break circular dependency
// #include "mapa.h"

class Cidade {
private:
    char nome;
    int x, y;  // Add coordinates
    std::vector<Caravana> caravanas;

public:
    // Remove default constructor and use single constructor with defaults
    Cidade(char nome = ' ', int x = 0, int y = 0) : nome(nome), x(x), y(y) {
        std::cout << "Creating city " << nome << " at position (" << x << "," << y << ")" << std::endl;
    }
    void adicionarCaravana(const Caravana& caravana);
    void removerCaravana(int id);
    void listarCaravanas() const;
    char getNome() const;
    int getProximaCaravanaId() const;  // Remove the Mapa parameter
    int getX() const;
    int getY() const;
    void removerCaravana(Caravana::Tipo tipo);
    void setPos(int newX, int newY) {
        x = newX;
        y = newY;
    }
    void atualizarCaravanas(const std::map<int, Caravana>& todasCaravanas); // Add this method
};

#endif