#include <iostream>
#include "simulacao.h"

int main() {
    Simulacao simulacao;

    // Exemplo de leitura do ficheiro de configuração
    simulacao.lerConfig("config.txt");
    simulacao.iniciarSimulacao();

    return 0;
}
