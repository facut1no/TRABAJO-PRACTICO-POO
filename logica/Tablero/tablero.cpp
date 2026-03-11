#include "tablero.h"
#include <QVector>
#include "../Casillas/casilladeoca.h"
#include "../Casillas/casillacalavera.h"
#include "../Casillas/casillacarcel.h"
#include "../Casillas/casillajardindeoca.h"
#include "../Casillas/casillalaberinto.h"
#include "../Casillas/casillanormal.h"
#include "../Casillas/casillaposada.h"
#include "../Casillas/casillapozo.h"
#include "../Casillas/casillapuente.h"
#include <QDebug>
#include <algorithm>
#include <vector>
#include <cassert>
#include <random>
#include <chrono>
#include <algorithm>
#include <QSet>

Tablero::Tablero(int cantCasilla) {
    this->cantCasillas = cantCasilla;
    this->casillas.reserve(this->cantCasillas);
    cargarCasilla();


}

void Tablero::reiniciar() {
    for(Casilla* casilla : this->casillas) {
        delete casilla;
    }
    casillas.clear();
    this->casillas.reserve(this->cantCasillas);
    this->cargarCasilla();
}

// void Tablero::cargarCasilla() {
//     for (Casilla* casilla : this->casillas) delete casilla;
//     this->casillas.clear();
//     this->casillas.reserve(this->cantCasillas);

//     inicializarCasillasNormales();

//     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//     std::default_random_engine rng(seed);

//     QSet<int> ocupadas;
//     ocupadas.insert(0);
//     ocupadas.insert(this->cantCasillas - 1);

//     // int distanciaMin = std::max(2, this->cantCasillas / 20);
//     // int distanciaOcas = std::max(5, this->cantCasillas / 12);
//     // int distanciaPeligros = std::max(8, this->cantCasillas / 10);

//     auto posicionesDisponibles = [&]() {
//         std::vector<int> libres;
//         for (int i = 1; i < this->cantCasillas - 1; ++i)
//             if (!ocupadas.contains(i)) libres.push_back(i);
//         std::shuffle(libres.begin(), libres.end(), rng);
//         return libres;
//     };

//     auto colocarCasilla = [&](int pos, Casilla* nueva) {
//         if (pos > 0 && pos < this->cantCasillas - 1) {
//             delete this->casillas[pos];
//             this->casillas[pos] = nueva;
//             ocupadas.insert(pos);
//         }
//     };

//     int numOcas = std::max(4, this->cantCasillas / 10);
//     int espacioEntreOcas = this->cantCasillas / (numOcas + 1);
//     for (int i = 1; i <= numOcas; ++i) {
//         int pos = i * espacioEntreOcas + (rng() % 2 - 1);
//         colocarCasilla(pos, new CasillaDeOca(pos + 1));
//     }

//     auto libres = posicionesDisponibles();
//     if (libres.size() > 20) {
//         int pos1 = libres[rng() % (libres.size() / 2)];
//         int pos2 = pos1 + 12 < this->cantCasillas - 1 ? pos1 + 12 : pos1 - 12;
//         colocarCasilla(pos1, new CasillaPuente(pos1 + 1));
//         colocarCasilla(pos2, new CasillaPuente(pos2 + 1));
//     }

//     libres = posicionesDisponibles();
//     if (!libres.empty()) {
//         int pos = libres[libres.size() * 3 / 4];
//         colocarCasilla(pos, new CasillaCalavera(pos + 1));
//     }

//     libres = posicionesDisponibles();
//     if (!libres.empty()) {
//         int pos = libres[libres.size() * 2 / 3];
//         colocarCasilla(pos, new CasillaLaberinto(pos + 1));
//     }

//     libres = posicionesDisponibles();
//     for (int i = 0; i < std::min(2, (int)libres.size()); ++i)
//         colocarCasilla(libres[i], new CasillaPosada(libres[i] + 1));

//     libres = posicionesDisponibles();
//     for (int i = 0; i < std::min(2, (int)libres.size()); ++i)
//         colocarCasilla(libres[i], new CasillaPozo(libres[i] + 1));

//     libres = posicionesDisponibles();
//     for (int i = 0; i < std::min(2, (int)libres.size()); ++i)
//         colocarCasilla(libres[i], new CasillaCarcel(libres[i] + 1));

//     delete this->casillas[this->cantCasillas - 1];
//     this->casillas[this->cantCasillas - 1] = new CasillaJardinDeOca(this->cantCasillas);

//     this->configurarCaminoDeOcas();
//     this->configurarCasillasPuentes();
// }


void Tablero::cargarCasilla() {
    for (Casilla* casilla : this->casillas) {
        delete casilla;
    }
    this->casillas.clear();
    this->casillas.reserve(this->cantCasillas);

    inicializarCasillasNormales();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    QSet<int> posicionesOcupadas;
    QSet<int> posicionesNoElegibles;

    posicionesOcupadas.insert(0);
    posicionesNoElegibles.insert(0);
    posicionesOcupadas.insert(this->cantCasillas - 1);
    posicionesNoElegibles.insert(this->cantCasillas - 1);

    colocarPuentes(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarLaberinto(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarCalavera(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarPozos(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarPosadas(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarCarceles(rng, posicionesOcupadas, posicionesNoElegibles);
    colocarOcas(rng, posicionesOcupadas, posicionesNoElegibles);

    asegurarCasillasFijas();


    this->configurarCaminoDeOcas();
    this->configurarCasillasPuentes();
}

void Tablero::inicializarCasillasNormales() {
    for (int i = 1; i <= this->cantCasillas; ++i) {
        this->casillas.push_back(new CasillaNormal(i));
    }
}

void Tablero::asegurarCasillasFijas() {
    if (this->casillas[0]->obtenerNumero() != 1 || this->casillas[0]->obtenerTipo() != TipoCasilla::Normal) {
        delete this->casillas[0];
        this->casillas[0] = new CasillaNormal(1);
    }
    delete this->casillas[this->cantCasillas - 1];
    this->casillas[this->cantCasillas - 1] = new CasillaJardinDeOca(this->cantCasillas);
}

void Tablero::marcarRangoExclusion(int pos_indice, int distancia) {
    for (int d = 1; d <= distancia; ++d) {
        int nextPos = pos_indice + d;
        if (nextPos < this->cantCasillas - 1) this->posicionesNoElegiblesInterna.insert(nextPos);
        int prevPos = pos_indice - d;
        if (prevPos > 0) this->posicionesNoElegiblesInterna.insert(prevPos);
    }
}

std::vector<int> Tablero::obtenerPosicionesDisponiblesPara(const QSet<int>& ocupadas, const QSet<int>& noElegibles) {
    std::vector<int> disponibles;
    for (int i = 1; i < this->cantCasillas - 1; ++i) {
        if (!ocupadas.contains(i) && !noElegibles.contains(i)) {
            disponibles.push_back(i);
        }
    }
    return disponibles;
}


void Tablero::colocarPuentes(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    int puentesColocados = 0;
    int distanciaPuente = 12;

    std::vector<int> posiblesPuente = obtenerPosicionesDisponiblesPara(ocupadas, noElegibles);
    std::shuffle(posiblesPuente.begin(), posiblesPuente.end(), rng);

    for (int intento = 0; intento < posiblesPuente.size() && puentesColocados < 2; ++intento) {
        int pos1 = posiblesPuente[intento];
        if (ocupadas.contains(pos1) || noElegibles.contains(pos1)) continue;

        for (int intento2 = intento + 1; intento2 < posiblesPuente.size() && puentesColocados < 2; ++intento2) {
            int pos2 = posiblesPuente[intento2];
            if (ocupadas.contains(pos2) || noElegibles.contains(pos2)) continue;

            if (qAbs(pos1 - pos2) == distanciaPuente) {
                delete this->casillas[pos1];
                this->casillas[pos1] = new CasillaPuente(pos1 + 1);
                ocupadas.insert(pos1);
                marcarRangoExclusion(pos1, 2);
                puentesColocados++;

                delete this->casillas[pos2];
                this->casillas[pos2] = new CasillaPuente(pos2 + 1);
                ocupadas.insert(pos2);
                marcarRangoExclusion(pos2, 2);
                puentesColocados++;

                break;
            }
        }
    }
}

void Tablero::colocarLaberinto(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    std::vector<int> posiblesLaberinto;
    for (int i = 39; i < this->cantCasillas - 1; ++i) {
        if (!ocupadas.contains(i) && !noElegibles.contains(i)) {
            posiblesLaberinto.push_back(i);
        }
    }
    std::shuffle(posiblesLaberinto.begin(), posiblesLaberinto.end(), rng);

    if (!posiblesLaberinto.empty()) {
        int pos = posiblesLaberinto.front();
        delete this->casillas[pos];
        this->casillas[pos] = new CasillaLaberinto(pos + 1);
        ocupadas.insert(pos);
        marcarRangoExclusion(pos, 2);
    }
}

void Tablero::colocarCalavera(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    std::vector<int> posiblesCalavera;
    for (int i = 29; i < this->cantCasillas - 1; ++i) {
        if (!ocupadas.contains(i) && !noElegibles.contains(i)) {
            posiblesCalavera.push_back(i);
        }
    }
    std::shuffle(posiblesCalavera.begin(), posiblesCalavera.end(), rng);

    if (!posiblesCalavera.empty()) {
        int pos = posiblesCalavera.front();
        delete this->casillas[pos];
        this->casillas[pos] = new CasillaCalavera(pos + 1);
        ocupadas.insert(pos);
        marcarRangoExclusion(pos, 2);
    }
}

void Tablero::colocarPozos(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    int pozosColocados = 0;
    int distanciaMinimaPozo = 20;

    std::vector<int> posiblesPozo = obtenerPosicionesDisponiblesPara(ocupadas, noElegibles);
    std::shuffle(posiblesPozo.begin(), posiblesPozo.end(), rng);

    for (int intento = 0; intento < posiblesPozo.size() && pozosColocados < 2; ++intento) {
        int pos1 = posiblesPozo[intento];
        if (ocupadas.contains(pos1) || noElegibles.contains(pos1)) continue;

        for (int intento2 = intento + 1; intento2 < posiblesPozo.size() && pozosColocados < 2; ++intento2) {
            int pos2 = posiblesPozo[intento2];
            if (ocupadas.contains(pos2) || noElegibles.contains(pos2)) continue;

            if (qAbs(pos1 - pos2) >= distanciaMinimaPozo) {
                delete this->casillas[pos1];
                this->casillas[pos1] = new CasillaPozo(pos1 + 1);
                ocupadas.insert(pos1);
                marcarRangoExclusion(pos1, 2);
                pozosColocados++;

                delete this->casillas[pos2];
                this->casillas[pos2] = new CasillaPozo(pos2 + 1);
                ocupadas.insert(pos2);
                marcarRangoExclusion(pos2, 2);
                pozosColocados++;

                break;
            }
        }
    }
}

void Tablero::colocarPosadas(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    int posadasColocadas = 0;
    int distanciaMinimaPosada = 20;

    std::vector<int> posiblesPosada = obtenerPosicionesDisponiblesPara(ocupadas, noElegibles);
    std::shuffle(posiblesPosada.begin(), posiblesPosada.end(), rng);

    for (int intento = 0; intento < posiblesPosada.size() && posadasColocadas < 2; ++intento) {
        int pos1 = posiblesPosada[intento];
        if (ocupadas.contains(pos1) || noElegibles.contains(pos1)) continue;

        for (int intento2 = intento + 1; intento2 < posiblesPosada.size() && posadasColocadas < 2; ++intento2) {
            int pos2 = posiblesPosada[intento2];
            if (ocupadas.contains(pos2) || noElegibles.contains(pos2)) continue;

            if (qAbs(pos1 - pos2) >= distanciaMinimaPosada) {
                delete this->casillas[pos1];
                this->casillas[pos1] = new CasillaPosada(pos1 + 1);
                ocupadas.insert(pos1);
                marcarRangoExclusion(pos1, 2);
                posadasColocadas++;

                delete this->casillas[pos2];
                this->casillas[pos2] = new CasillaPosada(pos2 + 1);
                ocupadas.insert(pos2);
                marcarRangoExclusion(pos2, 2);
                posadasColocadas++;

                break;
            }
        }
    }

}


void Tablero::colocarCarceles(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    int maxCarceles = 3;
    int distanciaMinimaCarcel = 15;
    int carcelesColocadas = 0;

    std::vector<int> posiblesCarcel = obtenerPosicionesDisponiblesPara(ocupadas, noElegibles);
    std::shuffle(posiblesCarcel.begin(), posiblesCarcel.end(), rng);

    QList<int> carcelesExistentesIndices;

    for (int intento = 0; intento < posiblesCarcel.size() && carcelesColocadas < maxCarceles; ++intento) {
        int pos = posiblesCarcel[intento];

        bool cumpleDistancia = true;
        for (int carcelPos : carcelesExistentesIndices) {
            if (qAbs(pos - carcelPos) < distanciaMinimaCarcel) {
                cumpleDistancia = false;
                break;
            }
        }

        if (cumpleDistancia && !ocupadas.contains(pos) && !noElegibles.contains(pos)) {
            delete this->casillas[pos];
            this->casillas[pos] = new CasillaCarcel(pos + 1);
            ocupadas.insert(pos);
            marcarRangoExclusion(pos, 2);
            carcelesExistentesIndices.append(pos);
            carcelesColocadas++;
        }
    }
}

void Tablero::colocarOcas(std::default_random_engine& rng, QSet<int>& ocupadas, QSet<int>& noElegibles) {
    int numOcas = std::max(2, this->cantCasillas / 15);
    int ocasColocadas = 0;
    for(int i = 0; i < this->cantCasillas; ++i) {
        if(this->casillas[i]->obtenerTipo() == TipoCasilla::Oca) ocasColocadas++;
    }
    int distanciaMinimaOca = 9;

    std::vector<int> posiblesOcasRestantes = obtenerPosicionesDisponiblesPara(ocupadas, noElegibles);
    std::shuffle(posiblesOcasRestantes.begin(), posiblesOcasRestantes.end(), rng);

    for (int i = 0; i < posiblesOcasRestantes.size() && ocasColocadas < numOcas; ++i) {
        int pos = posiblesOcasRestantes[i];

        bool cumpleDistanciaOca = true;
        for (int j = 0; j < this->cantCasillas; ++j) {
            if (this->casillas[j]->obtenerTipo() == TipoCasilla::Oca && qAbs(pos - j) < distanciaMinimaOca) {
                cumpleDistanciaOca = false;
                break;
            }
        }

        if (cumpleDistanciaOca && !ocupadas.contains(pos) && !noElegibles.contains(pos)) {
            delete this->casillas[pos];
            this->casillas[pos] = new CasillaDeOca(pos + 1);
            ocupadas.insert(pos);
            marcarRangoExclusion(pos, 2);
            ocasColocadas++;
        }
    }
}

void Tablero::configurarCaminoDeOcas() {
    std::vector<CasillaDeOca*> ocas;

    for (Casilla* casilla : std::as_const(this->casillas)) {
        if (CasillaDeOca* oca = dynamic_cast<CasillaDeOca*>(casilla)) {
            ocas.push_back(oca);
        }
    }

    std::sort(ocas.begin(), ocas.end(),
              [](CasillaDeOca* a, CasillaDeOca* b) {
                  return a->obtenerNumero() < b->obtenerNumero();
              });

    for (size_t i = 0; i < ocas.size(); i++) {
        if (i + 1 < ocas.size()) {
            ocas[i]->setSiguienteOca(ocas[i + 1]->obtenerNumero() - 1);
        } else {
            ocas[i]->setSiguienteOca(-1);
        }
    }
}

void Tablero::configurarCasillasPuentes() {
    std::vector<CasillaPuente*> puentes;
    for(Casilla* casilla : std::as_const(this->casillas)) {
        if(CasillaPuente* puente = dynamic_cast<CasillaPuente*>(casilla)) {
            puentes.push_back(puente);
        }
        if(puentes.size() >= 2) { break; }
    }
    puentes[0]->setPosOtroPuente(puentes[1]->obtenerNumero() - 1);
    puentes[1]->setPosOtroPuente(puentes[0]->obtenerNumero() - 1);
}


std::vector<Casilla*> Tablero::obtenerCasillas() const {
    return this->casillas;
}

void Tablero::setCasillas(std::vector<Casilla*>&& nuevasCasillas) {
    for (Casilla* c : this->casillas) {
        if(c)
            delete c;
    }
    this->casillas.clear();

    this->casillas = std::move(nuevasCasillas);
    for(const auto& casilla : this->casillas) {
        if(casilla->obtenerTipo() == TipoCasilla::Calavera || casilla->obtenerTipo() == TipoCasilla::Laberinto)
            qDebug() << casilla->obtenerNumero() << ": " << casilla->getMensaje();
    }
}



Casilla* Tablero::obtenerCasilla(int indice) const {
    if(indice < 0 || indice > this->cantCasillas) {
        return nullptr;
    }
    return this->casillas[indice];
}

Tablero::~Tablero() {
    qDeleteAll(this->casillas);
    this->casillas.clear();
}
