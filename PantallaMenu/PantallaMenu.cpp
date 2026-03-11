#include "PantallaMenu.h"
#include "ui_PantallaMenu.h"
#include <QApplication>
#include "../ManejadorDeSonidos/manejadordesonidos.h"
#include <QMessageBox>
#include <QDebug>
#include "../GestorDeGuardado/GestorDeGuardado.h"

PantallaMenu::PantallaMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaMenu)
{
    ui->setupUi(this);
    connect(ui->btnJugar, &QPushButton::clicked, this, &PantallaMenu::btnJugarClickeado);
    connect(ui->btnOpciones, &QPushButton::clicked, this, &PantallaMenu::btnOpcionesClickeado);
    connect(ui->btnCreditos, &QPushButton::clicked, this, &PantallaMenu::btnCreditosClickeado);
    connect(ui->btnSalir, &QPushButton::clicked, this, &PantallaMenu::btnSalirClickeado);

    connect(ui->btnPantallaCompleta, &QPushButton::clicked, this, &PantallaMenu::btnPantallaCompletaClickeado);
    connect(ui->btnModoVentana, &QPushButton::clicked, this, &PantallaMenu::btnModoVentanaClickeado);

    connect(ui->musicaSlider, &QSlider::valueChanged, this, &PantallaMenu::musicaSliderCambio);
    connect(ui->sonidoSlider, &QSlider::valueChanged, this, &PantallaMenu::sonidosSliderCambio);

    connect(ui->btnSalir, &QPushButton::clicked, this, &PantallaMenu::btnSalirClickeado);
    connect(ui->btnVolver, &QPushButton::clicked, this, &PantallaMenu::btnVolverClickeado);

    connect(ui->btnCargarPartida, &QPushButton::clicked, this, [this]() {
        emit cargarPartida();
    });

    ManejadorDeSonidos::instancia().reproducirMusicaMenu();
}

void PantallaMenu::btnJugarClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit setPantallaJugar();
}

void PantallaMenu::btnOpcionesClickeado() {
     ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackMenus->setCurrentWidget(ui->opcionesSonido);
    ui->stackBtnOpciones->setCurrentWidget(ui->pageBtnVolver);
}

void PantallaMenu::btnCreditosClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackMenus->setCurrentWidget(ui->pageCreditos);
    ui->stackBtnOpciones->setCurrentWidget(ui->pageBtnVolver);
}

void PantallaMenu::btnVolverClickeado() {
     ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackMenus->setCurrentWidget(ui->principal);
    ui->stackBtnOpciones->setCurrentWidget(ui->pageBtnSalir);
}

void PantallaMenu::btnPantallaCompletaClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit setPantallaCompleta();
}

void PantallaMenu::setSliderMusicaValue() {
    float volumen = ManejadorDeSonidos::instancia().getVolumenMusica() * 100;
    ui->musicaSlider->setValue(volumen);
    ui->labelMusicaPorc->setText(QString::number(volumen) + "%");
}

void PantallaMenu::setSliderSonidoValue() {
    float volumen = ManejadorDeSonidos::instancia().getVolumenSonido() * 100.0;
    ui->sonidoSlider->setValue(volumen);
    ui->labelSonidoPorc->setText(QString::number(volumen) + "%");
}

void PantallaMenu::btnModoVentanaClickeado() {
     ManejadorDeSonidos::instancia().reproducirBoton();
    emit setModoVentana();
}

void PantallaMenu::musicaSliderCambio() {
    float volumen = ui->musicaSlider->value();
    ManejadorDeSonidos::instancia().setVolumenMusica(volumen);
    emit volumenMusicaCambiado();
}

void PantallaMenu::sonidosSliderCambio() {
    float volumen = ui->sonidoSlider->value();
    ui->sonidoSlider->blockSignals(true);
    ManejadorDeSonidos::instancia().setVolumenEfectos(volumen);
    ui->sonidoSlider->blockSignals(false);
    emit volumenSonidoCambiado();
}

void PantallaMenu::btnSalirClickeado() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Cerrar Juego");
    msgBox.setText("¿Estás seguro de que quieres salir del juego?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Warning);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void PantallaMenu::cambiarBtnPantalla(bool fullscreen) {
    if(fullscreen) {
        ui->stackBtnModo->setCurrentWidget(ui->modoVentana);
    } else {
        ui->stackBtnModo->setCurrentWidget(ui->modoPantallaCompleta);
    }
}

void PantallaMenu::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);    ui->btnCargarPartida->setEnabled(GestorDeGuardado::instancia().hayPartidaGuardada());
  }



PantallaMenu::~PantallaMenu()
{
    delete ui;
}
