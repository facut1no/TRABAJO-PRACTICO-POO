#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <QColor>

class Jugador
{
public:
    Jugador() = default;
    Jugador(QString nombre, QColor color);
    QString getNombre() const;
    QColor color;

    void mover(int pasos, int cantCasillas);

    QColor getColor() const;
    int getPosicion() const;
    bool getEnPozo() const;
    int getTurnosPerdidos() const;
    bool esGanador() const;
    bool puedeJugar() const;
    bool getEfectoAplicado() const;
    int getPosicionAnterior() const;

    void reiniciar();
    void setPosicionAnterior();
    void setPosicion(int nuevaPosicion);
    void setEnPozo(bool estado);
    void setTurnosPerdidos(int turnos);
    void setEsGanador(bool estado);
    void restarTurno();
    void setEfectoAplicado(bool estado);

private:
    QString m_Nombre;
    int m_Posicion;
    bool m_EnPozo;
    int m_TurnosPerdidos;
    bool m_EsGanador;
    QColor m_Color;
    bool m_EfectoAplicado;
    int m_PosicionAnterior;
};

#endif // JUGADOR_H
