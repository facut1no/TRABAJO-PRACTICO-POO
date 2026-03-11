#include "notificacion.h"
#include "PantallaPartida/ui_notificacion.h"
#include <QPixmap>
#include <QTimer>
#include <QPropertyAnimation>
Notificacion::Notificacion(QWidget *parent)
    : QWidget(parent), ui(new Ui::Notificacion)
{
    ui->setupUi(this);
    this->hide();
    this->raise();

    animacionEntrada = new QPropertyAnimation(this, "pos", this);
    animacionEntrada->setDuration(500);
    animacionEntrada->setEasingCurve(QEasingCurve::OutExpo);

    animacionSalida = new QPropertyAnimation(this, "pos", this);
    animacionSalida->setDuration(500);
    animacionSalida->setEasingCurve(QEasingCurve::InExpo);

    connect(animacionSalida, &QPropertyAnimation::finished, this, [this]() {
        this->hide();
        emit animacionNotificacionFinalizada();
    });
}

void Notificacion::mostrar(QString rutaImagen, QString mensaje, std::function<void()> funcion) {
    QPixmap pixmap(rutaImagen);
    if (pixmap.isNull()) {
        pixmap = QPixmap(":/Imagenes/Recursos/Imagenes/oca-default.png");
    }
    ui->imagen->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->mensaje->setText(mensaje);

    int margenInferior = 30;
    int centroX = (this->parentWidget()->width() - this->width()) / 2;
    int destinoY = this->parentWidget()->height() - this->height() - margenInferior;

    this->move(centroX, this->parentWidget()->height());
    this->show();
    this->raise();

    animacionEntrada->stop();
    animacionEntrada->setStartValue(QPoint(centroX, this->parentWidget()->height()));
    animacionEntrada->setEndValue(QPoint(centroX, destinoY));
    animacionEntrada->start();

    disconnect(animacionEntrada, nullptr, nullptr, nullptr);
    connect(animacionEntrada, &QPropertyAnimation::finished, this, [this, centroX, funcion]() {
        QTimer::singleShot(1500, this, [this, centroX, funcion]() {
            cerrar(centroX, funcion);
        });
    });
}

void Notificacion::cerrar(int centroX, std::function<void()> funcion) {
    animacionSalida->stop();
    animacionSalida->setStartValue(this->pos());
    animacionSalida->setEndValue(QPoint(centroX, this->parentWidget()->height()));
    animacionSalida->start();

    disconnect(animacionSalida, nullptr, nullptr, nullptr);
    connect(animacionSalida, &QPropertyAnimation::finished, this, [this, funcion]() {
        this->hide();
        if (funcion) funcion();
        emit animacionNotificacionFinalizada();
    });
}
Notificacion::~Notificacion()
{
    delete ui;
}
