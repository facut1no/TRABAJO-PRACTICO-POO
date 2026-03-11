#ifndef CASILLAJARDINDEOCA_H
#define CASILLAJARDINDEOCA_H

#include "casilla.h"

class CasillaJardinDeOca : public Casilla
{
public:
    CasillaJardinDeOca(int num);
    void aplicarEfecto(Jugador& jugador) override;
};

#endif // CASILLAJARDINDEOCA_H
