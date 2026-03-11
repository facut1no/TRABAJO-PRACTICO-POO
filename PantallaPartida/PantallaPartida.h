#ifndef PANTALLAPARTIDA_H
#define PANTALLAPARTIDA_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include "../TableroUI/tableroui.h"
#include "../JugadorUI/jugadorui.h"
#include "../logica/JuegoOca/juegooca.h"
#include "notificacion.h"

namespace Ui {
class PantallaPartida;
}

class PantallaPartida : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaPartida(QWidget *parent = nullptr);
    ~PantallaPartida();
    void cargarUI(int cantCasillas, int cantDados);
    void cargarJugadores(const QList<QPair<QString, QColor>>& jugadores);
    void reiniciar();
    void limpiarTablero();
    void volverAJugar();
public slots:
    void guardarPartida();
    void cargarPartida();
signals:
    void ponerEnPausa();
    void animacionDadoTerminada(bool);
    void movimientoJugadorTerminado();
    void turnoTerminado();
    void juegoTerminado(const QList<Jugador*>&);
    void casillasCargadas();
    void partidaGuardada();
    void partidaCargada();
private slots:
    void jugarTurno();
    void cambiarTurno();
    void keyPressEvent(QKeyEvent *event);
    // void onDadoTirado(int resultado);
    // void onTurnoCambiado(Jugador* nuevoJugadorActual);
    // void onJugadorActualEnPozo(Jugador* jugadorLogico);
    // void onJugadorActualPierdeElTurno(Jugador* jugadorLogico);
    // void onJugadorEntraPozo(Jugador* jugadorLogico);
    // void onJugadorSalePozo(Jugador* jugadorLogico);
    // void onJugadorPierdeTurnos(Jugador* jugadorLogico, int numTurnos);
    // void onJugadorDebeVolverATirar(Jugador* jugadorLogico);
    // void onJuegoTerminado(Jugador* ganador);
    // void onJugadorCayoEnCalavera(Jugador* jugadorLogico);
    // void onJugadorCayoEnCarcel(Jugador* jugadorLogico);
    // void onJugadorCayoEnLaberinto(Jugador* jugadorLogico);
    // void onJugadorCayoEnOca(Jugador* jugadorLogico);
    // void onJugadorCayoEnPosada(Jugador* jugadorLogico);
    // void onJugadorCayoEnJardinDeOca(Jugador* jugadorLogico);
    // void onJugadorCayoEnPuente(Jugador* jugadorLogico);
    // void onJugadorCayoEnPozo(Jugador* jugadorLogico);
    // void onJugadorCayoEnNormal(Jugador* jugadorLogico);

private:
    void cargarJugadoresGuardados();
    int cantDados;
    void moverJugador(Jugador*);
    void mostrarNotificacion(QString, QString, std::function<void()>);
    bool debeMoversePorEfecto(Jugador* jugador);
    void animarMovimientoPostEfecto(Jugador* jugador);
    void finalizarTurno(Jugador* jugador);
    void aplicarEfectoInicialDeCasilla(Jugador* jugador);
    Ui::PantallaPartida *ui;
    Notificacion* notificacion;
    TableroUI* tableroui;
    QVector<JugadorUI*> jugadoresUI;
    JugadorUI* jugadorUIActual;
    JuegoOca* juego{nullptr};
    std::vector<QLabel*> dadosUI;
    QMap<Jugador*, JugadorUI*> mapaJugadorToUI;
    void btnTirarDadoClickeado();
    void enlistarJugadores();
    QTimer* timerAnimacionDado;
    int dadoAnimacionFrame;
    int dadoValorFinal;
    void iniciarAnimacionDado(int);
    void actualizarFrameDado();
    void finalizarAnimacionDado();
    void aplicarEfectoCasilla(Jugador*);
    QList<CasillaUI*> obtenerRutaDeMovimiento(int, int, bool);
    void animarMovimientoJugador(int, int, bool);
    void setTextLabelTurno();
    void realizarConexionesJugadorUI();
    void marcarRuta(QList<CasillaUI*>);
};

#endif // PANTALLAPARTIDA_H
