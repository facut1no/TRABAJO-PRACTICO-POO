#include "Mainwindow.h"
#include "./ui_Mainwindow.h"
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QGraphicsOpacityEffect>
#include "ManejadorDeSonidos/manejadordesonidos.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/Imagenes/Recursos/Imagenes/Logo-oca.png"));
    setWindowTitle("Trabajo practico POO");
    crearPantallas();
    conexionesPantallaMenu();
    conexionesPantallaPartida();
    conexionesPantallaPausa();
    conexionesPantallaDeAjusteDePartida();
    conexionesPantallaFinal();
    m_PantallaMenu->setFocus();
    m_PantallaDeAjusteDePartida->setFocus();
    this->showFullScreen();
    m_PantallaMenu->setFocus();
    connect(m_Pantallas, &QStackedWidget::currentChanged, this, [this]() {
        QWidget *pantallaActual = m_Pantallas->currentWidget();
        pantallaActual->setFocus();
    });;
}

void MainWindow::crearPantallas() {
    m_Pantallas = new QStackedWidget(this);
    m_PantallaMenu = new PantallaMenu(this);
    m_PantallaPartida = new PantallaPartida(this);
    m_PantallaPausa = new PantallaPausa(this);
    m_PantallaDeAjusteDePartida = new PantallaAjusteDePartida(this);
    m_PantallaTransicion = new PantallaTransicion(this);
    m_PantallaFinal = new PantallaFinal(this);
    m_PantallaTransicion->setGeometry(this->rect());
    m_PantallaTransicion->raise();
    m_PantallaTransicion->hide();


    m_Pantallas->addWidget(m_PantallaMenu);
    m_Pantallas->addWidget(m_PantallaPausa);
    m_Pantallas->addWidget(m_PantallaDeAjusteDePartida);
    m_Pantallas->addWidget(m_PantallaPartida);
    m_Pantallas->addWidget(m_PantallaFinal);

    setCentralWidget(m_Pantallas);
}

void MainWindow::conexionesPantallaMenu() {
    connect(m_PantallaMenu, &PantallaMenu::setPantallaCompleta, this, &MainWindow::setPantallaCompleta);
    connect(m_PantallaMenu, &PantallaMenu::setModoVentana, this, &MainWindow::setModoVentana);
    connect(m_PantallaMenu, &PantallaMenu::setPantallaJugar, this, &MainWindow::setPantallaAjusteDePartida);
    connect(m_PantallaMenu, &PantallaMenu::volumenMusicaCambiado, this, &MainWindow::setVolumenSliders);
    connect(m_PantallaMenu, &PantallaMenu::volumenSonidoCambiado, this, &MainWindow::setSonidosSliders);
    connect(m_PantallaMenu, &PantallaMenu::cargarPartida, m_PantallaPartida, &PantallaPartida::cargarPartida);
}

void MainWindow::conexionesPantallaDeAjusteDePartida() {
    connect(m_PantallaDeAjusteDePartida, &PantallaAjusteDePartida::volverAlMenuInicial, this, &MainWindow::setMenuDeInicio);
    connect(m_PantallaDeAjusteDePartida, &PantallaAjusteDePartida::comenzarPartida, this, &MainWindow::setPantallaPartida);
}

void MainWindow::conexionesPantallaPausa() {
    connect(m_PantallaPausa, &PantallaPausa::volverAlInicio, this, [this]() {
        m_PantallaPartida->reiniciar();
        this->setMenuDeInicio();
    });
    connect(m_PantallaPausa, &PantallaPausa::renaudarPartida, this, &MainWindow::volverALaPartida);
    connect(m_PantallaPausa, &PantallaPausa::setVolumenMusica, this, &MainWindow::setVolumenSliders);
    connect(m_PantallaPausa, &PantallaPausa::setVolumenSonidos, this, &MainWindow::setSonidosSliders);
    connect(m_PantallaPausa, &PantallaPausa::setPantallaCompleta, this, &MainWindow::setPantallaCompleta);
    connect(m_PantallaPausa, &PantallaPausa::setModoVentana, this, &MainWindow::setModoVentana);
    connect(m_PantallaPausa, &PantallaPausa::salirDePausa, this, &MainWindow::volverALaPartida);
    connect(m_PantallaPausa, &PantallaPausa::guardarPartida, this, [this]() {
        m_PantallaPartida->guardarPartida();
    });

}


void MainWindow::conexionesPantallaPartida() {
    connect(m_PantallaPartida, &PantallaPartida::ponerEnPausa, this, &MainWindow::setPantallaPausa);
    connect(m_PantallaPartida, &PantallaPartida::juegoTerminado, this, &MainWindow::setPantallaFinal);
    connect(m_PantallaPartida, &PantallaPartida::partidaGuardada, this, [this]() {
        m_PantallaTransicion->mostrarTransicion(1500, true, [this]() {
            m_PantallaPartida->reiniciar();
            m_Pantallas->setCurrentWidget(m_PantallaMenu);
        }, "Guardando...");
    });
    connect(m_PantallaPartida, &PantallaPartida::partidaCargada, this, &MainWindow::setPantallaPartidaCargada);
}


void MainWindow::conexionesPantallaFinal() {
    connect(m_PantallaFinal, &PantallaFinal::volverAlMenu, this, &MainWindow::setMenuDeInicio);
    connect(m_PantallaFinal, &PantallaFinal::volverAJugar, this, &MainWindow::volverAJugar);
}

void MainWindow::volverAJugar() {
    m_PantallaPartida->volverAJugar();
    m_PantallaTransicion->mostrarTransicion(1600, true, [this] {
        m_Pantallas->setCurrentWidget(m_PantallaPartida);
    });
}

void MainWindow::setPantallaPausa() {
    ManejadorDeSonidos::instancia().pausarMusicaParida();
    m_PantallaTransicion->mostrarTransicion(300, false, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaPausa);
    });
}

void MainWindow::setPantallaFinal(const QList<Jugador*>& jugadores) {
    qDebug() << jugadores.size();
    m_PantallaFinal->cargarResultados(jugadores);
    m_PantallaTransicion->mostrarTransicion(1000, true, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaFinal);
    }, "Fin de la partida");
}

void MainWindow::volverALaPartida() {
    ManejadorDeSonidos::instancia().reproducirMusicaPartida();
    m_PantallaTransicion->mostrarTransicion(500, false, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaPartida);
    });
}

void MainWindow::setPantallaPartida(const QList<QPair<QString, QColor>>& jugadores, int cantidadCasillas, int cantidadDados) {
    ManejadorDeSonidos::instancia().reproducirMusicaPartida();
    m_PantallaPartida->cargarUI(cantidadCasillas, cantidadDados);
    m_PantallaPartida->cargarJugadores(jugadores);
    m_PantallaTransicion->mostrarTransicion(1200, true, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaPartida);
    });
}

void MainWindow::setPantallaPartidaCargada() {
    ManejadorDeSonidos::instancia().reproducirMusicaPartida();
    m_PantallaTransicion->mostrarTransicion(1200, true, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaPartida);
    });
}

void MainWindow::setMenuDeInicio() {
    m_PantallaTransicion->mostrarTransicion(400, false, [=, this] {
        if(m_Pantallas->currentWidget() == m_PantallaFinal) {
            m_PantallaDeAjusteDePartida->resetear();
        }
        m_Pantallas->setCurrentWidget(m_PantallaMenu);
        m_PantallaPartida->reiniciar();
    });
}

void MainWindow::calcularCentroPantalla()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    this->centroPantallaX = (screenGeometry.width() - this->width()) / 2;
    this->centroPantallaY = (screenGeometry.height() - this->height()) / 2;
}


void MainWindow::setPantallaCompleta() {
    this->showFullScreen();
    m_PantallaPausa->cambiarBtnPantalla(true);
    m_PantallaMenu->cambiarBtnPantalla(true);
}

void MainWindow::setModoVentana() { 
    this->showNormal();
    this->resize(1280, 720);
    calcularCentroPantalla();
    this->move(this->centroPantallaX, this->centroPantallaY);
    m_PantallaPausa->cambiarBtnPantalla(false);
    m_PantallaMenu->cambiarBtnPantalla(false);
}


void MainWindow::setVolumenSliders() {
    m_PantallaPausa->setSliderMusicaValue();
    m_PantallaMenu->setSliderMusicaValue();
}

void MainWindow::setSonidosSliders() {
    m_PantallaPausa->setSliderSonidoValue();
    m_PantallaMenu->setSliderSonidoValue();
}

void MainWindow::setPantallaAjusteDePartida() {
    m_PantallaTransicion->mostrarTransicion(400, false, [=, this] {
        m_Pantallas->setCurrentWidget(m_PantallaDeAjusteDePartida);
    });
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    m_PantallaTransicion->resize(this->size());
}
MainWindow::~MainWindow()
{
    delete ui;
}
