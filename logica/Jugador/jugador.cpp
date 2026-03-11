#include "jugador.h"
#include <qdebug.h>

Jugador::Jugador(QString nombre, QColor color) : m_Nombre(nombre), m_Color(color) {
    m_Posicion = -1;
    m_EfectoAplicado = false;
    m_TurnosPerdidos = 0;
    m_EnPozo = false;
    m_EsGanador = false;
    m_PosicionAnterior = m_Posicion;
}

QString Jugador::getNombre() const {
    return m_Nombre;
}

void Jugador::reiniciar() {
    this->m_TurnosPerdidos = 0;
    this->m_Posicion = -1;
    this->m_EsGanador = false;
    this->m_EnPozo = false;
    this->m_EfectoAplicado = false;
    this->m_PosicionAnterior = this->m_Posicion;
}

void Jugador::mover(int pasos, int cantCasillas) {
    this->m_PosicionAnterior = this->m_Posicion;

    int nuevaPosicion = m_Posicion + pasos;
    if (nuevaPosicion < 0) nuevaPosicion = 0;

    if (nuevaPosicion > cantCasillas - 1) {
        int exceso = nuevaPosicion - (cantCasillas - 1);
        nuevaPosicion = (cantCasillas - 1) - exceso;
    }

    this->m_Posicion = nuevaPosicion;
}

int Jugador::getPosicionAnterior() const {
    return m_PosicionAnterior;
}

int Jugador::getPosicion() const {
    return m_Posicion;
}

QColor Jugador::getColor() const {
    return m_Color;
}

bool Jugador::getEnPozo() const {
    return m_EnPozo;
}

bool Jugador::esGanador() const {
    return m_EsGanador;
}

int Jugador::getTurnosPerdidos() const {
    return m_TurnosPerdidos;
}

void Jugador::restarTurno() {
    m_TurnosPerdidos--;
}

bool Jugador::puedeJugar() const {
    return !m_EnPozo && m_TurnosPerdidos == 0;
}

void Jugador::setPosicion(int nuevaPosicion) {
    m_PosicionAnterior = m_Posicion;
    m_Posicion = nuevaPosicion;
}

void Jugador::setPosicionAnterior() {
    m_PosicionAnterior = m_Posicion;
}

void Jugador::setTurnosPerdidos(int turnos) {
    m_TurnosPerdidos = turnos;
}

void Jugador::setEnPozo(bool estado) {
    m_EnPozo = estado;
}

void Jugador::setEsGanador(bool estado) {
    m_EsGanador = estado;
}

void Jugador::setEfectoAplicado(bool estado) {
    m_EfectoAplicado = estado;
}

bool Jugador::getEfectoAplicado() const {
    return m_EfectoAplicado;
}
