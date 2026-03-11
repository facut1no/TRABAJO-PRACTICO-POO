#include "juegooca.h"
#include <qdebug.h>
#include "../Dado/dado.h"


JuegoOca::JuegoOca(int cantCasillas, int cantDados) {
    this->cantCasillas = cantCasillas;
    this->tablero = new Tablero(cantCasillas);
    this->cantidadDados = cantDados;
    this->jugadorActual = nullptr;
    this->jugadorEnPozo = nullptr;
    this->turnoActual = 0;
    this->valorDado = 0;
    this->vuelveATirar = false;
    this->cantidadDadosJuego = cantDados;
    for(int i = 0; i < this->cantidadDados; ++i) {
        this->dados.push_back(new Dado());
    }
}

void JuegoOca::cargarDatosJugadores(QList<QPair<QString, QColor>> datos) {
    if(!jugadores.isEmpty())
        jugadores.clear();
    for(const auto& dato : datos) {
        Jugador* jugador = new Jugador(dato.first, dato.second);
        this->jugadores.push_back(jugador);
    }
    this->jugadorActual = this->jugadores[0];
}

Tablero* JuegoOca::obtenerTablero() const {
    return this->tablero;
}

Jugador* JuegoOca::obtenerJugadorActual() const {
    return this->jugadores[this->turnoActual];
}

QVector<Jugador*> JuegoOca::obtenerJugadores() {
    return this->jugadores;
}

QVector<Jugador*> JuegoOca::obtenerJugadoresOrdenados() {
    QVector<Jugador*> copia = this->jugadores;

    std::sort(copia.begin(), copia.end(), [](Jugador* a, Jugador* b) {
        return a->getPosicion() > b->getPosicion();
    });
    return copia;
}

void JuegoOca::sacarJugadorDelPozo() {
    if(this->jugadorEnPozo != nullptr && this->jugadorEnPozo != this->jugadorActual) {
        this->jugadorEnPozo->setEnPozo(false);
    }
}

void JuegoOca::meterJugadorAlPozo() {
    this->jugadorEnPozo = this->jugadorActual;
    this->jugadorActual->setEnPozo(true);
}

void JuegoOca::siguienteTurno() {
    this->turnoActual = (this->turnoActual + 1) % jugadores.size();
    this->jugadorActual = this->obtenerJugadorActual();
}

void JuegoOca::aplicarEfectoCasilla() {
    int posicion = this->jugadorActual->getPosicion();
    Casilla* casilla = this->obtenerCasilla(posicion);

    if(!casilla) {
        qDebug() << "Casilla nullptr en posicion " << posicion;
        return;
    }

    TipoCasilla tipo = casilla->obtenerTipo();

    switch(tipo) {
    case TipoCasilla::Pozo:
        this->sacarJugadorDelPozo();
        this->meterJugadorAlPozo();
        break;

    case TipoCasilla::Oca: {
        this->vuelveATirar = true;
        break;
    }
    default:
        break;
    }
    casilla->aplicarEfecto(*this->jugadorActual);
}
// void JuegoOca::aplicarEfectoCasilla() {
//     Casilla* casilla = this->obtenerCasilla(this->jugadorActual->getPosicion() - 1);
//     if(!casilla) {
//         qDebug() << "Casilla nullptr en posicion " << this->jugadorActual->getPosicion() - 1;
//     }
//     TipoCasilla tipo = casilla->obtenerTipo();
//     if(tipo == Pozo) {
//         this->sacarJugadorDelPozo();
//         this->meterJugadorAlPozo();
//     } else if (tipo == Oca) {
//         this->vuelveATirar = true;
//         this->jugadorActual->mover(this->siguienteOca()->obtenerNumero() - this->jugadorActual->getPosicion());
//     } else {
//         casilla->aplicarEfecto(*this->jugadorActual);
//     }

// }


void JuegoOca::actualizarEstadoJuegoCargado() {
    for(Jugador* jugador : this->jugadores) {
        if(jugador->getEnPozo()) {
            this->jugadorActual = jugador;
            this->sacarJugadorDelPozo();
            this->meterJugadorAlPozo();
        }
    }
}

void JuegoOca::tirarDado() {
    if(this->jugadorActual->getPosicion() >= 59) { this->cantidadDados = 1; }
    else { this->cantidadDados = 2; }
    this->valorDado = 0;
    if (this->cantidadDados > 1) {
        for (Dado* dado : this->dados) {
            int valor = dado->tirar();
            this->valorDado += valor;
        }
    } else {
        this->valorDado = this->dados[0]->tirar();
    }
}

int JuegoOca::obtenerValorDado() const {
    return this->valorDado;
}

const std::vector<Dado*> JuegoOca::obtenerDados() {
    return this->dados;
}

std::vector<int> JuegoOca::obtenerValoresDados() {
    std::vector<int> valores{};
    for(const auto& dado : this->dados) {
        valores.push_back(dado->getValor());
    }
    return valores;
}

bool JuegoOca::hayGanador() {
    for(const Jugador* jugador : std::as_const(this->jugadores)) {
        if(jugador->esGanador()) {
            return true;
        }
    }
    return false;
}

Jugador* JuegoOca::obtenerGanador() {
    for(Jugador* jugador : std::as_const(this->jugadores)) {
        if(jugador->esGanador()) {
            return jugador;
        }
    }
    return nullptr;
}

void JuegoOca::terminarTurno() {
    if(this->vuelveATirar && this->jugadorActual->puedeJugar()) {
        this->vuelveATirar = false;
        return;
    }
    this->vuelveATirar = false;
    this->siguienteTurno();
}


Jugador* JuegoOca::obtenerJugadorDelPozo() {
    return this->jugadorEnPozo;
}

void JuegoOca::reiniciar() {
    this->turnoActual = 0;
    this->jugadorActual = nullptr;
    this->jugadorEnPozo = nullptr;
    this->vuelveATirar = false;
    qDeleteAll(this->jugadores);
    this->jugadores.clear();
}

bool JuegoOca::jugadorPuedeJugar() const {
    return this->jugadorActual->puedeJugar();
}

void JuegoOca::setTurno(int turno) {
    this->turnoActual = turno;
    if (!jugadores.empty())
        this->jugadorActual = jugadores[turno];
}


void JuegoOca::restarTurnoJugador() {
    if(this->jugadorActual->getTurnosPerdidos() > 0) {
        this->jugadorActual->restarTurno();
    }
}

Casilla* JuegoOca::obtenerCasilla(int indice) const {
    return this->tablero->obtenerCasilla(indice);
}

void JuegoOca::setCantidadDados(int cantidad) {
    this->cantidadDados = cantidad;
}

void JuegoOca::volverAJugar() {
    for(Jugador* jugador : this->jugadores) {
        jugador->reiniciar();
    }
    this->turnoActual = 0;
    this->tablero->reiniciar();
    this->jugadorActual = nullptr;
    this->jugadorEnPozo = nullptr;
}

void JuegoOca::moverJugador() {
    int dado1 = this->dados[0]->getValor();
    int dado2 = (this->cantidadDados > 1) ? this->dados[1]->getValor() : 0;

    int suma = dado1 + dado2;

    if (this->jugadorActual->getPosicion() <= 0 && suma == 9 && this->cantidadDados > 1) {
        if ((dado1 == 3 && dado2 == 6) || (dado1 == 6 && dado2 == 3)) {
            this->jugadorActual->setPosicion(35);
            return;
        }
        else if ((dado1 == 4 && dado2 == 5) || (dado1 == 5 && dado2 == 4)) {
            this->jugadorActual->setPosicion(53);
            return;
        }
    }

    this->jugadorActual->mover(this->valorDado, this->cantCasillas);
}



int JuegoOca::obtenerCantidadDados() {
    return this->cantidadDadosJuego;
}

int JuegoOca::obtenerTurnoActual() {
    return this->turnoActual;
}

void JuegoOca::cargarJugadoresGuardados(const std::vector<Jugador>& jugadores) {
    for(Jugador* jugador : this->jugadores) {
        delete jugador;
    }
    this->jugadores.clear();

    for(const Jugador& j: jugadores) {
        Jugador* jugador = new Jugador(j.getNombre(), j.getColor());
        jugador->setTurnosPerdidos(j.getTurnosPerdidos());
        jugador->setPosicion(j.getPosicion());
        this->jugadores.push_back(jugador);
    }
}

JuegoOca::~JuegoOca() {
    qDeleteAll(this->jugadores);
    this->jugadores.clear();
    qDeleteAll(this->dados);
      this->dados.clear();

    delete tablero;
    tablero = nullptr;

    jugadorEnPozo = nullptr;
    jugadorActual = nullptr;
}


