#include "buffer.h"
#include <cstring>
#include <string>
#include <sstream>

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
    // Clear screen once
    std::cout << "\033[2J\033[H";
    
    // Draw single border
    std::cout << "╔";
    for (int j = 0; j < colunas; ++j) std::cout << "═";
    std::cout << "╗\n";

    // Draw map content
    for (int i = 0; i < linhas; ++i) {
        std::cout << "║";
        for (int j = 0; j < colunas; ++j) {
            char c = buffer[i][j];
            
            if (c >= 'a' && c <= 'z') std::cout << "\033[32m" << c << "\033[0m";
            else if (c >= '0' && c <= '9') std::cout << "\033[34m" << c << "\033[0m";
            else if (c == '!') std::cout << "\033[31m" << c << "\033[0m";
            else if (c == '+') std::cout << "\033[33m" << c << "\033[0m";
            else if (c == 'I') std::cout << "\033[35m" << c << "\033[0m";
            else if (c == '*') std::cout << "\033[33;1m" << c << "\033[0m";  // Bright yellow for sandstorm
            else std::cout << ".";
        }
        std::cout << "║\n";
    }

    // Draw bottom border
    std::cout << "╚";
    for (int j = 0; j < colunas; ++j) std::cout << "═";
    std::cout << "╝" << std::endl;
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