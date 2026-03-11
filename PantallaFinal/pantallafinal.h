#ifndef PANTALLAFINAL_H
#define PANTALLAFINAL_H

#include <QWidget>
#include "../logica/Jugador/jugador.h"

namespace Ui {
class PantallaFinal;
}

class PantallaFinal : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaFinal(QWidget *parent = nullptr);
    ~PantallaFinal();
    void cargarResultados(const QList<Jugador*>& jugadores);
signals:
    void volverAlMenu();
    void volverAJugar();
private:
    void btnVolverAJugarDenuevoClickeado();
    void btnVolverAlMenuClickeado();
    Ui::PantallaFinal *ui;
};

#endif // PANTALLAFINAL_H
