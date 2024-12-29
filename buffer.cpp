#include "buffer.h"
#include <cstring>
#include <string>
#include <sstream>
#include <locale.h>

Buffer::Buffer(int l, int c) : linhas(l), colunas(c), cursorX(0), cursorY(0) {
    buffer = new char*[linhas];
    for (int i = 0; i < linhas; ++i) {
        buffer[i] = new char[colunas];
        for (int j = 0; j < colunas; ++j) {
            buffer[i][j] = ' ';
        }
    }
}

Buffer::~Buffer() {
    if (buffer) {
        for (int i = 0; i < linhas; ++i) {
            delete[] buffer[i];
        }
        delete[] buffer;
    }
}

Buffer::Buffer(const Buffer& other) : linhas(other.linhas), colunas(other.colunas),
                                    cursorX(other.cursorX), cursorY(other.cursorY) {
    buffer = new char*[linhas];
    for (int i = 0; i < linhas; ++i) {
        buffer[i] = new char[colunas];
        memcpy(buffer[i], other.buffer[i], colunas);
    }
}

Buffer& Buffer::operator=(const Buffer& other) {
    if (this != &other) {
        // Delete old data
        for (int i = 0; i < linhas; ++i) {
            delete[] buffer[i];
        }
        delete[] buffer;
        
        // Copy new data
        linhas = other.linhas;
        colunas = other.colunas;
        cursorX = other.cursorX;
        cursorY = other.cursorY;
        
        buffer = new char*[linhas];
        for (int i = 0; i < linhas; ++i) {
            buffer[i] = new char[colunas];
            memcpy(buffer[i], other.buffer[i], colunas);
        }
    }
    return *this;
}

void Buffer::esvaziar() {
    cursorX = 0;
    cursorY = 0;
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            buffer[i][j] = ' ';
        }
    }
}

void Buffer::transcreverParaConsola() const {
    std::cout << "\033[2J\033[H";  // Clear screen
    
    // Top border with simple ASCII
    std::cout << "+";
    for (int j = 0; j < colunas; ++j) 
        std::cout << "-";
    std::cout << "+\n";
    
    // Map content with simple ASCII
    for (int i = 0; i < linhas; ++i) {
        std::cout << "|";  // Left border
        for (int j = 0; j < colunas; ++j) {
            char c = buffer[i][j];
            
            // Use simpler coloring scheme that works everywhere
            if (c >= 'a' && c <= 'z') std::cout << c;  // Cities
            else if (c >= '0' && c <= '9') std::cout << c;  // Caravans
            else if (c == '!') std::cout << c;  // Barbarians
            else if (c == '+') std::cout << c;  // Obstacles
            else if (c == 'I') std::cout << c;  // Items
            else if (c == '*') std::cout << c;  // Sandstorm
            else std::cout << ".";  // Empty space
        }
        std::cout << "|\n";  // Right border
    }

    // Bottom border with simple ASCII
    std::cout << "+";
    for (int j = 0; j < colunas; ++j) 
        std::cout << "-";
    std::cout << "+\n";
}

void Buffer::moverCursor(int x, int y) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas) {
        cursorX = x;
        cursorY = y;
    }
}

void Buffer::imprimir(char c) {
    if (c == '\n') {
        cursorX++;
        cursorY = 0;
    } else if (cursorX < linhas && cursorY < colunas) {
        buffer[cursorX][cursorY] = c;
        cursorY++;
    }
}

void Buffer::imprimir(const char* str) {
    while (*str) {
        imprimir(*str++);
    }
}

void Buffer::imprimir(const std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        imprimir(c);
    }
}

void Buffer::imprimir(int num) {
    std::ostringstream oss;
    oss << num;
    imprimir(oss.str().c_str());
}

Buffer& operator<<(Buffer& buf, char c) {
    buf.imprimir(c);
    return buf;
}

Buffer& operator<<(Buffer& buf, const std::string& str) {
    buf.imprimir(str);
    return buf;
}

Buffer& operator<<(Buffer& buf, int num) {
    buf.imprimir(num);
    return buf;
}