#include <iostream>
#include "mapa.h"
#include "caravana.h"
#include "barbaro.h"
#include "cidade.h"  // Add this include here instead of in mapa.h
#include "simulacao.h"
#include <cstring>

Mapa::Mapa(int l, int c) : linhas(l), colunas(c) {
    mapa = new char*[linhas];
    for (int i = 0; i < linhas; ++i) {
        mapa[i] = new char[colunas];
        for (int j = 0; j < colunas; ++j) {
            mapa[i][j] = '.';
        }
    }
}

// Add destructor
Mapa::~Mapa() {
    for (int i = 0; i < linhas; ++i) {
        delete[] mapa[i];
    }
    delete[] mapa;
}

// Add copy constructor
Mapa::Mapa(const Mapa& other) : linhas(other.linhas), colunas(other.colunas) {
    mapa = new char*[linhas];
    for (int i = 0; i < linhas; ++i) {
        mapa[i] = new char[colunas];
        memcpy(mapa[i], other.mapa[i], colunas);
    }
}

// Add assignment operator
Mapa& Mapa::operator=(const Mapa& other) {
    if (this != &other) {
        // Delete old data
        for (int i = 0; i < linhas; ++i) {
            delete[] mapa[i];
        }
        delete[] mapa;
        
        // Copy new data
        linhas = other.linhas;
        colunas = other.colunas;
        cidades = other.cidades;      // Copy cities
        caravanas = other.caravanas;  // Copy caravans
        barbaros = other.barbaros;    // Copy barbarians
        itens = other.itens;          // Copy items
        
        mapa = new char*[linhas];
        for (int i = 0; i < linhas; ++i) {
            mapa[i] = new char[colunas];
            memcpy(mapa[i], other.mapa[i], colunas);
        }
    }
    return *this;
}

void Mapa::adicionarElemento(int x, int y, char tipo) {
    if (x < 0 || x >= linhas || y < 0 || y >= colunas) return;
    
    mapa[x][y] = tipo;
    if (tipo >= '0' && tipo <= '9') {
        int id = tipo - '0';
        caravanas[id] = Caravana(id, x, y);
        std::cout << "Added caravana " << id << " at position (" << x << "," << y << ")" << std::endl;
    } else if (tipo == '!') {
        barbaros.push_back(Barbaro(x, y));
        std::cout << "Added barbaro at position (" << x << "," << y << ")" << std::endl;
    }
}

void Mapa::exibirMapa() {
    for (int i = 0; i < linhas; i++) {
        std::string linha;
        for (int j = 0; j < colunas; j++) {
            linha += mapa[i][j];
        }
        std::cout << linha << std::endl;  // Print complete line at once
    }
}

void Mapa::moverCaravana(int id, char direcao) {
    if (caravanas.find(id) == caravanas.end()) return;
    
    Caravana& caravana = caravanas[id];
    int novoX = caravana.getX();
    int novoY = caravana.getY();
    
    switch(direcao) {
        case 'D': novoY = (novoY + 1) % colunas; break;
        case 'E': novoY = (novoY - 1 + colunas) % colunas; break;
        case 'C': novoX = (novoX - 1 + linhas) % linhas; break;
        case 'B': novoX = (novoX + 1) % linhas; break;
        case 'Q': // CE
            novoX = (novoX - 1 + linhas) % linhas;
            novoY = (novoY - 1 + colunas) % colunas;
            break;
        case 'W': // CD
            novoX = (novoX - 1 + linhas) % linhas;
            novoY = (novoY + 1) % colunas;
            break;
        case 'A': // BE
            novoX = (novoX + 1) % linhas;
            novoY = (novoY - 1 + colunas) % colunas;
            break;
        case 'S': // BD
            novoX = (novoX + 1) % linhas;
            novoY = (novoY + 1) % colunas;
            break;
        default: return;
    }
    
    if (posicaoValida(novoX, novoY)) {
        // Check if current position was a city before replacing with dot
        bool oldPosWasCity = false;
        for (const auto& [nome, cidade] : cidades) {
            if (cidade.getX() == caravana.getX() && cidade.getY() == caravana.getY()) {
                oldPosWasCity = true;
                mapa[caravana.getX()][caravana.getY()] = nome;
                break;
            }
        }
        
        if (!oldPosWasCity) {
            mapa[caravana.getX()][caravana.getY()] = '.';
        }

        // Update new position
        char oldChar = mapa[novoX][novoY];  // Store the character at new position
        caravana.setPos(novoX, novoY);
        mapa[novoX][novoY] = '0' + id;

        // Update city tracking after movement
        for (auto& [nome, cidade] : cidades) {
            if (cidade.getX() == novoX && cidade.getY() == novoY) {
                cidade.atualizarCaravanas(caravanas);
                break;
            }
        }
        
        std::cout << "Moved caravana " << id << " to (" << novoX << "," << novoY << ")" << std::endl;
    }
}

const std::map<int, Caravana>& Mapa::getCaravanas() const {
    return caravanas;
}

void Mapa::tempestadeDeAreia(int centroX, int centroY, int raio) {
    for (int i = centroX - raio; i <= centroX + raio; ++i) {
        for (int j = centroY - raio; j <= centroY + raio; ++j) {
            int x = (i + linhas) % linhas;
            int y = (j + colunas) % colunas;
            if (mapa[x][y] == '.') {
                mapa[x][y] = '*'; // Representa uma tempestade de areia
            }
        }
    }
}

bool Mapa::posicaoValida(int x, int y) const {
    int nx = (x + linhas) % linhas;
    int ny = (y + colunas) % colunas;
    return mapa[nx][ny] == '.' || (mapa[nx][ny] >= 'a' && mapa[nx][ny] <= 'z');
}

bool Mapa::isPosicaoVazia(int x, int y) const {
    return mapa[x][y] == '.';
}

bool Mapa::isCidade(int x, int y) const {
    for (const auto& cidade : cidades) {
        if (cidade.second.getX() == x && cidade.second.getY() == y) {
            return true;
        }
    }
    return false;
}

Cidade* Mapa::getCidade(char nome) {
    auto it = cidades.find(nome);
    if (it != cidades.end()) {
        std::cout << "Found city " << nome << " at (" << it->second.getX() << "," << it->second.getY() << ")" << std::endl;
        it->second.atualizarCaravanas(caravanas);
        return &(it->second);
    }
    std::cout << "City " << nome << " not found" << std::endl;
    return nullptr;
}

void Mapa::adicionarItem(Item::Tipo tipo, int x, int y, int duracao) {
    if (itens.size() < 5 && mapa[x][y] == '.') {
        itens.emplace_back(tipo, x, y, duracao);
        mapa[x][y] = 'I'; // Representa um item
    }
}

void Mapa::atualizarItens() {
    std::cout << "Updating items... Current count: " << itens.size() << std::endl;
    for (auto it = itens.begin(); it != itens.end();) {
        it->reduzirDuracao();
        std::cout << "Item at (" << it->getX() << "," << it->getY() 
                  << ") duration: " << it->getDuracao() << std::endl;
        
        if (it->getDuracao() <= 0) {
            std::cout << "Removing expired item at (" << it->getX() << "," << it->getY() << ")" << std::endl;
            mapa[it->getX()][it->getY()] = '.';
            it = itens.erase(it);
        } else {
            ++it;
        }
    }
}

Item* Mapa::getItemAdjacente(int x, int y) {
    for (auto& item : itens) {
        if ((item.getX() == x && (item.getY() == y - 1 || item.getY() == y + 1)) ||
            (item.getY() == y && (item.getX() == x - 1 || item.getX() == x + 1))) {
            return &item;
        }
    }
    return nullptr;
}

int Mapa::getLinhas() const {
    return linhas;
}

int Mapa::getColunas() const {
    return colunas;
}

void Mapa::removerItem(int x, int y) {
    for (auto it = itens.begin(); it != itens.end(); ++it) {
        if (it->getX() == x && it->getY() == y) {
            itens.erase(it);
            mapa[x][y] = '.';
            break;
        }
    }
}

void Mapa::adicionarCaravana(int id, int x, int y) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas) {
        caravanas[id] = Caravana(id, x, y);
        mapa[x][y] = '0' + id;
    }
}

void Mapa::adicionarCaravana(int id, int x, int y, Caravana::Tipo tipo) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas) {
        std::cout << "Adding caravana with ID " << id << " at (" << x << "," << y << ")" << std::endl;
        if (caravanas.find(id) != caravanas.end()) {
            std::cout << "Warning: Overwriting existing caravana with ID " << id << std::endl;
        }
        caravanas[id] = Caravana(id, x, y, tipo);
        mapa[x][y] = '0' + id;
    }
}

const std::vector<Barbaro>& Mapa::getBarbaros() const {
    return barbaros;
}

void Mapa::adicionarBarbaro(int x, int y) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas && mapa[x][y] == '.') {
        barbaros.push_back(Barbaro(x, y));
        mapa[x][y] = '!';
    }
}

int Mapa::contarItens() const {
    return itens.size();
}

void Mapa::verificarInteracoesItens(Simulacao& simulacao) {
    for (auto& [id, caravana] : caravanas) {
        Item* item = getItemAdjacente(caravana.getX(), caravana.getY());
        if (item && item->getTipo() == Item::ARCA_TESOURO) {
            double moedas = simulacao.getMoedas();  // Now using public getter
            simulacao.adicionarMoedas(moedas * 0.1);
        }
    }
}

void Mapa::exibirParaBuffer(Buffer& buffer) const {
    buffer.esvaziar();
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            buffer.imprimir(mapa[i][j]);
        }
        if (i < linhas - 1) {  // Only add newline if not last line
            buffer.imprimir('\n');
        }
    }
}

void Mapa::adicionarCidade(char nome, int x, int y) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas 
        && cidades.find(nome) == cidades.end()) {  // Only add if city doesn't exist
        cidades[nome] = Cidade(nome, x, y);
        mapa[x][y] = nome;
        std::cout << "Added city " << nome << " at position (" << x << "," << y << ")" << std::endl;
    }
}

int Mapa::getNextAvailableId() const {
    for (int i = 0; i < 10; i++) {
        if (caravanas.find(i) == caravanas.end()) {
            std::cout << "Next available ID: " << i << std::endl;
            return i;
        }
    }
    std::cout << "No available IDs!" << std::endl;
    return -1;
}
