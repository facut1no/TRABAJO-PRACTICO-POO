#include "casillanormal.h"

CasillaNormal::CasillaNormal(int num) : Casilla(num, TipoCasilla::Normal) {
    m_Mensaje = QString("Te encuentras en una casilla tranquila. Espera tu próximo turno.");
}

void CasillaNormal::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicionAnterior();
}
