#ifndef PANTALLATRANSICION_H
#define PANTALLATRANSICION_H

#include <QWidget>

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class PantallaTransicion;
}

class PantallaTransicion : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaTransicion(QWidget *parent = nullptr);
    ~PantallaTransicion();

    void mostrarTransicion(int, bool, std::function<void()>, QString = "Cargando...");
private:
    Ui::PantallaTransicion *ui;
    QGraphicsOpacityEffect* m_EfectoOpacidad;
};

#endif // PANTALLATRANSICION_H
