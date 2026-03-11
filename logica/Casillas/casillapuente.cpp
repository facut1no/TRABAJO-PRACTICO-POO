#include "casillapuente.h"

CasillaPuente::CasillaPuente(int num) : Casilla(num, TipoCasilla::Puente) {
    m_Mensaje = QString("Caiste en un punte. Saltas al otro puente.");
}

void CasillaPuente::setPosOtroPuente(int pos) { this->posOtroPuente = pos; }

void CasillaPuente::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicion(this->posOtroPuente);
}
