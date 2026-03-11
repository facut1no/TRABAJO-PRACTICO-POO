#include "pantallatransicion.h"
#include "ui_pantallatransicion.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

PantallaTransicion::PantallaTransicion(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaTransicion)
{
    ui->setupUi(this);
    setGeometry(parent->rect());
    resize(parent->size());
    hide();
    m_EfectoOpacidad = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_EfectoOpacidad);
}

void PantallaTransicion::mostrarTransicion(int tiempo, bool mostrarLabel, std::function<void()> func, QString text) {
    show();
    raise();
    if(mostrarLabel) {
        ui->label->raise();
        ui->label->show();
        ui->label->setText(text);
    } else {
        ui->label->hide();
    }

    QPropertyAnimation* animacionEntrada = new QPropertyAnimation(m_EfectoOpacidad, "opacity");
    animacionEntrada->setDuration(tiempo);
    animacionEntrada->setStartValue(0.0);
    animacionEntrada->setEndValue(1.0);

    QPropertyAnimation* animacionSalida = new QPropertyAnimation(m_EfectoOpacidad, "opacity");
    animacionSalida->setDuration(tiempo);
    animacionSalida->setStartValue(1.0);
    animacionSalida->setEndValue(0.0);

    connect(animacionEntrada, &QPropertyAnimation::finished, this, [=, this]() {
        if (func) func();
        animacionSalida->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(animacionSalida, &QPropertyAnimation::finished, this, [=, this]() {
        hide();
    });

    animacionEntrada->start(QAbstractAnimation::DeleteWhenStopped);
}

PantallaTransicion::~PantallaTransicion()
{
    delete ui;
    delete m_EfectoOpacidad;
}
