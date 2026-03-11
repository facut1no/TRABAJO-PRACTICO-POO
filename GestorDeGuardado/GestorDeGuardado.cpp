#include "GestorDeGuardado.h"
#include <QDir>
#include <fstream>

#include <fstream>
#include <string>
#include <QDebug>
#include <cstring>
#include "../logica/Casillas/casilladeoca.h"
#include "../logica/Casillas/casillapuente.h"


bool GestorDeGuardado::guardarPartida() {
    std::ofstream archivo(this->direccionArchivo.toStdString(), std::ios::binary);
    if(!archivo.is_open()) {
        qCritical() << "Error: No se pudo abrir el archivo para guardar:" << this->direccionArchivo;
        return false;
    }

    auto jugadores = this->m_juego->obtenerJugadores();
    auto casillas = this->m_juego->obtenerTablero()->obtenerCasillas();

    PartidaStruct partida;
    partida.cantidadDados = this->m_juego->obtenerCantidadDados();
    partida.cantidadJugadores = jugadores.size();
    partida.sizeTablero = casillas.size();
    partida.turnoActual = this->m_juego->obtenerTurnoActual();
    partida.partidaGuardada = this->m_juego->partidaGuardada;

    if (!archivo.write(reinterpret_cast<char*>(&partida), sizeof(PartidaStruct))) {
        archivo.close();
        return false;
    }

    JugadorStruct j{};
    for(const auto& jugador : std::as_const(jugadores)) {
        std::string nombre = jugador->getNombre().toStdString();

        std::memset(j.nombre, 0, sizeof(j.nombre));
        std::strncpy(j.nombre, nombre.c_str(), MAX_LEN_NOMBRE - 1);
        j.nombre[MAX_LEN_NOMBRE- 1] = '\0';

        j.color = jugador->getColor().rgb();
        j.posicionTablero = jugador->getPosicion();
        j.turnosPerdidos = jugador->getTurnosPerdidos();
        j.enPozo = jugador->getEnPozo();
        if (!archivo.write(reinterpret_cast<char*>(&j), sizeof(JugadorStruct))) {
            archivo.close();
            return false;
        }
    }

    CasillaStruct c{};
    for(const auto& casilla : std::as_const(casillas)) {
        c.numero = casilla->obtenerNumero();
        c.tipo = casilla->obtenerTipo();
        if(CasillaDeOca* oca = dynamic_cast<CasillaDeOca*>(casilla)) c.siguienteOca = oca->getSiguienteOca();
        else if(CasillaPuente* puente = dynamic_cast<CasillaPuente*>(casilla)) c.posOtroPuente = puente->getOtroPuentePos();

        if (!archivo.write(reinterpret_cast<char*>(&c), sizeof(CasillaStruct))) {
            archivo.close();
            return false;
        }
    }
    archivo.close();
    return true;
}


bool GestorDeGuardado::eliminarPartidaGuardada() {
    QFile archivo(this->direccionArchivo);
    if (!archivo.exists()) {
        qDebug() << "No existe archivo de partida guardada para eliminar.";
        return false;
    }
    if (!archivo.remove()) {
        qCritical() << "No se pudo eliminar el archivo de partida guardada:" << this->direccionArchivo;
        return false;
    }
    return true;
}

bool GestorDeGuardado::hayPartidaGuardada() {
    std::ifstream archivo(this->direccionArchivo.toStdString(), std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekg(0, std::ios::end);
    std::streampos tamaño = archivo.tellg();
    archivo.close();

    return tamaño > 0;
}

std::optional<JuegoStruct> GestorDeGuardado::cargarPartida() {
    JuegoStruct juego;
    std::ifstream archivo(this->direccionArchivo.toStdString(), std::ios::binary);
    if(!archivo.is_open()) return std::nullopt;
    PartidaStruct partida{};
    archivo.read(reinterpret_cast<char*>(&partida), sizeof(PartidaStruct));

    std::vector<JugadorStruct> jugadores;
    for(int i = 0; i < partida.cantidadJugadores; ++i) {
        JugadorStruct jugador{};
        archivo.read(reinterpret_cast<char*>(&jugador), sizeof(JugadorStruct));
        if (archivo.fail()) return std::nullopt;
        jugadores.push_back(jugador);
    }

    std::vector<CasillaStruct> casillas{};
    for(int i = 0; i < partida.sizeTablero; ++i) {
        CasillaStruct casilla{};
        archivo.read(reinterpret_cast<char*>(&casilla), sizeof(CasillaStruct));
        if (archivo.fail()) return std::nullopt;
        casillas.push_back(casilla);
    }
    juego.casillas = casillas;
    juego.jugadores = jugadores;
    juego.partida = partida;
    return juego;
}
