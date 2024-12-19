#ifndef BARBARO_H
#define BARBARO_H

class Barbaro {
private:
    int x, y;
    int tripulantes;
    int agua;
    bool destruido;

public:
    Barbaro(int x, int y);
    void moverAutonomo(int targetX, int targetY);
    void adicionarTripulantes(int n);
    void adicionarAgua(int quantidade);
    void destruir();
    bool isDestruido() const;
    
    int getTripulantes() const;
    int getAgua() const;
    int getX() const { return x; }
    int getY() const { return y; }
    void setPos(int novoX, int novoY);
};

#endif
