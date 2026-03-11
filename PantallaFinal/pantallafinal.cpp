#include "pantallafinal.h"
#include "ui_pantallafinal.h"
#include <QTableWidgetItem>

PantallaFinal::PantallaFinal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaFinal)
{
    ui->setupUi(this);
    ui->tabla->setColumnCount(3);
    QStringList cabecera;
    cabecera << "Posición" << "Nombre" << "Color";
    ui->tabla->setHorizontalHeaderLabels(cabecera);
    ui->tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabla->verticalHeader()->setVisible(false);

    connect(ui->btnVolverAlMenu, &QPushButton::clicked, this, &PantallaFinal::btnVolverAlMenuClickeado);
    connect(ui->btnJugarDenuevo, &QPushButton::clicked, this, &PantallaFinal::btnVolverAJugarDenuevoClickeado);
}

void PantallaFinal::cargarResultados(const QList<Jugador*>& jugadores)
{
    ui->tabla->setRowCount(jugadores.size());
    for (int i = 0; i < jugadores.size(); ++i) {
        Jugador* jugador = jugadores[i];

        QTableWidgetItem* posicion= new QTableWidgetItem(QString::number(i + 1));
        posicion->setTextAlignment(Qt::AlignCenter);
        ui->tabla->setItem(i, 0, posicion);

        QTableWidgetItem* nombre= new QTableWidgetItem(jugador->getNombre());
        nombre->setTextAlignment(Qt::AlignCenter);
        ui->tabla->setItem(i, 1, nombre);

        QTableWidgetItem* color= new QTableWidgetItem();
        color->setFlags(color->flags() & ~Qt::ItemIsEditable);
        color->setBackground(QBrush(jugador->getColor()));
        ui->tabla->setItem(i, 2, color);
    }
}

void PantallaFinal::btnVolverAlMenuClickeado() {
    emit volverAlMenu();
}

void PantallaFinal::btnVolverAJugarDenuevoClickeado() {
    emit volverAJugar();
}


PantallaFinal::~PantallaFinal()
{
    delete ui;
}
