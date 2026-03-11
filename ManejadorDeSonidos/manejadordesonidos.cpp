#include "manejadordesonidos.h"


ManejadorDeSonidos* ManejadorDeSonidos::m_instancia = nullptr;

ManejadorDeSonidos::ManejadorDeSonidos(QObject* parent) : QObject(parent) {
    // Por probleas de peso del archivo no se puede usar.
    // musicaMenu = new QSoundEffect(this);
    // musicaMenu->setSource(QUrl("qrc:/Sonidos/Recursos/Sonidos/AlexBouncyMaster.wav"));
    // musicaMenu->setVolume(1.0);

    sonidoBoton = new QSoundEffect(this);
    sonidoBoton->setSource(QUrl("qrc:/Sonidos/Recursos/Sonidos/BotonesMenu.WAV"));
    sonidoBoton->setVolume(1.0);

    sonidoDeDado = new QSoundEffect(this);
    sonidoDeDado->setSource(QUrl("qrc:/Sonidos/Recursos/Sonidos/sonidoDado.wav"));
    sonidoDeDado->setVolume(1.0);

    musicaPartida = new QSoundEffect(this);
    musicaPartida->setSource(QUrl("qrc:/Sonidos/Recursos/Sonidos/musicaPartida.wav"));
    musicaPartida->setVolume(1.0);

    sonidoFicha = new QSoundEffect(this);
    sonidoFicha->setSource(QUrl("qrc:/Sonidos/Recursos/Sonidos/sonidoFichas.wav"));
    sonidoFicha->setVolume(1.0);
}

void ManejadorDeSonidos::reproducirBoton() {
    this->sonidoBoton->play();
}

void ManejadorDeSonidos::reproducirMusicaMenu() {
    this->musicaPartida->play();
    this->musicaPartida->setLoopCount(QSoundEffect::Infinite);
}

void ManejadorDeSonidos::reproducirSonidoDado() const {
    this->sonidoDeDado->play();
}

void ManejadorDeSonidos::pausarMusicaMenu() {
    this->musicaPartida->stop();
}

float ManejadorDeSonidos::normalizarVolumen(float volumen) {
    return  volumen / 100.0;
}

void ManejadorDeSonidos::reproducirMusicaPartida() const {
    this->musicaPartida->play();
    this->musicaPartida->setLoopCount(QSoundEffect::Infinite);
}

void ManejadorDeSonidos::pausarMusicaParida() const {
    this->musicaPartida->stop();
}

void ManejadorDeSonidos::reproducirSonidoFicha() const {
    this->sonidoFicha->play();
}

void ManejadorDeSonidos::setVolumenMusica(float volumen) {
    this->volumenMusica = normalizarVolumen(volumen);
    this->musicaPartida->setVolume(this->volumenMusica);
    this->musicaPartida->setVolume(this->volumenMusica);
}

void ManejadorDeSonidos::setVolumenEfectos(float volumen) {
    this->volumenEfectos = normalizarVolumen(volumen);
    this->sonidoBoton->setVolume(this->volumenEfectos);
    this->sonidoDeDado->setVolume(this->volumenEfectos);
    this->sonidoFicha->setVolume(this->volumenEfectos);
}

float ManejadorDeSonidos::getVolumenMusica() const {
    return this->volumenMusica;
}

float ManejadorDeSonidos::getVolumenSonido() const {
    return this->volumenEfectos;
}
