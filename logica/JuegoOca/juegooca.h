#ifndef JUEGOOCA_H
#define JUEGOOCA_H

#include "../Tablero/tablero.h"
#include "../Jugador/jugador.h"
#include "../Dado/dado.h"
#include <QObject>



class JuegoOca : public QObject {
    Q_OBJECT
public:
    JuegoOca(int cantCasillas, int cantDados);
    JuegoOca() = default;
    void cargarDatosJugadores(QList<QPair<QString, QColor>> datos);
    Jugador* obtenerJugadorActual() const;
    Tablero* obtenerTablero() const;
    void aplicarEfectoCasilla();
    int obtenerValorDado() const;
    void tirarDado();
    QVector<Jugador*> obtenerJugadoresOrdenados();
    QVector<Jugador*> obtenerJugadores();
    bool hayGanador();
    Jugador* obtenerGanador();
    void moverJugador();
    void terminarTurno();
    void sacarJugadorDelPozo();
    void meterJugadorAlPozo();
    Jugador* obtenerJugadorDelPozo();
    void reiniciar();
    bool jugadorPuedeJugar() const;
    void restarTurnoJugador();
    Casilla* obtenerCasilla(int) const;
    void setTurno(int);
    void setCantidadDados(int);
    std::vector<int> obtenerValoresDados();
    const std::vector<Dado*> obtenerDados();
    int obtenerCantidadDados();
    int obtenerTurnoActual();
    void  cargarJugadoresGuardados(const std::vector<Jugador>& jugadores);
    void volverAJugar();
    int getCantCasillas() {return this->cantCasillas;}
    ~JuegoOca();
    bool partidaGuardada = false;
    void actualizarEstadoJuegoCargado();
signals:
    void jugadorCayoEnCasilla(TipoCasilla);
private:
    int cantidadDadosJuego;
    int cantCasillas;
    int cantidadDados;
    int cantJugadore;
    int turnoActual;
    Tablero* tablero;
    QVector<Jugador*> jugadores;
    Jugador* jugadorEnPozo;
    Jugador* jugadorActual;
    std::vector<Dado*> dados;
    int valorDado;
    bool vuelveATirar;
    void siguienteTurno();
    void emitirSignalEfectoCasilla(const TipoCasilla);
    void seguirTurnoDespuesDeAnimacionMovimiento();
    void seguirTurnoDespuesDeAnimacionDado();
};

#endif // JUEGOOCA_H
