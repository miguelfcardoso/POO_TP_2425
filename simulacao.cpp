#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>  // Add this for INT_MAX
#include "simulacao.h"

Simulacao::Simulacao() : mapa(10, 20), buffer(new Buffer(20, 40)), moedas(1000), instantesEntreNovosItens(10),
                         duracaoItem(20), maxItens(5), precoVendaMercadoria(2),
                         precoCompraMercadoria(1), precoCaravana(100),
                         instantesEntreNovosBarbaros(40), duracaoBarbaros(60),
                         combatesVencidos(0), instantesDecorridos(0) {}

Simulacao::~Simulacao() {
    delete buffer;
    for (auto& [name, buf] : bufferSaves) {
        delete buf;
    }
}

void Simulacao::lerConfig(const std::string &nomeFicheiro) {
    // Try different possible file locations with absolute path
    std::vector<std::string> possiblePaths = {
        nomeFicheiro,
        "./text",  // Try without extension
        "./text.txt",
        "../text",
        "../text.txt",
        "/home/miguel/Desktop/TP/POO/POO_TP_2425/text",
        "/home/miguel/Desktop/TP/POO/POO_TP_2425/text.txt"
    };

    std::ifstream file;
    std::string pathUsed;
    
    for (const auto& path : possiblePaths) {
        std::cout << "Trying path: " << path << std::endl;
        file.open(path);
        if (file.is_open()) {
            pathUsed = path;
            std::cout << "Successfully opened file: " << path << std::endl;
            break;
        }
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file. Attempted paths:" << std::endl;
        for (const auto& path : possiblePaths) {
            std::cerr << "- " << path << std::endl;
        }
        std::cerr << "Current directory contents:" << std::endl;
        system("ls -la");
        return;
    }

    std::cout << "Arquivo aberto com sucesso: " << pathUsed << std::endl;
    
    std::string line;
    int linha = 0, coluna = 0;

    std::getline(file, line); // "linhas 10"
    linha = std::stoi(line.substr(line.find(' ') + 1));

    std::getline(file, line); // "colunas 20"
    coluna = std::stoi(line.substr(line.find(' ') + 1));

    Mapa novoMapa(linha, coluna);

    // Read map data
    for (int i = 0; i < linha; ++i) {
        std::getline(file, line);
        if (line.length() >= coluna) {
            for (int j = 0; j < coluna; ++j) {
                char elemento = line[j];
                if (elemento >= 'a' && elemento <= 'z') {
                    // Only call adicionarCidade for cities
                    novoMapa.adicionarCidade(elemento, i, j);
                } else {
                    // Call adicionarElemento for non-city elements
                    novoMapa.adicionarElemento(i, j, elemento);
                }
            }
        } else {
            std::cerr << "Warning: Line " << i << " is too short (length: " << line.length() 
                     << ", expected: " << coluna << ")" << std::endl;
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
    buffer->esvaziar();
    mapa.exibirParaBuffer(*buffer);
    buffer->transcreverParaConsola();
    std::cout << "Moedas: " << moedas << std::endl;
}

void Simulacao::iniciarSimulacao() {
    std::cout << "Simulação iniciada com as seguintes configurações:" << std::endl;
    std::cout << "Moedas iniciais: " << moedas << std::endl;
    std::cout << "Mapa do deserto:" << std::endl;
    exibirMapa();
}

void Simulacao::adicionarCaravana(int id, int x, int y) {
    mapa.adicionarCaravana(id, x, y);
}

void Simulacao::adicionarCaravana(int id, int x, int y, Caravana::Tipo tipo) {
    mapa.adicionarCaravana(id, x, y, tipo);
}

void Simulacao::adicionarItem(Item::Tipo tipo, int x, int y, int duracao) {
    mapa.adicionarItem(tipo, x, y, duracao);
}

void Simulacao::aplicarEfeitoItem(Caravana& caravana, Item::Tipo tipo) {
    switch (tipo) {
        case Item::CAIXA_PANDORA:
            caravana.adicionarTripulantes(-caravana.getTripulantes() * 0.2);
            break;
        case Item::ARCA_TESOURO:
            moedas += moedas * 0.1;
            break;
        case Item::JAULA:
            caravana.adicionarTripulantes(5); // Exemplo de adicionar 5 tripulantes
            break;
        case Item::MINA:
            // Lógica para destruir a caravana
            break;
        case Item::SURPRESA:
            // Lógica para o item surpresa
            break;
    }
}

void Simulacao::realizarCombate(Caravana& caravana, Barbaro& barbaro) {
    int forcaCaravana = rand() % (caravana.getTripulantes() + 1);
    int forcaBarbaro = rand() % (barbaro.getTripulantes() + 1);

    if (forcaCaravana > forcaBarbaro) {
        int perdaCaravana = caravana.getTripulantes() * 0.2;
        int perdaBarbaro = perdaCaravana * 2;
        caravana.adicionarTripulantes(-perdaCaravana);
        barbaro.adicionarTripulantes(-perdaBarbaro);
        if (barbaro.getTripulantes() <= 0) {
            caravana.adicionarAgua(barbaro.getAgua());
            barbaro.destruir();
        }
    } else {
        int perdaBarbaro = barbaro.getTripulantes() * 0.2;
        int perdaCaravana = perdaBarbaro * 2;
        barbaro.adicionarTripulantes(-perdaBarbaro);
        caravana.adicionarTripulantes(-perdaCaravana);
        if (caravana.getTripulantes() <= 0) {
            barbaro.adicionarAgua(caravana.getAgua());
            caravana.destruir();
        }
    }
}

void Simulacao::passarInstantes(int instantes) {
    for (int i = 0; i < instantes; ++i) {
        instantesDecorridos++;
        
        // Move caravanas
        for (auto& [id, caravana] : mapa.getCaravanas()) {
            if (!caravana.isDestruida()) {
                if (!caravana.getAutogestao()) {
                    // More diverse movement options
                    char directions[] = {'C', 'B', 'E', 'D', 'Q', 'W', 'A', 'S'};
                    char dir = directions[rand() % 8];
                    std::cout << "Moving caravana " << id << " in direction " << dir << std::endl;
                    mapa.moverCaravana(id, dir);
                }
                
                // Update water consumption with feedback
                int prevWater = caravana.getAgua();
                caravana.consumirAgua();
                std::cout << "Caravana " << id << " water: " << prevWater << " -> " << caravana.getAgua() << std::endl;
                
                if (mapa.isCidade(caravana.getX(), caravana.getY())) {
                    caravana.reabastecerAgua();
                    std::cout << "Caravana " << id << " refilled water at city" << std::endl;
                }
            }
        }

        // Move barbaros
        for (auto& barbaro : mapa.getBarbaros()) {
            if (!barbaro.isDestruido()) {
                // Find closest caravana
                int minDist = INT_MAX;
                std::pair<int, int> target = {-1, -1};
                for (const auto& [id, caravana] : mapa.getCaravanas()) {
                    int dist = abs(barbaro.getX() - caravana.getX()) + 
                             abs(barbaro.getY() - caravana.getY());
                    if (dist < minDist) {
                        minDist = dist;
                        target = {caravana.getX(), caravana.getY()};
                    }
                }
                if (target.first != -1) {
                    // Cast away const since we know this is our non-const barbaro
                    const_cast<Barbaro&>(barbaro).moverAutonomo(target.first, target.second);
                }
            }
        }

        // Item management with debug output
        if (instantesDecorridos % instantesEntreNovosItens == 0) {
            std::cout << "Checking for item spawning..." << std::endl;
            int currentItems = mapa.contarItens();
            std::cout << "Current items: " << currentItems << "/" << maxItens << std::endl;
            
            if (currentItems < maxItens) {
                for (int j = 0; j < maxItens - currentItems; ++j) {
                    int x = rand() % mapa.getLinhas();
                    int y = rand() % mapa.getColunas();
                    if (mapa.isPosicaoVazia(x, y)) {
                        Item::Tipo tipo = static_cast<Item::Tipo>(rand() % 5);
                        mapa.adicionarItem(tipo, x, y, duracaoItem);
                        std::cout << "Added new item at (" << x << "," << y << ")" << std::endl;
                    }
                }
            }
        }

        // Atualizar itens existentes e verificar interações
        mapa.atualizarItens();
        mapa.verificarInteracoesItens(*this);

        // Verificar interações com itens
        for (auto& caravana : mapa.getCaravanas()) {
            if (Item* item = mapa.getItemAdjacente(caravana.second.getX(), caravana.second.getY())) {
                // Se encontrou item adjacente, aplicar efeito
                if (item->getTipo() == Item::ARCA_TESOURO) {
                    moedas += moedas * 0.1; // 10% de aumento
                }
                item->aplicarEfeito(caravana.second);
                mapa.removerItem(item->getX(), item->getY());
            }
        }
        
        // Verificar combates
        for (auto& [id, caravana] : mapa.getCaravanas()) {
            for (auto& barbaro : mapa.getBarbaros()) {
                if (verificarCombate(caravana.getX(), caravana.getY(), 
                    barbaro.getX(), barbaro.getY())) {
                    Barbaro& barbaroRef = const_cast<Barbaro&>(barbaro);
                    realizarCombate(caravana, barbaroRef);
                }
            }

            // Verificar itens adjacentes
            Item* item = mapa.getItemAdjacente(caravana.getX(), caravana.getY());
            if (item) {
                aplicarEfeitoItem(caravana, item->getTipo());
                mapa.removerItem(item->getX(), item->getY());
            }

            // Movimento autônomo
            caravana.moverAutonomo();

            // Consumo de água
            caravana.consumirAgua();

            // Movimento sem tripulantes
            if (caravana.getTripulantes() == 0) {
                caravana.moverSemTripulantes();
            }
        }

        // Water consumption
        for (auto& [id, caravana] : mapa.getCaravanas()) {
            caravana.consumirAgua();
            
            // Automatic water refill in cities
            if (mapa.isCidade(caravana.getX(), caravana.getY())) {
                caravana.reabastecerAgua();
            }
        }
    }
    exibirMapa(); // Show updated state after all instants
}

// Update method signature to match header
bool Simulacao::verificarCombate(int x1, int y1, int x2, int y2) {
    // Check if positions are adjacent
    return (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1);
}

void Simulacao::terminarSimulacao() {
    bool semCaravanas = mapa.getCaravanas().empty();
    bool semDinheiro = moedas < precoCaravana;
    
    if (semCaravanas && semDinheiro) {
        std::cout << "Game Over! Sem caravanas e sem dinheiro para comprar novas." << std::endl;
    }
    
    std::cout << "Simulação terminada." << std::endl;
    std::cout << "Instantes decorridos: " << instantesDecorridos << std::endl;
    std::cout << "Combates vencidos: " << combatesVencidos << std::endl;
    std::cout << "Moedas restantes: " << moedas << std::endl;
}

void Simulacao::tempestadeDeAreia(int centroX, int centroY, int raio) {
    mapa.tempestadeDeAreia(centroX, centroY, raio);
}

void Simulacao::listarCaravanasCidade(char nome) {
    Cidade* cidade = mapa.getCidade(nome);
    if (cidade) {
        cidade->listarCaravanas();
    } else {
        std::cout << "Cidade não encontrada." << std::endl;
    }
}

void Simulacao::comprarTripulantes(int id, int quantidade) {
    if (moedas >= quantidade) {
        moedas -= quantidade;
        mapa.getCaravanas().at(id).adicionarTripulantes(quantidade);
    } else {
        std::cout << "Moedas insuficientes." << std::endl;
    }
}

void Simulacao::venderMercadoria(int id) {
    mapa.getCaravanas().at(id).venderMercadoria(precoVendaMercadoria);
}

void Simulacao::comprarMercadoria(int id, int quantidade) {
    int custo = quantidade * precoCompraMercadoria;
    if (moedas >= custo) {
        moedas -= custo;
        mapa.getCaravanas().at(id).comprarMercadoria(quantidade, precoCompraMercadoria);
    } else {
        std::cout << "Moedas insuficientes." << std::endl;
    }
}

void Simulacao::comprarCaravana(char cidade, Caravana::Tipo tipo) {
    if (moedas >= precoCaravana) {
        Cidade* c = mapa.getCidade(cidade);
        if (c != nullptr) {
            int novoId = mapa.getNextAvailableId();
            if (novoId == -1) {
                std::cout << "Limite máximo de caravanas atingido (10)." << std::endl;
                return;
            }
            
            std::cout << "Creating new caravana with ID " << novoId << " at city " << cidade << std::endl;
            adicionarCaravana(novoId, c->getX(), c->getY(), tipo);
            moedas -= precoCaravana;
            std::cout << "Caravana criada com ID " << novoId << std::endl;
        } else {
            std::cout << "Cidade '" << cidade << "' não encontrada." << std::endl;
        }
    } else {
        std::cout << "Moedas insuficientes." << std::endl;
    }
}

void Simulacao::executarComandos(const std::string& comando) {
    std::istringstream ss(comando);
    std::string cmd;
    ss >> cmd;

    if (cmd == "config") {
        std::string filename;
        ss >> filename;
        lerConfig(filename);
    } else if (cmd == "exec") {
        std::string filename;
        ss >> filename;
        exec(filename);
    } else if (cmd == "prox") {
        int n = 1;  // default value
        ss >> n;
        prox(n);
    } else if (cmd == "comprac") {
        char cidade;
        char tipo;
        ss >> cidade >> tipo;
        comprac(cidade, tipo);
    } else if (cmd == "precos") {
        precos();
    } else if (cmd == "cidade") {
        char nome;
        ss >> nome;
        cidade(nome);
    } else if (cmd == "caravana") {
        int id;
        if (ss >> id) {
            try {
                caravana(id);
            } catch (const std::out_of_range& e) {
                std::cout << "Caravana " << id << " não encontrada." << std::endl;
            }
        } else {
            std::cout << "Uso correto: caravana <id>" << std::endl;
        }
    } else if (cmd == "compra") {
        int id, quantidade;
        ss >> id >> quantidade;
        compra(id, quantidade);
    } else if (cmd == "vende") {
        int id;
        ss >> id;
        vende(id);
    } else if (cmd == "move") {
        int id;
        char direcao;
        ss >> id >> direcao;
        move(id, direcao);
    } else if (cmd == "auto") {
        int id;
        ss >> id;
        autoGestao(id);
    } else if (cmd == "stop") {
        int id;
        ss >> id;
        stop(id);
    } else if (cmd == "barbaro") {
        int x, y;
        ss >> x >> y;
        barbaro(x, y);
    } else if (cmd == "areia") {
        int x, y, raio;
        ss >> x >> y >> raio;
        areia(x, y, raio);
    } else if (cmd == "moedas") {
        int quantidade;
        ss >> quantidade;
        adicionarMoedas(quantidade);
    } else if (cmd == "tripul") {
        int id, quantidade;
        ss >> id >> quantidade;
        tripul(id, quantidade);
    } else if (cmd == "saves") {
        std::string nome;
        ss >> nome;
        saves(nome);
    } else if (cmd == "loads") {
        std::string nome;
        ss >> nome;
        loads(nome);
    } else if (cmd == "lists") {
        lists();
    } else if (cmd == "dels") {
        std::string nome;
        ss >> nome;
        dels(nome);
    } else if (cmd == "terminar") {
        terminar();
    } else {
        std::cout << "Comando desconhecido: " << cmd << std::endl;
    }
}

void Simulacao::exec(const std::string &nomeFicheiro) {
    std::ifstream file(nomeFicheiro);
    if (!file) {
        std::cerr << "Erro ao abrir o ficheiro de comandos." << std::endl;
        return;
    }

    std::string comando;
    while (std::getline(file, comando)) {
        executarComandos(comando);
    }
}

void Simulacao::prox(int n) {
    for (int i = 0; i < n; ++i) {
        passarInstantes(1);
        exibirMapa();
    }
}

void Simulacao::comprac(char cidade, char tipo) {
    std::cout << "Attempting to buy caravana at city " << cidade << " of type " << tipo << std::endl;
    
    if (moedas < precoCaravana) {
        std::cout << "Moedas insuficientes. Necessário: " << precoCaravana << ", Disponível: " << moedas << std::endl;
        return;
    }

    Cidade* c = mapa.getCidade(cidade);
    if (!c) {
        std::cout << "Cidade '" << cidade << "' não encontrada no mapa." << std::endl;
        return;
    }

    int novoId = mapa.getNextAvailableId();  // Changed from c->getProximaCaravanaId()
    if (novoId == -1) {
        std::cout << "Limite máximo de caravanas atingido (10)." << std::endl;
        return;
    }

    Caravana::Tipo tipoCaravana;
    switch (tipo) {
        case 'C': tipoCaravana = Caravana::COMERCIO; break;
        case 'M': tipoCaravana = Caravana::MILITAR; break;
        case 'S': tipoCaravana = Caravana::SECRETA; break;
        default:
            std::cout << "Tipo de caravana inválido: " << tipo << std::endl;
            return;
    }

    mapa.adicionarCaravana(novoId, c->getX(), c->getY(), tipoCaravana);
    moedas -= precoCaravana;
    std::cout << "Caravana " << novoId << " criada com sucesso em (" << c->getX() << "," << c->getY() << ")" << std::endl;
}

void Simulacao::compra(int id, int quantidade) {
    try {
        if (quantidade <= 0) {
            std::cout << "Quantidade inválida." << std::endl;
            return;
        }
        
        auto& caravana = mapa.getCaravanas().at(id);
        int custo = quantidade * precoCompraMercadoria;
        
        if (moedas < custo) {
            std::cout << "Moedas insuficientes." << std::endl;
            return;
        }
        
        if (caravana.getMercadoria() + quantidade > caravana.getCapacidadeCarga()) {
            std::cout << "Capacidade de carga insuficiente." << std::endl;
            return;
        }
        
        moedas -= custo;
        caravana.comprarMercadoria(quantidade, precoCompraMercadoria);
        std::cout << "Compra realizada com sucesso." << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "Caravana " << id << " não encontrada." << std::endl;
    }
}

void Simulacao::precos() {
    std::cout << "Preço de compra de mercadoria: " << precoCompraMercadoria << " moedas por tonelada" << std::endl;
    std::cout << "Preço de venda de mercadoria: " << precoVendaMercadoria << " moedas por tonelada" << std::endl;
}

void Simulacao::cidade(char nome) {
    listarCaravanasCidade(nome);
}

void Simulacao::caravana(int id) {
    mapa.getCaravanas().at(id).mostrarDetalhes();
}

void Simulacao::vende(int id) {
    venderMercadoria(id);
}

void Simulacao::move(int id, char direcao) {
    mapa.moverCaravana(id, direcao);
}

void Simulacao::autoGestao(int id) {
    // Implementar l��gica para colocar a caravana em auto-gestão
}

void Simulacao::stop(int id) {
    // Implementar lógica para parar a auto-gestão da caravana
}

void Simulacao::barbaro(int x, int y) {
    mapa.adicionarBarbaro(x, y);
}

void Simulacao::areia(int x, int y, int raio) {
    tempestadeDeAreia(x, y, raio);
}

void Simulacao::adicionarMoedas(int quantidade) {  // renamed from moedas
    moedas += quantidade;
}

void Simulacao::tripul(int id, int quantidade) {
    comprarTripulantes(id, quantidade);
}

void Simulacao::saves(const std::string& nome) {
    Buffer* novoBuf = new Buffer(*buffer);
    delete bufferSaves[nome];  // Delete old buffer if exists
    bufferSaves[nome] = novoBuf;
    std::cout << "Estado visual salvo como '" << nome << "'" << std::endl;
}

void Simulacao::loads(const std::string& nome) {
    auto it = bufferSaves.find(nome);
    if (it != bufferSaves.end()) {
        it->second->transcreverParaConsola();
    } else {
        std::cout << "Estado visual '" << nome << "' não encontrado." << std::endl;
    }
}

void Simulacao::lists() {
    std::cout << "Estados visuais salvos:" << std::endl;
    for (const auto& [nome, _] : bufferSaves) {
        std::cout << "- " << nome << std::endl;
    }
}

void Simulacao::dels(const std::string& nome) {
    if (bufferSaves.erase(nome) > 0) {
        std::cout << "Estado visual '" << nome << "' apagado." << std::endl;
    } else {
        std::cout << "Estado visual '" << nome << "' não encontrado." << std::endl;
    }
}

void Simulacao::terminar() {
    terminarSimulacao();
    // Implementar lógica para reiniciar a simulação ou sair do programa
}
