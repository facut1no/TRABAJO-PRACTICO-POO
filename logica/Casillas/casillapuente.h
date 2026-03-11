#ifndef CASILLAPUENTE_H
#define CASILLAPUENTE_H

#include "casilla.h"

class CasillaPuente : public Casilla
{
private:
    int posOtroPuente;
public:
    CasillaPuente(int num);
    void aplicarEfecto(Jugador& jugador) override;
    void setPosOtroPuente(int);
    int getOtroPuentePos() { return this->posOtroPuente; }
};

#endif // CASILLAPUENTE_H
