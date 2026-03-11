#ifndef CASILLADEOCA_H
#define CASILLADEOCA_H

#include "casilla.h"

class CasillaDeOca : public Casilla
{
private:
    int siguienteOca;
public:
    CasillaDeOca(int num);
    void aplicarEfecto(Jugador& jugador) override;
    void setSiguienteOca(int siguiente);
    int getSiguienteOca() const;
};

#endif // CASILLADEOCA_H
