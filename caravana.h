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
    int x;
    int y;
    int tripulantes;
    int maxTripulantes;
    int capacidadeCarga;
    int mercadoria;
    int agua;
    int maxAgua;
    bool destruida;
    bool autogestao;
    Tipo tipo;
    char ultimaDirecao;
    int turnosSemTripulantes;
    int movimentosRestantes;
    int itemProximoX;
    int itemProximoY;
    int barbaroProximoX;
    int barbaroProximoY;
    
    static const int MAX_MOVIMENTOS_COMERCIO = 2;
    static const int MAX_MOVIMENTOS_MILITAR = 3;
    static const int MAX_MOVIMENTOS_SECRETA = 1;

public:
    static const int DISTANCIA_DETECAO_ITEM = 2;
    static const int DISTANCIA_DETECAO_BARBARO = 6;
    static int mapaLinhas;
    static int mapaColunas;

    Caravana(int id, int x, int y, Tipo tipo);
    Caravana(int id, int x, int y);
    Caravana();

    // Movement methods
    void mover(char direcao, const std::pair<int, int>& destino = {-1, -1});
    void moverParaDestino(int destX, int destY);
    void moverParaCaravanaProxima();
    void moverAutonomo();
    void moverSemTripulantes();
    void resetarMovimentos();
    bool podeSeMovimentar() const;

    // State methods
    void adicionarTripulantes(int n);
    void venderMercadoria(int precoVenda);
    void comprarMercadoria(int quantidade, int precoCompra);
    void consumirAgua();
    void reabastecerAgua();
    void mostrarDetalhes() const;
    void destruir();
    void processarTempestedeAreia();
    void atualizarTurnosSemTripulantes();
    void gerenciarRecursos();

    // Getters and setters
    int getId() const;
    int getTripulantes() const;
    int getMaxTripulantes() const;
    int getMercadoria() const;
    int getCapacidadeCarga() const;
    int getAgua() const;
    Tipo getTipo() const;
    bool isDestruida() const;
    bool getAutogestao() const;
    void setAutogestao(bool valor);
    void setPos(int novoX, int novoY);
    void adicionarAgua(int quantidade);
    int getX() const { return x; }
    int getY() const { return y; }

    // Detection methods
    bool podeApanharItem() const;
    bool podeVerItem(int itemX, int itemY) const;
    bool podeVerBarbaro(int barbaroX, int barbaroY) const;

private:
    int calcularConsumoAgua() const;
    void perderTripulante();
};

#endif
