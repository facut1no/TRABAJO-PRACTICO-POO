#ifndef GESTORDEGUARDADO_H
#define GESTORDEGUARDADO_H
#include "../logica/JuegoOca/juegooca.h"

#include <QCoreApplication>
#include "../logica/Casillas/casilla.h"
#include <QDir>

static constexpr int JUGADOR_NOMBRE_BUFFER_SIZE = 32;
#define MAX_LEN_NOMBRE (JUGADOR_NOMBRE_BUFFER_SIZE - 1)


struct JugadorStruct {
    char nombre[JUGADOR_NOMBRE_BUFFER_SIZE];
    int posicionTablero;
    int turnosPerdidos;
    unsigned int color;
    bool enPozo;
};

struct CasillaStruct {
    int numero;
    TipoCasilla tipo;
    int siguienteOca;
    int posOtroPuente;
};

struct PartidaStruct {
    int cantidadJugadores;
    int sizeTablero;
    int cantidadDados;
    int turnoActual;
    bool partidaGuardada;
};

struct JuegoStruct {
    std::vector<JugadorStruct> jugadores;
    std::vector<CasillaStruct> casillas;
    PartidaStruct partida;
};

class GestorDeGuardado {
private:
    JuegoOca* m_juego = nullptr;
    const QString direccionCarpeta { QCoreApplication::applicationDirPath() };
    QString direccionArchivo;
    GestorDeGuardado(JuegoOca* juego) : m_juego(juego) {
        QDir directorio;
        if (directorio.mkpath(this->direccionCarpeta)){
            this->direccionArchivo = this->direccionCarpeta + "/partidaBin.dat";
        } else{
            qDebug() << "Carpeta con saves no creada";
        };
    }

public:
    GestorDeGuardado(const GestorDeGuardado&) = delete;
    GestorDeGuardado& operator=(const GestorDeGuardado&) = delete;

    void inicializar(JuegoOca* juego) {
        if (m_juego != juego) {
            m_juego = juego;
        }
    }
    bool guardarPartida();
    bool hayPartidaGuardada();
    std::optional<JuegoStruct> cargarPartida();
    static GestorDeGuardado& instancia(JuegoOca* juego = nullptr) {
        static GestorDeGuardado instance(juego);
        return instance;
    }
    bool eliminarPartidaGuardada();
};

#endif // GESTORDEGUARDADO_H
