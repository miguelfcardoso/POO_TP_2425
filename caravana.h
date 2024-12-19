#ifndef CARAVANA_H
#define CARAVANA_H

#include <string>
#include <vector>

class Caravana {
public:
    enum Tipo {
        COMERCIO,
        MILITAR,
        SECRETA
    };

private:
    int id;
    int x, y;
    int tripulantes;
    int capacidadeCarga;
    bool destruida;
    int maxTripulantes;
    Tipo tipo;
    int mercadoria;
    int agua;
    int maxAgua;
    bool autogestao;
    char ultimaDirecao;
    static int mapaLinhas;
    static int mapaColunas;

public:
    Caravana(); // Add default constructor
    Caravana(int id, int x, int y);
    Caravana(int id, int x, int y, Tipo tipo);
    void mover(char direcao);
    void moverAutonomo();
    void moverSemTripulantes();
    void adicionarTripulantes(int n);
    void venderMercadoria(int precoVenda);
    void comprarMercadoria(int quantidade, int precoCompra);
    void consumirAgua();
    void mostrarDetalhes() const;
    void destruir();
    bool isDestruida() const;
    void setPos(int novoX, int novoY);
    void adicionarAgua(int quantidade);
    
    int getTripulantes() const;
    int getMaxTripulantes() const;
    int getAgua() const;
    Tipo getTipo() const;
    int getMercadoria() const;
    int getCapacidadeCarga() const;
    int getId() const;
    int getX() const { return x; }
    int getY() const { return y; }
    void setAutogestao(bool estado) { autogestao = estado; }
    bool getAutogestao() const { return autogestao; }
    void reabastecerAgua();
    bool podeMoverse() const;
    int getMaxAgua() const;
    static void setMapaDimensoes(int l, int c) {
        mapaLinhas = l;
        mapaColunas = c;
    }
};

#endif
