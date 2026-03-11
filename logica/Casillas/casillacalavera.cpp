#include "casillacalavera.h"

CasillaCalavera::CasillaCalavera(int num) : Casilla(num, TipoCasilla::Calavera) {
    m_Mensaje = QString("Has caído en la casilla de la Calavera. Retrocedes hasta el inicio y pierdes tu progreso.");
}

void CasillaCalavera::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicion(0);
}
