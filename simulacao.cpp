#include <iostream>
#include <fstream>
#include <sstream>
#include "simulacao.h"

Simulacao::Simulacao() : mapa(10, 20), moedas(1000), instantesEntreNovosItens(10),
                         duracaoItem(20), maxItens(5), precoVendaMercadoria(2),
                         precoCompraMercadoria(1), precoCaravana(100),
                         instantesEntreNovosBarbaros(40), duracaoBarbaros(60) {}

void Simulacao::lerConfig(const std::string &nomeFicheiro) {
    std::ifstream file(nomeFicheiro);
    if (!file) {
        std::cerr << "Erro ao abrir o ficheiro de configuração." << std::endl;
        exit(1);
    }

    std::string line;
    int linha = 0, coluna = 0;

    std::getline(file, line); // "linhas 10"
    linha = std::stoi(line.substr(line.find(' ') + 1));

    std::getline(file, line); // "colunas 20"
    coluna = std::stoi(line.substr(line.find(' ') + 1));

    Mapa novoMapa(linha, coluna);

    for (int i = 0; i < linha; ++i) {
        std::getline(file, line);
        for (int j = 0; j < coluna; ++j) {
            novoMapa.adicionarElemento(i, j, line[j]);
        }
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string nome;
        int valor;
        ss >> nome >> valor;

        if (nome == "moedas") {
            moedas = valor;
        } else if (nome == "instantes_entre_novos_itens") {
            instantesEntreNovosItens = valor;
        } else if (nome == "duração_item") {
            duracaoItem = valor;
        } else if (nome == "max_itens") {
            maxItens = valor;
        } else if (nome == "preço_venda_mercadoria") {
            precoVendaMercadoria = valor;
        } else if (nome == "preço_compra_mercadoria") {
            precoCompraMercadoria = valor;
        } else if (nome == "preço_caravana") {
            precoCaravana = valor;
        } else if (nome == "instantes_entre_novos_barbaros") {
            instantesEntreNovosBarbaros = valor;
        } else if (nome == "duração_barbaros") {
            duracaoBarbaros = valor;
        }
    }

    mapa = novoMapa;
}

void Simulacao::exibirMapa() {
    mapa.exibirMapa();
}

void Simulacao::iniciarSimulacao() {
    std::cout << "Simulação iniciada com as seguintes configurações:" << std::endl;
    std::cout << "Moedas iniciais: " << moedas << std::endl;
    std::cout << "Mapa do deserto:" << std::endl;
    exibirMapa();
}
