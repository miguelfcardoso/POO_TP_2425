#ifndef SIMULACAO_H
#define SIMULACAO_H

#include <string>
#include "mapa.h"
#include "caravana.h"
#include "barbaro.h"

class Simulacao {
private:
    Mapa mapa;
    int moedas;
    int instantesEntreNovosItens;
    int duracaoItem;
    int maxItens;
    int precoVendaMercadoria;
    int precoCompraMercadoria;
    int precoCaravana;
    int instantesEntreNovosBarbaros;
    int duracaoBarbaros;

public:
    Simulacao();
    void lerConfig(const std::string &nomeFicheiro);
    void exibirMapa();
    void iniciarSimulacao();
    void executarComandos();
};

#endif
