#ifndef NOTIFICACION_H
#define NOTIFICACION_H

#include <QWidget>
#include <qpropertyanimation.h>

namespace Ui {
class Notificacion;
}

class Notificacion : public QWidget
{
    Q_OBJECT

public:
    explicit Notificacion(QWidget *parent = nullptr);
    ~Notificacion();
    void mostrar(QString rutaImagen, QString mensaje, std::function<void ()> funcion);
    void cerrar(int,  std::function<void ()> funcion);
signals:
    void animacionNotificacionFinalizada();
private:
    Ui::Notificacion *ui;
    QTimer* temporizador;
    QPropertyAnimation* animacionEntrada;
    QPropertyAnimation* animacionSalida;
};

#endif // NOTIFICACION_H
