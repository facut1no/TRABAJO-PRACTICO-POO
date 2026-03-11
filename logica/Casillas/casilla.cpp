#include "casilla.h"

Casilla::Casilla(int num, TipoCasilla tipo) : m_Numero(num), m_Tipo(tipo) {
    m_Mensaje = "";
}

int Casilla::obtenerNumero() const {
    return m_Numero;
}

QString Casilla::getMensaje() const {
    return m_Mensaje;
}

TipoCasilla Casilla::obtenerTipo() const {
    return m_Tipo;
}
