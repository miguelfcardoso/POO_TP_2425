#pragma once
#include <string>
#include <map>
#include "mapa.h"
#include "caravana.h"
#include "barbaro.h"
#include "buffer.h"

class Simulacao {
private:
    Mapa mapa;
    Buffer* buffer;  // Add buffer pointer
    std::map<std::string, Buffer*> bufferSaves;  // Add buffer saves map
    int moedas;
    int instantesEntreNovosItens;
    int duracaoItem;
    int maxItens;
    int precoVendaMercadoria;
    int precoCompraMercadoria;
    int precoCaravana;
    int instantesEntreNovosBarbaros;
    int duracaoBarbaros;
    int combatesVencidos;
    int instantesDecorridos;
    bool verificarCombate(int x1, int y1, int x2, int y2);  // Updated parameters
    void aplicarEfeitoItem(Caravana& caravana, Item::Tipo tipo);
    void realizarCombate(Caravana& caravana, Barbaro& barbaro);

public:
    Simulacao();
    ~Simulacao();
    void lerConfig(const std::string &nomeFicheiro);
    void exibirMapa();
    void iniciarSimulacao();
    void executarComandos(const std::string& comando);  // Update declaration
    void adicionarCaravana(int id, int x, int y);
    void adicionarCaravana(int id, int x, int y, Caravana::Tipo tipo);
    void passarInstantes(int instantes);
    void terminarSimulacao();
    void tempestadeDeAreia(int centroX, int centroY, int raio);
    void listarCaravanasCidade(char nome);
    void adicionarItem(Item::Tipo tipo, int x, int y, int duracao);
    void comprarTripulantes(int id, int quantidade);
    void venderMercadoria(int id);
    void comprarMercadoria(int id, int quantidade);
    void comprarCaravana(char cidade, Caravana::Tipo tipo);
    void exec(const std::string &nomeFicheiro);
    void prox(int n = 1);
    void comprac(char cidade, char tipo);
    void precos();
    void cidade(char nome);
    void caravana(int id);
    void compra(int id, int quantidade);
    void vende(int id);
    void move(int id, char direcao);
    void autoGestao(int id);
    void stop(int id);
    void barbaro(int x, int y);
    void areia(int x, int y, int raio);
    void adicionarMoedas(int quantidade);  // renamed from moedas
    void tripul(int id, int quantidade);
    void saves(const std::string &nome);
    void loads(const std::string &nome);
    void lists();
    void dels(const std::string &nome);
    void terminar();
    int getMoedas() const { return moedas; }  // Add this getter
};
