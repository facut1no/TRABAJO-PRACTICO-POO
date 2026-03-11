#include "pantallapausa.h"
#include "ui_pantallapausa.h"
#include <QKeyEvent>

#include "../ManejadorDeSonidos/manejadordesonidos.h"
#include "../GestorDeGuardado/GestorDeGuardado.h"

PantallaPausa::PantallaPausa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaPausa)
{
    ui->setupUi(this);
    connect(ui->btnConfiguracion, &QPushButton::clicked, this, &PantallaPausa::btnConfiguracionClickeado);
    connect(ui->btnRenaudar, &QPushButton::clicked, this, &PantallaPausa::btnRenaudarClickeado);
    connect(ui->btnVolverAlInicio, &QPushButton::clicked, this, &PantallaPausa::btnVolverAlInicioClickeado);
    connect(ui->btnVolver, &QPushButton::clicked, this, &PantallaPausa::btnVolverClickeado);

    connect(ui->btnVolverPG, &QPushButton::clicked, this, &PantallaPausa::btnVolverClickeado);
    connect(ui->sliderMusica, &QSlider::valueChanged, this, &PantallaPausa::sliderMusicaCambio);
    connect(ui->sliderSonidos, &QSlider::valueChanged, this, &PantallaPausa::sliderSonidoCambio);

    connect(ui->btnPantallaCompleta, &QPushButton::clicked, this, &PantallaPausa::btnPantallaCompletaClickeado);
    connect(ui->btnModoVentana, &QPushButton::clicked, this, &PantallaPausa::btnModoVentanaClickeado);
    connect(ui->btnSalirSinGuardar, &QPushButton::clicked, this, [this]() {
        emit volverAlInicio();
    });
    connect(ui->btnGuardarPartida, &QPushButton::clicked, this, [this]() {
        emit guardarPartida();
    });

    ui->pagePausa->setFocus();
    setSliderMusicaValue();
    setSliderSonidoValue();
}

void PantallaPausa::btnVolverAlInicioClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackPaginas->setCurrentWidget(ui->pageGuardado);
}

void PantallaPausa::btnConfiguracionClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackPaginas->setCurrentWidget(ui->pageConfiguracion);
}

void PantallaPausa::btnRenaudarClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit renaudarPartida();
}

void PantallaPausa::btnVolverClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackPaginas->setCurrentWidget(ui->pagePausa);
}

void PantallaPausa::sliderMusicaCambio() {
    ManejadorDeSonidos::instancia().setVolumenMusica(ui->sliderMusica->value());
    emit setVolumenMusica();
}

void PantallaPausa::sliderSonidoCambio() {
    ManejadorDeSonidos::instancia().setVolumenEfectos(ui->sliderSonidos->value());
    emit setVolumenSonidos();
}

void PantallaPausa::setSliderMusicaValue() {
    float volumen = ManejadorDeSonidos::instancia().getVolumenMusica() * 100;
    ui->sliderMusica->setValue(volumen);
    ui->labelMusicaPorc->setText(QString::number(volumen) + "%");
}

void PantallaPausa::setSliderSonidoValue() {
    float volumen = ManejadorDeSonidos::instancia().getVolumenSonido() * 100.0;
    qDebug() << volumen << '\n';
    ui->sliderSonidos->setValue(volumen);
    ui->labelSonidosPorc->setText(QString::number(volumen) + "%");
}


void PantallaPausa::btnPantallaCompletaClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit setPantallaCompleta();
}

void PantallaPausa::btnModoVentanaClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit setModoVentana();
}

void PantallaPausa::cambiarBtnPantalla(bool fullscreen) {
    if(fullscreen) {
        ui->stackBtns->setCurrentWidget(ui->pageModoVentana);
    } else {
        ui->stackBtns->setCurrentWidget(ui->pagePantallaCompleta);
    }
}
void PantallaPausa::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    ui->stackPaginas->setCurrentWidget(ui->pagePausa);
}

void PantallaPausa::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        emit salirDePausa();
    } else {
        QWidget::keyPressEvent(event);
    }
}

PantallaPausa::~PantallaPausa()
{
    delete ui;
}
