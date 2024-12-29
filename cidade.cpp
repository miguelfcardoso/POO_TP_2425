#include "cidade.h"
#include "caravana.h"  // Add this include to access static members

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
    for (const auto& [id, car] : todasCaravanas) {
        // Use Caravana's static members for dimensions
        int carX = ((car.getX() % Caravana::mapaLinhas) + Caravana::mapaLinhas) % Caravana::mapaLinhas;
        int carY = ((car.getY() % Caravana::mapaColunas) + Caravana::mapaColunas) % Caravana::mapaColunas;
        int cityX = ((x % Caravana::mapaLinhas) + Caravana::mapaLinhas) % Caravana::mapaLinhas;
        int cityY = ((y % Caravana::mapaColunas) + Caravana::mapaColunas) % Caravana::mapaColunas;
        
        if (carX == cityX && carY == cityY && !car.isDestruida()) {
            caravanas.push_back(car);
            std::cout << "Added caravana " << id << " to city " << nome << std::endl;
        }
    }
}