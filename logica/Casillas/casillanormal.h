#ifndef CASILLANORMAL_H
#define CASILLANORMAL_H

#include "casilla.h"

class CasillaNormal : public Casilla
{
public:
    CasillaNormal(int num);
    void aplicarEfecto(Jugador& jugador) override;
    virtual ~CasillaNormal() = default;
};

#endif // CASILLANORMAL_H
