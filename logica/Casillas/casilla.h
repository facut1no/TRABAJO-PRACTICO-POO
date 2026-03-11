#ifndef CASILLA_H
#define CASILLA_H

#include <QString>
#include "../Jugador/jugador.h"

enum TipoCasilla : uint8_t{
    Normal,
    Calavera,
    Carcel,
    Oca,
    JardinDeOca,
    Posada,
    Pozo,
    Puente,
    Laberinto
};

class Casilla {
protected:
    TipoCasilla m_Tipo;
    int m_Numero{};
    QString m_Mensaje{};
public:
    Casilla(int num, TipoCasilla tipo = TipoCasilla::Normal);
    QString getMensaje() const;
    virtual void aplicarEfecto(Jugador &jugador) = 0;
    int obtenerNumero() const;
    TipoCasilla obtenerTipo() const;
    virtual ~Casilla() = default;
};

#endif // CASILLA_H
