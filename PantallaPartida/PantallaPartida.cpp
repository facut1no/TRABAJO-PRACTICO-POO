#include "PantallaPartida.h"
#include "ui_PantallaPartida.h"
#include "../TableroUI/tableroui.h"
#include "../ManejadorDeSonidos/manejadordesonidos.h"
#include <qmessagebox.h>
#include <qstyle.h>
#include <qvalidator.h>
#include "../GestorDeGuardado/GestorDeGuardado.h"
#include "../logica/Casillas/casillanormal.h"
#include "../logica/Casillas/casillacalavera.h"//.h"
#include "../logica/Casillas/casillacarcel.h"//.h"
#include "../logica/Casillas/casilladeoca.h"//.h"
#include "../logica/Casillas/casillaposada.h"//.h"
#include "../logica/Casillas/casillajardindeoca.h"//.h"
#include "../logica/Casillas/casillalaberinto.h"//.h"
#include "../logica/Casillas/casillapuente.h"//.h"
#include "../logica/Casillas/casillapozo.h"//.h"

PantallaPartida::PantallaPartida(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaPartida)
{
    ui->setupUi(this);
    this->tableroui = new TableroUI(this);
    ui->tablero->addWidget(this->tableroui);
    this->setFocusPolicy(Qt::StrongFocus);

    this->notificacion = new Notificacion(this->tableroui);

    connect(ui->btnTirarDado, &QPushButton::clicked, this, &PantallaPartida::btnTirarDadoClickeado);
    this->timerAnimacionDado = new QTimer(this);
    connect(timerAnimacionDado, &QTimer::timeout, this, &PantallaPartida::actualizarFrameDado);
    connect(this, &PantallaPartida::animacionDadoTerminada, this, &PantallaPartida::jugarTurno);
    connect(this, &PantallaPartida::turnoTerminado, this, &PantallaPartida::cambiarTurno);
}

void PantallaPartida::cargarUI(int cantCasillas, int cantDados) {
    ui->btnTirarDado->setEnabled(true);
    if(!this->juego) {
       this->juego = new JuegoOca(cantCasillas, cantDados);
    }
    this->cantDados = cantDados;
    if(this->dadosUI.empty()) {
        for (int i = 1; i <= this->cantDados; ++i) {
            QLabel* dadoLabel = new QLabel(this);
            dadoLabel->setMinimumSize(80, 80);
            dadoLabel->setMaximumSize(100, 100);
            dadoLabel->setAlignment(Qt::AlignCenter);
            dadoLabel->setPixmap(QPixmap(":/Imagenes/Recursos/Imagenes/dado1.png")
                                     .scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            dadoLabel->setScaledContents(false);
            ui->layoutDados->addWidget(dadoLabel);
            this->dadosUI.push_back(dadoLabel);
        }

    }
    this->tableroui->cargarVectorCasillas(this->juego->obtenerTablero()->obtenerCasillas());
    this->tableroui->cargarCasillasAlTablero();
    emit casillasCargadas();
}

void PantallaPartida::jugarTurno() {
    this->juego->moverJugador();
    Jugador* jugador = this->juego->obtenerJugadorActual();
    int inicio = jugador->getPosicionAnterior();
    int fin = jugador->getPosicion();
    this->animarMovimientoJugador(inicio, fin, true);
    if(!this->jugadorUIActual) return;
    connect(this->jugadorUIActual, &JugadorUI::animacionMovimientoTerminada, this, [this, jugador] {
        this->aplicarEfectoCasilla(jugador);
    }, Qt::SingleShotConnection);
}

void PantallaPartida::aplicarEfectoCasilla(Jugador* jugador) {
    if (!jugador->getEfectoAplicado()) {
        this->aplicarEfectoInicialDeCasilla(jugador);
        return;
    }

    if (this->debeMoversePorEfecto(jugador)) {
        this->animarMovimientoPostEfecto(jugador);
        return;
    }
    this->finalizarTurno(jugador);
}


void PantallaPartida::aplicarEfectoInicialDeCasilla(Jugador* jugador) {
    this->juego->aplicarEfectoCasilla();
    jugador->setEfectoAplicado(true);

    CasillaUI* casillaui = this->tableroui->obtenerCasillaUI(jugador->getPosicionAnterior() + 1);
    this->mostrarNotificacion(
        casillaui->obtenerRutaImagen(),
        casillaui->obtenerMensaje(),
        [this, jugador]() { this->aplicarEfectoCasilla(jugador); }
    );
}

bool PantallaPartida::debeMoversePorEfecto(Jugador* jugador) {
    return jugador->getPosicion() != jugador->getPosicionAnterior();
}

void PantallaPartida::animarMovimientoPostEfecto(Jugador* jugador) {
    int posAntes = jugador->getPosicionAnterior();
    jugador->setPosicionAnterior();
    int posDespues = jugador->getPosicion();
    this->animarMovimientoJugador(posAntes, posDespues, false);

    connect(this->jugadorUIActual, &JugadorUI::animacionMovimientoTerminada, this, [this, jugador] {
        this->aplicarEfectoCasilla(jugador);
    }, Qt::SingleShotConnection);

}

void PantallaPartida::finalizarTurno(Jugador* jugador) {
    jugador->setEfectoAplicado(false);
    if (this->juego->hayGanador()) {
        if(this->juego->partidaGuardada)
            GestorDeGuardado::instancia().eliminarPartidaGuardada();
        emit juegoTerminado(this->juego->obtenerJugadoresOrdenados());
    } else {
        emit turnoTerminado();
    }
}

void PantallaPartida::cambiarTurno() {
    this->juego->terminarTurno();
    this->jugadorUIActual = mapaJugadorToUI.value(this->juego->obtenerJugadorActual());
    setTextLabelTurno();
    this->enlistarJugadores();
    if (!this->juego->jugadorPuedeJugar()) {
        this->juego->restarTurnoJugador();
        notificacion->mostrar(":/Imagenes/Recursos/Imagenes/cancelado.png", "¡Perdiste turno! Ahora espera a poder jugar.", [this] {
            QTimer::singleShot(300, this, &PantallaPartida::cambiarTurno);
        });
        return;
    }
    ui->btnTirarDado->setEnabled(true);
}


void PantallaPartida::cargarJugadores(const QList<QPair<QString, QColor>>& jugadoresDatos) {
    juego->cargarDatosJugadores(jugadoresDatos);
    QVector<Jugador*> jugadoresLogicos = juego->obtenerJugadores();
    if(!this->jugadoresUI.empty() || !this->mapaJugadorToUI.empty()) {
        this->jugadoresUI.clear();
        this->mapaJugadorToUI.clear();
    }
    int fila = 0, columna = 0;
    CasillaUI* casillaInicialUI = this->tableroui->obtenerCasillaUI(0);
    for(Jugador* jugadorLogico : std::as_const(jugadoresLogicos)) {
        JugadorUI* jugadorui = new JugadorUI(jugadorLogico->getNombre(), jugadorLogico->getColor(), casillaInicialUI);
        if (casillaInicialUI) {
            casillaInicialUI->insertar(jugadorui);
            if(columna == 2) {
                columna = 0;
                fila++;
            }
        }
        jugadoresUI.push_back(jugadorui);
        mapaJugadorToUI[jugadorLogico] = jugadorui;
    }
    this->enlistarJugadores();
    this->jugadorUIActual = mapaJugadorToUI.value(this->juego->obtenerJugadorActual());
    setTextLabelTurno();
}


void PantallaPartida::setTextLabelTurno() {
    Jugador* jugador = this->juego->obtenerJugadorActual();
    ui->labelTurno->setText(QString("Turno de %1").arg(jugador->getNombre()));
    ui->colorJugador->setStyleSheet(QString("background-color: %1;").arg(jugador->getColor().name()));
}

void PantallaPartida::animarMovimientoJugador(int posActual, int posNueva, bool porDado) {
       auto ruta = this->obtenerRutaDeMovimiento(posActual + 1, posNueva + 1, porDado);
    jugadorUIActual->moverPorRuta(ruta, this->tableroui);
}

QList<CasillaUI*> PantallaPartida::obtenerRutaDeMovimiento(int posActual, int posNueva, bool porDado) {
    int totalCasillasUI = this->juego->obtenerTablero()->obtenerCasillas().size();
    QList<CasillaUI*> ruta;

    if (porDado) {
        if (posActual < posNueva && posActual + this->juego->obtenerValorDado() <= this->juego->getCantCasillas()) {
            for (int i = posActual; i <= posNueva; ++i) {
                ruta.append(this->tableroui->obtenerCasillaUI(i));
            }
        } else {
            int pasosHastaFinal = totalCasillasUI - posActual;
            int pasosRestantes = this->juego->obtenerValorDado() - pasosHastaFinal;

            for (int i = posActual; i <= totalCasillasUI; ++i) {
                ruta.append(this->tableroui->obtenerCasillaUI(i));
            }

            for (int i = totalCasillasUI - 1; i >= totalCasillasUI - pasosRestantes; --i) {
                ruta.append(this->tableroui->obtenerCasillaUI(i));
            }
        }
    } else {
        int pasos = (posNueva > posActual) ? 1 : -1;
        for (int i = posActual; i != posNueva + pasos; i += pasos) {
            ruta.append(this->tableroui->obtenerCasillaUI(i));
        }
    }
    return ruta;
}

void PantallaPartida::moverJugador(Jugador* jugador) {
    if(jugador->puedeJugar()) {
        int inicio = jugador->getPosicion();
        this->juego->moverJugador();
        int fin = jugador->getPosicion();
        this->animarMovimientoJugador(inicio, fin, true);
    } else {
        emit turnoTerminado();
    }
}

void PantallaPartida::iniciarAnimacionDado(int resultadoFinal) {
    ManejadorDeSonidos::instancia().reproducirSonidoDado();
    dadoAnimacionFrame = 0;
    this->dadoValorFinal = resultadoFinal;
    if(this->juego->obtenerJugadorActual()->getPosicion() >= 60) {
        for (int i = 0; i < this->dadosUI.size(); ++i) {
            this->dadosUI[i]->setVisible(i == 0);
        }
        this->juego->setCantidadDados(1);
    } else {
        for (QLabel* dado : this->dadosUI) {
            dado->setVisible(true);
        }
        this->juego->setCantidadDados(this->cantDados);
    }
    timerAnimacionDado->setInterval(80);
    timerAnimacionDado->start();
    QTimer::singleShot(1200, this, &PantallaPartida::finalizarAnimacionDado);
}

void PantallaPartida::actualizarFrameDado() {
    dadoAnimacionFrame++;
    for(QLabel* dado : this->dadosUI) {
        int randomNumber = QRandomGenerator::global()->bounded(1, 7);
        QString imagenPath = QString(":/Imagenes/Recursos/Imagenes/dado%1.png").arg(randomNumber);
        QPixmap pixmap(imagenPath);
        dado->setPixmap(pixmap.scaled(dado->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        dado->setScaledContents(true);
    }
    }


void PantallaPartida::finalizarAnimacionDado() {
    timerAnimacionDado->stop();

    for (int i = 0; i < this->dadosUI.size(); ++i) {
        int valor = this->juego->obtenerValoresDados()[i];
        QString imagenPath = QString(":/Imagenes/Recursos/Imagenes/dado%1.png").arg(valor);
        QPixmap pixmap(imagenPath);
        this->dadosUI[i]->setPixmap(pixmap.scaled(this->dadosUI[i]->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this->dadosUI[i]->setScaledContents(true);
    }

    emit animacionDadoTerminada(true);
}

void PantallaPartida::btnTirarDadoClickeado() {
    ui->btnTirarDado->setEnabled(false);
    ManejadorDeSonidos::instancia().reproducirBoton();
    this->juego->tirarDado();
    this->iniciarAnimacionDado(this->juego->obtenerValorDado());
}

void PantallaPartida::enlistarJugadores() {
    QVector<Jugador*> jugadoresOrdenados = juego->obtenerJugadoresOrdenados();

    QLayoutItem* item;
    while ((item = ui->listaJugadores->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    for (const auto& jugador : std::as_const(jugadoresOrdenados)) {
        QLabel* label = new QLabel(this);
        label->setText(jugador->getNombre());
        if(jugador == this->juego->obtenerJugadorActual())
            label->setStyleSheet(QString("background-color: #121212; padding: 5px; color: white; border: 3px solid %1; font-weight: bold;").arg(jugador->getColor().name()));
        else
            label->setStyleSheet(QString("background-color: #121212; padding: 5px; color: white;font-weight: bold;"));
        label->setMaximumHeight(35);
        ui->listaJugadores->addWidget(label);
    }
}

void PantallaPartida::mostrarNotificacion(QString imagenPath, QString mensaje, std::function<void()> funcion) {
    this->notificacion->mostrar(imagenPath, mensaje, funcion);
}

void PantallaPartida::reiniciar() {
    if (timerAnimacionDado && timerAnimacionDado->isActive())
        timerAnimacionDado->stop();
    disconnect(timerAnimacionDado);

    qDeleteAll(jugadoresUI);
    jugadoresUI.clear();
    mapaJugadorToUI.clear();
    jugadorUIActual = nullptr;

    if(tableroui)
        tableroui->reiniciar();

    for(auto* dado : dadosUI) {
        ui->layoutDados->removeWidget(dado);
        delete dado;
    }
    dadosUI.clear();

    ui->labelTurno->clear();
    ui->colorJugador->setStyleSheet("");
    ui->btnTirarDado->setEnabled(false);

    QLayoutItem* item;
    while((item = ui->listaJugadores->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
        GestorDeGuardado::instancia().inicializar(nullptr);
    if(juego) {
        delete juego;
        juego = nullptr;
    }
    ui->btnTirarDado->setEnabled(true);
}

void PantallaPartida::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        emit ponerEnPausa();
    }else if (event->key() == Qt::Key_Space) {
        if(ui->btnTirarDado->isEnabled()) {
            btnTirarDadoClickeado();
        }
    } else {
        QWidget::keyPressEvent(event);
    }
}

void PantallaPartida::guardarPartida() {
    int ret;
    if(!this->juego->partidaGuardada && GestorDeGuardado::instancia().hayPartidaGuardada()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Sobreescribir partida");
        msgBox.setText("Ya hay una partida guardada, desea sobreescribirla?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Warning);

        ret = msgBox.exec();
        if (ret == QMessageBox::No) {
            emit ponerEnPausa();
            return;
        }
    }
    this->juego->partidaGuardada = true;
    GestorDeGuardado::instancia().inicializar(nullptr);
    GestorDeGuardado::instancia().inicializar(this->juego);
    if (!GestorDeGuardado::instancia().guardarPartida()) {
        qDebug() << "Error al guardar la partida.";
        return;
    } else {
        qDebug() << "Partida guardada correctamente";
    }
    emit partidaGuardada();
}

void PantallaPartida::cargarPartida() {
    std::optional<JuegoStruct> juegoGuardado = GestorDeGuardado::instancia().cargarPartida();
    if(!juegoGuardado.has_value()) {
        qDebug() << "No hay partida guardada";
        return;
    }
    GestorDeGuardado::instancia().inicializar(nullptr);
    if (this->juego) {
        delete this->juego;
        this->juego = nullptr;
    }

    this->juego = new JuegoOca(juegoGuardado->partida.sizeTablero, juegoGuardado->partida.cantidadDados);
    this->juego->partidaGuardada = juegoGuardado->partida.partidaGuardada;
    if(this->juego->partidaGuardada) qDebug() << "Partida guardada anteriormente";
    QList<QPair<QString, QColor>> jugadoresDatos;
    QList<QPair<int, int>> jugadoresDatosTablero;
    for(const auto& j : juegoGuardado->jugadores) {
        jugadoresDatos.push_back({j.nombre, j.color});
        qDebug() << j.nombre << ": " << j.posicionTablero;
        jugadoresDatosTablero.push_back({j.posicionTablero, j.turnosPerdidos});
    }

    this->cargarJugadores(jugadoresDatos);
    this->juego->setTurno(juegoGuardado->partida.turnoActual);
    auto jugadores = this->juego->obtenerJugadores();
    for (size_t i = 0; i < std::min(jugadores.size(), jugadoresDatosTablero.size()); ++i) {
        jugadores[i]->setPosicion(jugadoresDatosTablero[i].first);
        jugadores[i]->setPosicionAnterior();
        jugadores[i]->setTurnosPerdidos(jugadoresDatosTablero[i].second);
        jugadores[i]->setEnPozo(juegoGuardado->jugadores[i].enPozo);
    }

    std::vector<Casilla*> casillas;
    for(const auto& c : juegoGuardado->casillas) {
        Casilla* casilla = nullptr;
        switch (c.tipo) {
        case TipoCasilla::Normal:      casilla = new CasillaNormal(c.numero); break;
        case TipoCasilla::Oca:         casilla = new CasillaDeOca(c.numero); break;
        case TipoCasilla::Puente:      casilla = new CasillaPuente(c.numero); break;
        case TipoCasilla::Pozo:        casilla = new CasillaPozo(c.numero); break;
        case TipoCasilla::Carcel:      casilla = new CasillaCarcel(c.numero); break;
        case TipoCasilla::Laberinto:   casilla = new CasillaLaberinto(c.numero); break;
        case TipoCasilla::Calavera:    casilla = new CasillaCalavera(c.numero); break;
        case TipoCasilla::Posada:      casilla = new CasillaPosada(c.numero); break;
        case TipoCasilla::JardinDeOca: casilla = new CasillaJardinDeOca(c.numero); break;
        }
        if(CasillaDeOca* oca = dynamic_cast<CasillaDeOca*>(casilla)) {
            oca->setSiguienteOca(c.siguienteOca);
            casillas.push_back(oca);
        }
        else if(CasillaPuente* puente = dynamic_cast<CasillaPuente*>(casilla)) {
            puente->setPosOtroPuente(c.posOtroPuente);
            casillas.push_back(puente);
        } else {
            casillas.push_back(casilla);
        }
        }
    this->juego->obtenerTablero()->setCasillas(std::move(casillas));
    this->cargarUI(juegoGuardado->partida.sizeTablero, juegoGuardado->partida.cantidadDados);
    this->cargarJugadoresGuardados();
    ui->btnTirarDado->setEnabled(true);
    this->juego->actualizarEstadoJuegoCargado();
    emit partidaCargada();
}

void PantallaPartida::cargarJugadoresGuardados() {
    for(const auto& jugador : this->juego->obtenerJugadores()) {
        CasillaUI* casillaActual = this->tableroui->obtenerCasillaUI(jugador->getPosicion() + 1);
        if (!casillaActual) continue;

        JugadorUI* jugadorUI = new JugadorUI(jugador->getNombre(), jugador->getColor(), casillaActual);
        casillaActual->insertar(jugadorUI);

        this->jugadoresUI.push_back(jugadorUI);
        this->mapaJugadorToUI[jugador] = jugadorUI;
    }
    this->enlistarJugadores();
    this->jugadorUIActual = mapaJugadorToUI.value(this->juego->obtenerJugadorActual());
    setTextLabelTurno();
}


void PantallaPartida::volverAJugar() {
    this->juego->volverAJugar();
    this->jugadorUIActual = nullptr;
    if(!this->jugadoresUI.empty() || !this->mapaJugadorToUI.empty()) {
        qDeleteAll(this->jugadoresUI);
        this->jugadoresUI.clear();
        this->mapaJugadorToUI.clear();
    }
    auto jugadoresLogicos = juego->obtenerJugadores();
    int fila = 0, columna = 0;
    CasillaUI* casillaInicialUI = this->tableroui->obtenerCasillaUI(0);
    for(Jugador* jugadorLogico : std::as_const(jugadoresLogicos)) {
        JugadorUI* jugadorui = new JugadorUI(jugadorLogico->getNombre(), jugadorLogico->getColor(), casillaInicialUI);
        if (casillaInicialUI) {
            casillaInicialUI->insertar(jugadorui);
            if(columna == 2) {
                columna = 0;
                fila++;
            }
        }
        jugadoresUI.push_back(jugadorui);
        mapaJugadorToUI[jugadorLogico] = jugadorui;
    }
    this->enlistarJugadores();
    this->jugadorUIActual = mapaJugadorToUI.value(this->juego->obtenerJugadorActual());
    setTextLabelTurno();
    ui->btnTirarDado->setEnabled(true);
}


PantallaPartida::~PantallaPartida()
{
    delete juego;
    delete notificacion;
    delete tableroui;
    delete ui;
}
