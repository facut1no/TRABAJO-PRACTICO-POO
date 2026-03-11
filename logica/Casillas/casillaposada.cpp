#include "casillaposada.h"

CasillaPosada::CasillaPosada(int num) : Casilla(num, TipoCasilla::Posada) {
    m_Mensaje = QString("Te detienes en la posada. Pierdes un turno descansando.");
}

void CasillaPosada::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicionAnterior();
    jugador.setTurnosPerdidos(1);
}
