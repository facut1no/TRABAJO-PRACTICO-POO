#ifndef TABLEROUI_H
#define TABLEROUI_H

#include <QWidget>
#include "../logica/Casillas/casilla.h"
#include "../CasillaUI/casillaui.h"

namespace Ui {
class TableroUI;
}

class TableroUI : public QWidget
{
    Q_OBJECT

public:
    explicit TableroUI(QWidget *parent = nullptr);
    ~TableroUI();
    void cargarCasillasAlTablero();
    void cargarVectorCasillas(const std::vector<Casilla *> &casillas);
    CasillaUI* obtenerCasillaUI(int);
    void reiniciar();
private:
    int totalCasillas;
    Ui::TableroUI *ui;
    QVector<CasillaUI*> casillasUI;
};

#endif // TABLEROUI_H
