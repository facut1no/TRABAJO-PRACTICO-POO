#ifndef JUGADORUI_H
#define JUGADORUI_H

#include <QWidget>
#include <QColor>
#include <QString>
#include <qpropertyanimation.h>
#include "../CasillaUI/casillaui.h"
#include "../TableroUI/tableroui.h"

namespace Ui {
class JugadorUI;
}

class JugadorUI : public QWidget
{
    Q_OBJECT

public:
    explicit JugadorUI(QString nombre, QColor color, QWidget *parent = nullptr);
    ~JugadorUI();
    void mover(CasillaUI*, CasillaUI*, TableroUI*);
    void moverPorRuta(const QList<CasillaUI*>&, QWidget*);
signals:
    void animacionMovimientoTerminada();
public slots:
    // void iniciarAnimacionMovimiento(QPoint, QPoint);
private:
    Ui::JugadorUI *ui;
    QString nombre;
    QColor color;
    QPropertyAnimation* currentAnimation = nullptr;
};

#endif // JUGADORUI_H
