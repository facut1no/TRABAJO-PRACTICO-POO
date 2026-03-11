#include "casillacarcel.h"

CasillaCarcel::CasillaCarcel(int num) : Casilla(num, TipoCasilla::Carcel) {
    m_Mensaje = QString("Has caido en la casilla carcel. Pierdes dos turnos.");
}

void CasillaCarcel::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicionAnterior();
    jugador.setTurnosPerdidos(2);
}
