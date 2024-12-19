#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <string>

class Buffer {
    int linhas, colunas;
    int cursorX, cursorY;
    char** buffer;

public:
    Buffer(int l, int c);
    Buffer(const Buffer& other);
    ~Buffer();
    Buffer& operator=(const Buffer& other);

    void esvaziar();
    void transcreverParaConsola() const;
    void moverCursor(int x, int y);
    
    void imprimir(char c);
    void imprimir(const char* str);
    void imprimir(const std::string& str);
    void imprimir(int num);

    friend Buffer& operator<<(Buffer& buf, char c);
    friend Buffer& operator<<(Buffer& buf, const std::string& str);
    friend Buffer& operator<<(Buffer& buf, int num);
};

Buffer& operator<<(Buffer& buf, char c);
Buffer& operator<<(Buffer& buf, const std::string& str);
Buffer& operator<<(Buffer& buf, int num);

#endif