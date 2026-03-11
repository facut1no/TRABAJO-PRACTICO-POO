#include "dado.h"

int Dado::tirar() {
    this->valor = QRandomGenerator::global()->bounded(1, 7);
    return valor;
}

int Dado::getValor() { return valor; }

