#include "casillalaberinto.h"

CasillaLaberinto::CasillaLaberinto(int num) : Casilla(num, TipoCasilla::Laberinto) {
    m_Mensaje = QString("¡Oh no! Te pierdes en el laberinto. Retrocede a la casilla número 30.");
}

void CasillaLaberinto::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicion(29);
}
