#include "jugadorui.h"
#include "ui_jugadorui.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "../ManejadorDeSonidos/manejadordesonidos.h"
JugadorUI::JugadorUI(QString nombre, QColor color, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JugadorUI)
    , color(color)
    , nombre(nombre)
{
    ui->setupUi(this);
    ui->fondo->setAutoFillBackground(true);
    ui->fondo->setStyleSheet(QString("background-color: %1").arg(this->color.name()));
    this->setMinimumSize(10, 10);
    this->setMaximumSize(15, 15);
    this->setContentsMargins(0,0,0,0);
}

void JugadorUI::mover(CasillaUI* casillaActual, CasillaUI* casillaNueva, TableroUI* tablero) {
    casillaActual->remover(this);

    QPoint posGlobalInicio = casillaActual->mapToGlobal(casillaActual->rect().center() - this->rect().center());
    QPoint posGlobalFin = casillaNueva->mapToGlobal(casillaNueva->rect().center() - this->rect().center());

    QPoint posInicioLocal = tablero->mapFromGlobal(posGlobalInicio);
    QPoint posFinLocal = tablero->mapFromGlobal(posGlobalFin);

    this->setParent(tablero);
    this->move(posInicioLocal);
    this->show();
    this->raise();

    QPoint puntoIntermedio;
    if (qAbs(posFinLocal.x() - posInicioLocal.x()) >= qAbs(posFinLocal.y() - posInicioLocal.y())) {
        puntoIntermedio = QPoint(posFinLocal.x(), posInicioLocal.y());
    } else {
        puntoIntermedio = QPoint(posInicioLocal.x(), posFinLocal.y());
    }

    QSequentialAnimationGroup* grupo = new QSequentialAnimationGroup(this);

    QPropertyAnimation* anim1 = new QPropertyAnimation(this, "pos");
    anim1->setDuration(1000);
    anim1->setStartValue(posInicioLocal);
    anim1->setEndValue(puntoIntermedio);

    QPropertyAnimation* anim2 = new QPropertyAnimation(this, "pos");
    anim2->setDuration(1000);
    anim2->setStartValue(puntoIntermedio);
    anim2->setEndValue(posFinLocal);

    grupo->addAnimation(anim1);
    grupo->addAnimation(anim2);


    connect(grupo, &QSequentialAnimationGroup::finished, this, [this, casillaNueva, grupo]() {
        this->setParent(casillaNueva);
        this->move(0, 0);
        this->show();

        casillaNueva->insertar(this);

        emit animacionMovimientoTerminada();
        grupo->deleteLater();
        currentAnimation = nullptr;
    });

    grupo->start();
}

void JugadorUI::moverPorRuta(const QList<CasillaUI*>& ruta, QWidget* tablero) {
    int tiempoAnimacion = 400;
    if(ruta.size() > 14) {
        tiempoAnimacion = 200;
    }
    CasillaUI* casillaActual = ruta.first();
    CasillaUI* casillaNueva = ruta.last();
    casillaActual->remover(this);

    this->setParent(tablero);
    this->show();
    this->raise();

    QSequentialAnimationGroup* grupoAnimaciones = new QSequentialAnimationGroup(this);


    for(int i = 0; i < ruta.size() - 1; i++) {
        QPoint posInicialCasilla = ruta[i]->mapToGlobal(ruta[i]->rect().center() - this->rect().center());
        QPoint posFinalCasilla = ruta[i + 1]->mapToGlobal(ruta[i + 1]->rect().center() - this->rect().center());
        QPoint posInicialTablero = tablero->mapFromGlobal(posInicialCasilla);
        this->move(posInicialTablero);
        QPoint posFinalTablero = tablero->mapFromGlobal(posFinalCasilla);
        QPropertyAnimation* animacion = new QPropertyAnimation(this, "pos");
        animacion->setDuration(tiempoAnimacion);
        animacion->setStartValue(posInicialTablero);
        animacion->setEndValue(posFinalTablero);
        grupoAnimaciones->addAnimation(animacion);
        connect(animacion, &QPropertyAnimation::stateChanged, this, [](QAbstractAnimation::State newState, QAbstractAnimation::State){
            if (newState == QAbstractAnimation::Running) {
                ManejadorDeSonidos::instancia().reproducirSonidoFicha();
            }
        });
    }

    connect(grupoAnimaciones, &QSequentialAnimationGroup::finished, this, [this, casillaNueva, grupoAnimaciones]() {
        this->setParent(casillaNueva);
        this->move(0, 0);
        this->show();
        casillaNueva->insertar(this);
        emit animacionMovimientoTerminada();
        grupoAnimaciones->deleteLater();
        currentAnimation = nullptr;
    });
    grupoAnimaciones->start();
}


JugadorUI::~JugadorUI()
{
    if (currentAnimation && currentAnimation->state() == QAbstractAnimation::Running) {
        currentAnimation->stop();
    }
    delete ui;
}
