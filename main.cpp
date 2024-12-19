#include <iostream>
#include <string>
#include <algorithm>
#include "simulacao.h"
#include "buffer.h"

int main() {
    Simulacao simulacao;
    bool simulacaoIniciada = false;

    while (true) {
        std::cout << "Digite um comando: ";
        std::string comando;
        std::getline(std::cin, comando);

        if (comando.rfind("config ", 0) == 0) {
            std::string nomeFicheiro = comando.substr(7);
            // Remove any < > characters if present
            nomeFicheiro.erase(
                std::remove(nomeFicheiro.begin(), nomeFicheiro.end(), '<'),
                nomeFicheiro.end()
            );
            nomeFicheiro.erase(
                std::remove(nomeFicheiro.begin(), nomeFicheiro.end(), '>'),
                nomeFicheiro.end()
            );
            // Trim whitespace
            nomeFicheiro.erase(0, nomeFicheiro.find_first_not_of(" \t"));
            nomeFicheiro.erase(nomeFicheiro.find_last_not_of(" \t") + 1);
            
            std::cout << "Full command: '" << comando << "'" << std::endl;
            std::cout << "Extracted filename: '" << nomeFicheiro << "'" << std::endl;
            std::cout << "Tentando abrir arquivo: " << nomeFicheiro << std::endl;
            simulacao.lerConfig(nomeFicheiro);
            simulacaoIniciada = true;
            simulacao.iniciarSimulacao();
        } else if (comando == "sair") {
            if (simulacaoIniciada) {
                simulacao.terminarSimulacao();
            }
            break;
        } else if (simulacaoIniciada) {
            // Forward all other commands to the simulation when it's running
            simulacao.executarComandos(comando);
        } else {
            std::cout << "Por favor, inicie a simulação com o comando 'config <arquivo>' primeiro." << std::endl;
        }
    }

    return 0;
}
