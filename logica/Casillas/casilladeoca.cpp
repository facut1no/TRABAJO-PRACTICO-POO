#include "casilladeoca.h"

CasillaDeOca::CasillaDeOca(int num) : Casilla(num, TipoCasilla::Oca) {
    m_Mensaje = QString("¡De oca a oca y tiro porque me toca! Avanzas a la siguiente oca y tiras de nuevo.");
}

void CasillaDeOca::setSiguienteOca(int siguiente) {
    this->siguienteOca = siguiente;
}

int CasillaDeOca::getSiguienteOca() const { return this->siguienteOca; }

void CasillaDeOca::aplicarEfecto(Jugador& jugador) {
    if(this->siguienteOca != -1){
        jugador.setPosicion(this->siguienteOca);
    } else {
        jugador.setPosicionAnterior();
    }
}
