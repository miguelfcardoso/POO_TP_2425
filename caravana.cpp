#include <iostream>
#include "caravana.h"

// Initialize static members
int Caravana::mapaLinhas = 10;
int Caravana::mapaColunas = 20;

Caravana::Caravana(int id, int x, int y, Tipo tipo) 
    : id(id), x(x), y(y), tipo(tipo), 
      mercadoria(0), destruida(false), autogestao(false), 
      ultimaDirecao('N'), movimentosRestantes(0) {
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
    resetarMovimentos();  // Initialize movement points
}

Caravana::Caravana(int id, int x, int y)
    : id(id), x(x), y(y), tripulantes(10), capacidadeCarga(100),
      destruida(false), maxTripulantes(20), tipo(COMERCIO),
      mercadoria(0), maxAgua(200), agua(200), autogestao(false),
      ultimaDirecao('N'), turnosSemTripulantes(0), movimentosRestantes(MAX_MOVIMENTOS_COMERCIO) {}

Caravana::Caravana() : id(0), x(0), y(0), tripulantes(0), capacidadeCarga(0),
    destruida(false), maxTripulantes(0), tipo(COMERCIO), mercadoria(0), agua(0) {}

void Caravana::resetarMovimentos() {
    switch (tipo) {
        case COMERCIO: movimentosRestantes = MAX_MOVIMENTOS_COMERCIO; break;
        case MILITAR: movimentosRestantes = MAX_MOVIMENTOS_MILITAR; break;
        case SECRETA: movimentosRestantes = MAX_MOVIMENTOS_SECRETA; break;
    }
}

bool Caravana::podeSeMovimentar() const {
    return movimentosRestantes > 0 && !destruida;
}

// Optimize movement system by unifying different movement types
void Caravana::mover(char direcao, const std::pair<int, int>& destino) {
    if (!podeSeMovimentar()) return;
    
    int novoX = x, novoY = y;
    
    if (destino.first != -1) {
        // Intelligent pathfinding towards destination
        if (x < destino.first) novoX++;
        else if (x > destino.first) novoX--;
        if (y < destino.second) novoY++;
        else if (y > destino.second) novoY--;
    } else {
        // Regular directional movement
        switch(direcao) {
            case 'C': novoX = (novoX - 1 + mapaLinhas) % mapaLinhas; break;
            case 'B': novoX = (novoX + 1) % mapaLinhas; break;
            case 'E': novoY = (novoY - 1 + mapaColunas) % mapaColunas; break;
            case 'D': novoY = (novoY + 1) % mapaColunas; break;
            case 'Q': // CE
                novoX = (novoX - 1 + mapaLinhas) % mapaLinhas;
                novoY = (novoY - 1 + mapaColunas) % mapaColunas;
                break;
            case 'W': // CD
                novoX = (novoX - 1 + mapaLinhas) % mapaLinhas;
                novoY = (novoY + 1) % mapaColunas;
                break;
            case 'A': // BE
                novoX = (novoX + 1) % mapaLinhas;
                novoY = (novoY - 1 + mapaColunas) % mapaColunas;
                break;
            case 'S': // BD
                novoX = (novoX + 1) % mapaLinhas;
                novoY = (novoY + 1) % mapaColunas;
                break;
        }
    }
    
    // Update position directly instead of using atualizarPosicao
    x = novoX;
    y = novoY;
    movimentosRestantes--;
    ultimaDirecao = direcao;
}

void Caravana::moverParaDestino(int destX, int destY) {
    // Move one step towards destination
    if (x < destX) mover('B');
    else if (x > destX) mover('C');
    else if (y < destY) mover('D');
    else if (y > destY) mover('E');
}

void Caravana::moverParaCaravanaProxima() {
    // Simple following behavior
    mover(ultimaDirecao);
}

void Caravana::moverAutonomo() {
    if (!autogestao || destruida) return;

    switch (tipo) {
        case COMERCIO:
            // Move towards items or stay near other caravanas
            if (podeApanharItem()) {
                // Move towards nearest item within 2 cells
                moverParaDestino(itemProximoX, itemProximoY);
            } else {
                // Stay near friendly caravana for protection
                moverParaCaravanaProxima();
            }
            break;

        case MILITAR:
            if (podeVerBarbaro(barbaroProximoX, barbaroProximoY)) {
                // Move towards barbarian if within 6 cells
                moverParaDestino(barbaroProximoX, barbaroProximoY);
            }
            break;

        case SECRETA:
            // Special movement pattern here
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

// Unify resource management
void Caravana::gerenciarRecursos() {
    // Combine:
    // - consumirAgua()
    // - atualizarTurnosSemTripulantes()
    // - calcularConsumoAgua()
    // Into a single resource management function
}

void Caravana::consumirAgua() {
    if (destruida) return;

    // Calculate water consumption based on crew size
    int consumo = calcularConsumoAgua();
    agua = std::max(0, agua - consumo);

    // Lose crew if no water
    if (agua == 0 && tripulantes > 0) {
        perderTripulante();
    }
}

int Caravana::calcularConsumoAgua() const {
    if (tripulantes == 0) return 0;
    
    switch (tipo) {
        case COMERCIO:
            return (tripulantes > maxTripulantes/2) ? 2 : 1;
        case MILITAR:
            return (tripulantes > maxTripulantes/2) ? 3 : 1;
        case SECRETA:
            return 1;
        default:
            return 1;
    }
}

void Caravana::perderTripulante() {
    if (tripulantes > 0) {
        tripulantes--;
        if (tripulantes == 0) {
            atualizarTurnosSemTripulantes();
        }
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

void Caravana::processarTempestedeAreia() {
    if (destruida) return;
    
    switch (tipo) {
        case COMERCIO:
            if (mercadoria > capacidadeCarga/2) {
                if (rand() % 100 < 50) destruir();
            } else if (rand() % 100 < 25) {
                destruir();
            }
            if (!destruida) mercadoria = mercadoria * 0.75;
            break;
            
        case MILITAR:
            tripulantes = tripulantes * 0.9;
            if (rand() % 100 < 33) destruir();
            break;
            
        case SECRETA:
            // Custom sandstorm handling for secret type
            break;
    }
}

void Caravana::atualizarTurnosSemTripulantes() {
    if (tripulantes <= 0) {
        turnosSemTripulantes++;
        if ((tipo == MILITAR && turnosSemTripulantes >= 7) ||
            (tipo != MILITAR && turnosSemTripulantes >= 5)) {
            destruir();
        }
    } else {
        turnosSemTripulantes = 0;
    }
}

bool Caravana::podeApanharItem() const {
    return !destruida && tripulantes > 0;
}

bool Caravana::podeVerItem(int itemX, int itemY) const {
    return abs(x - itemX) <= DISTANCIA_DETECAO_ITEM && 
           abs(y - itemY) <= DISTANCIA_DETECAO_ITEM;
}

bool Caravana::podeVerBarbaro(int barbaroX, int barbaroY) const {
    return abs(x - barbaroX) <= DISTANCIA_DETECAO_BARBARO && 
           abs(y - barbaroY) <= DISTANCIA_DETECAO_BARBARO;
}

bool Caravana::getAutogestao() const {
    return autogestao;
}

void Caravana::setAutogestao(bool valor) {
    autogestao = valor;
}
