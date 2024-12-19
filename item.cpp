#include "item.h"
#include "caravana.h"
#include <cstdlib>

Item::Item(Tipo tipo, int x, int y, int duracao) 
    : tipo(tipo), x(x), y(y), duracao(duracao) {}

void Item::reduzirDuracao() {
    duracao--;
}

int Item::getDuracao() const {
    return duracao;
}

int Item::getX() const {
    return x;
}

int Item::getY() const {
    return y;
}

Item::Tipo Item::getTipo() const {
    return tipo;
}

void Item::aplicarEfeito(Caravana& caravana) {
    switch (tipo) {
        case CAIXA_PANDORA:
            // Mata 20% da tripulação
            caravana.adicionarTripulantes(-(caravana.getTripulantes() * 0.2));
            break;
            
        case ARCA_TESOURO:
            // Aumento de 10% das moedas é tratado na classe Simulacao
            break;
            
        case JAULA:
            // Adiciona tripulantes sem exceder o máximo
            {
                int espacoDisponivel = caravana.getMaxTripulantes() - caravana.getTripulantes();
                if (espacoDisponivel > 0) {
                    int novosTrip = std::min(5, espacoDisponivel);  // Adiciona até 5 tripulantes
                    caravana.adicionarTripulantes(novosTrip);
                }
            }
            break;
            
        case MINA:
            // Destroi a caravana
            caravana.destruir();
            break;
            
        case SURPRESA:
            // Efeito surpresa: Troca aleatoriamente a posição da caravana com outra
            // (Este é apenas um exemplo - você pode implementar outro efeito surpresa)
            {
                int novoX = rand() % 10;  // Assumindo mapa 10x20
                int novoY = rand() % 20;
                caravana.setPos(novoX, novoY);
            }
            break;
    }
}

std::string Item::getTipoNome(Tipo tipo) {
    switch (tipo) {
        case CAIXA_PANDORA: return "Caixa de Pandora";
        case ARCA_TESOURO: return "Arca do Tesouro";
        case JAULA: return "Jaula";
        case MINA: return "Mina";
        case SURPRESA: return "Surpresa";
        default: return "Desconhecido";
    }
}
