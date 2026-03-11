#include "casillapozo.h"

CasillaPozo::CasillaPozo(int num) : Casilla(num, TipoCasilla::Pozo) {
    m_Mensaje = QString("Has caído en el pozo. Debes esperar aquí hasta que otro jugador caiga y te rescate.");
}

void CasillaPozo::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicionAnterior();
    jugador.setEnPozo(true);
}
