#include "casillajardindeoca.h"

CasillaJardinDeOca::CasillaJardinDeOca(int num) : Casilla(num, TipoCasilla::JardinDeOca) {
    m_Mensaje = QString("Has caido en el jardon de oca. Ganaste felicidades!");
}

void CasillaJardinDeOca::aplicarEfecto(Jugador& jugador) {
    jugador.setPosicionAnterior();
    jugador.setEsGanador(true);
}
