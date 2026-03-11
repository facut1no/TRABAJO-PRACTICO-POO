#ifndef MANEJADORDESONIDOS_H
#define MANEJADORDESONIDOS_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMap>

class ManejadorDeSonidos : QObject
{
    Q_OBJECT
    public:
        static ManejadorDeSonidos& instancia() {
            static ManejadorDeSonidos instanciaUnica;
            return instanciaUnica;
        }
        void reproducirBoton();
        void reproducirMusicaMenu();
        void pausarMusicaMenu();
        void setVolumenEfectos(float volumen);
        void setVolumenMusica(float volumen);
        float getVolumenMusica() const;
        float getVolumenSonido() const;
        void reproducirSonidoDado() const;
        void reproducirMusicaPartida() const;
        void pausarMusicaParida() const;
        void reproducirSonidoFicha() const;

    private:
        explicit ManejadorDeSonidos(QObject *parent = nullptr);
        static ManejadorDeSonidos* m_instancia;
        // QSoundEffect* musicaMenu;
        QSoundEffect* sonidoBoton;
        QSoundEffect* musicaPartida;
        QSoundEffect* sonidoDeDado;
        QSoundEffect* sonidoFicha;
        float volumenEfectos = 1.0;
        float volumenMusica = 1.0;
        float normalizarVolumen(float volumen);
};
#endif // MANEJADORDESONIDOS_H
