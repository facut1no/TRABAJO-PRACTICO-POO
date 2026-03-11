#include "casillaui.h"
#include "ui_casillaui.h"

CasillaUI::CasillaUI(QWidget *parent, TipoCasilla tipo, int num, QString mensaje)
    : QWidget(parent),
    ui(new Ui::CasillaUI),
    tipo(tipo),
    mensaje(mensaje),
    num(num)
{
    ui->setupUi(this);
    ui->imagenCasilla->lower();
    this->setObjectName("CasillaUI");
    this->setAutoFillBackground(true);

    setMinimumSize(60, 60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground, true);
    setearBackground();
}

void CasillaUI::setearBackground() {
    this->setObjectName("CasillaUI");
    auto it = tipoMap.find(this->tipo);
    if (it != tipoMap.end()) {
        QColor color = it->second.first;
        QString imagenPath = it->second.second;
        this->setStyleSheet(QString("#CasillaUI { background-color: %1; border: 1px solid black; }").arg(color.name()));
        this->imagenPath = imagenPath;
    } else {
        QColor color;

        if (this->getNumero() % 2 == 0) {
            color = QColor(0xd0e6f6);
        } else {
            color = QColor(0xa9cce3);
        }

        this->setStyleSheet(QString("background-color: %1; border: 1px solid #555;").arg(color.name()));
        this->imagenPath = "";

    }

    this->setObjectName("CasillaUI");

    if(!this->imagenPath.isEmpty()) {
        this->pixmap = new QPixmap(this->imagenPath);
    } else {
        this->pixmap = new QPixmap();
    }
}

void CasillaUI::actualizarCasilla(TipoCasilla nuevoTipo, QString nuevoMensaje, int nuevoNum) {
    this->tipo = nuevoTipo;
    this->mensaje = nuevoMensaje;
    this->num = nuevoNum;
    setearBackground();
    update();
}


QString CasillaUI::obtenerRutaImagen() const {
    return this->imagenPath;
}

QString CasillaUI::obtenerMensaje() const {
    return this->mensaje;
}

void CasillaUI::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    if (!this->pixmap->isNull()) {
        int ancho = this->width() * 0.8;
        int alto = this->height() * 0.8;
        QPixmap escala = pixmap->scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint centro((width() - escala.width())/2, (height() - escala.height())/2);
        painter.drawPixmap(centro, escala);
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignLeft, QString::number(this->num));
}



void CasillaUI::insertar(QWidget* widget) {
    const int maxColumnas = 3;
    int count = ui->contenedor->count();
    int fila = count / maxColumnas;
    int columna = count % maxColumnas;

    ui->contenedor->addWidget(widget, fila, columna);
    widget->setFixedSize(25, 25);
    widget->show();
    widget->raise();
}

void CasillaUI::remover(QWidget* jugador) {
    if (!jugador) return;
    ui->contenedor->removeWidget(jugador);
    jugador->setParent(nullptr);
    this->update();
}

CasillaUI::~CasillaUI() {
    delete pixmap;
    delete ui;
}
