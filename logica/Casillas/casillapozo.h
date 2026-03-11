#ifndef CASILLAPOZO_H
#define CASILLAPOZO_H

#include "casilla.h"

class CasillaPozo : public Casilla
{
public:
    CasillaPozo(int num);
    void aplicarEfecto(Jugador& jugador) override;
};

#endif // CASILLAPOZO_H
