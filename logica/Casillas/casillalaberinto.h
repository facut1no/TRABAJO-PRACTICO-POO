#ifndef CASILLALABERINTO_H
#define CASILLALABERINTO_H

#include "casilla.h"

class CasillaLaberinto : public Casilla
{
public:
    CasillaLaberinto(int num);
    void aplicarEfecto(Jugador& jugador) override;
};

#endif // CASILLALABERINTO_H
