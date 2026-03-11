#include "tableroui.h"
#include "ui_tableroui.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

TableroUI::TableroUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableroUI)
{
    ui->setupUi(this);
}


void TableroUI::cargarVectorCasillas(const std::vector<Casilla*>& casillas) {
    if (casillasUI.empty()) {
        CasillaUI* casillaDeSalida = new CasillaUI(this, TipoCasilla::Normal, 0, "Salida");
        casillasUI.push_back(casillaDeSalida);
    }

    for (size_t i = 0; i < casillas.size(); ++i) {
        const Casilla* c = casillas[i];

        if (i + 1 >= casillasUI.size()) {
            CasillaUI* casillaUI = new CasillaUI(this, c->obtenerTipo(), c->obtenerNumero(), c->getMensaje());
            casillaUI->setMinimumSize(20, 20);
            casillasUI.push_back(casillaUI);
        } else {
            casillasUI[i + 1]->actualizarCasilla(c->obtenerTipo(), c->getMensaje(), c->obtenerNumero());
        }
    }
    this->totalCasillas = this->casillasUI.size();
}


void TableroUI::cargarCasillasAlTablero() {
    int filas = 11;
    int columnas = 11;
    QVector<QPoint> posiciones;

    int arriba = 0, abajo = filas - 1;
    int izquierda = 0, derecha = columnas - 1;

    while (arriba <= abajo && izquierda <= derecha) {
        for (int j = izquierda; j <= derecha; ++j)
            posiciones.append(QPoint(arriba, j));
        ++arriba;

        for (int i = arriba; i <= abajo; ++i)
            posiciones.append(QPoint(i, derecha));
        --derecha;

        if (arriba <= abajo) {
            for (int j = derecha; j >= izquierda; --j)
                posiciones.append(QPoint(abajo, j));
            --abajo;
        }

        if (izquierda <= derecha) {
            for (int i = abajo; i >= arriba; --i)
                posiciones.append(QPoint(i, izquierda));
            ++izquierda;
        }
    }

    for (int i = 0; i < casillasUI.size() && i < posiciones.size(); ++i) {
        const QPoint& pos = posiciones[i];
        ui->tablero->addWidget(casillasUI[i], pos.x(), pos.y());
    }
}

CasillaUI* TableroUI::obtenerCasillaUI(int indiceUI) {
    if(indiceUI >= 0 && indiceUI < this->casillasUI.size())
        return this->casillasUI[indiceUI];
    return nullptr;
}

void TableroUI::reiniciar() {
    qDeleteAll(this->casillasUI);
    this->casillasUI.clear();
}

TableroUI::~TableroUI()
{
    qDeleteAll(this->casillasUI);
    casillasUI.clear();
    delete ui;
}
