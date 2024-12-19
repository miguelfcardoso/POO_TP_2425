#include "cidade.h"
#include <iostream>

// Remove constructor since it's already defined in the header
// Cidade::Cidade(char nome, int x, int y) : nome(nome), x(x), y(y) { ... }

void Cidade::adicionarCaravana(const Caravana& caravana) {
    std::cout << "Adding caravana to city " << nome << " at position (" << x << "," << y << ")" << std::endl;
    caravanas.push_back(caravana);
}

void Cidade::removerCaravana(int id) {
    caravanas.erase(std::remove_if(caravanas.begin(), caravanas.end(),
                                   [id](const Caravana& c) { return c.getId() == id; }),
                    caravanas.end());
}

void Cidade::listarCaravanas() const {
    std::cout << "Caravanas na cidade " << nome << ":" << std::endl;
    if (caravanas.empty()) {
        std::cout << "Nenhuma caravana nesta cidade." << std::endl;
    } else {
        for (const auto& caravana : caravanas) {
            caravana.mostrarDetalhes();
        }
    }
}

char Cidade::getNome() const {
    return nome;
}

int Cidade::getProximaCaravanaId() const {
    static int nextId = 0;
    return nextId++;
}

int Cidade::getX() const {
    return x;
}

int Cidade::getY() const {
    return y;
}

void Cidade::removerCaravana(Caravana::Tipo tipo) {
    auto it = std::find_if(caravanas.begin(), caravanas.end(),
        [tipo](const Caravana& c) { return c.getTipo() == tipo; });
    if (it != caravanas.end()) {
        caravanas.erase(it);
    }
}

void Cidade::atualizarCaravanas(const std::map<int, Caravana>& todasCaravanas) {
    caravanas.clear();
    for (const auto& [id, caravana] : todasCaravanas) {
        // Debug output to help track caravana positions
        std::cout << "Checking caravana " << id << " at (" << caravana.getX() 
                 << "," << caravana.getY() << ") against city at (" 
                 << x << "," << y << ")" << std::endl;
                 
        if (caravana.getX() == x && caravana.getY() == y && !caravana.isDestruida()) {
            caravanas.push_back(caravana);
            std::cout << "Added caravana " << id << " to city " << nome << std::endl;
        }
    }
}