#ifndef TABLERO_H
#define TABLERO_H

#include "../Casillas/casilla.h"
#include <QVector>
#include <QDebug>
#include <vector>
#include <cassert>
#include <random>
#include <QSet>

class Jugador;

class Tablero
{
public:
    Tablero(int cantCasillas);
    std::vector<Casilla*> obtenerCasillas() const;
    Casilla* obtenerCasilla(int indice) const;
    void setCasillas(std::vector<Casilla*>&& casillas);
    void reiniciar();
    ~Tablero();
private:
    void configurarCasillasPuentes();
    QSet<int> posicionesOcupadasInterna;
    QSet<int> posicionesNoElegiblesInterna;
    void configurarCaminoDeOcas();
    int cantCasillas;
    std::vector<Casilla*> casillas{};
    void cargarCasilla();
    void iniciarTablero();
    void inicializarCasillasNormales();
    void asegurarCasillasFijas();
    void marcarRangoExclusion(int pos_indice, int distancia);
    void colocarPuentes(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarLaberinto(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarCalavera(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarPozos(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarPosadas(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarCarceles(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    void colocarOcas(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles);
    std::vector<int> obtenerPosicionesDisponiblesPara(const QSet<int>& ocupadas, const QSet<int>& noElegibles);
};

#endif // TABLERO_H
