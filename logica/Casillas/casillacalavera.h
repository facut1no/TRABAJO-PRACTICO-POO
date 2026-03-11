#ifndef CASILLACALAVERA_H
#define CASILLACALAVERA_H

#include "casilla.h"

class CasillaCalavera : public Casilla
{
public:
    CasillaCalavera(int numero);
    void aplicarEfecto(Jugador& jugador) override;
};

#endif // CASILLACALAVERA_H
