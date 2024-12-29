#ifndef ITEM_H
#define ITEM_H

#include <string>

class Caravana;

class Item {
public:
    enum Tipo {
        CAIXA_PANDORA,
        ARCA_TESOURO,
        JAULA,
        MINA,
        SURPRESA
    };

private:
    Tipo tipo;
    int x;
    int y;
    int duracao;

public:
    Item(Tipo tipo, int x, int y, int duracao);
    void reduzirDuracao();
    int getDuracao() const;
    int getX() const;
    int getY() const;
    Tipo getTipo() const;
    void aplicarEfeito(Caravana& caravana);
    static std::string getTipoNome(Tipo tipo);
    static Item* criarItemAleatorio(int x, int y, int duracao);
    bool estaAdjacente(int x, int y) const;
};

#endif
