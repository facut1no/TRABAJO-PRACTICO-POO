#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QStackedWidget>
#include "PantallaMenu/PantallaMenu.h"
#include "PantallaPartida/PantallaPartida.h"
#include "PantallaAjusteDePartida/PantallaAjusteDePartida.h"
#include "PantallaTransicion/pantallatransicion.h"
#include "PantallaPausa/pantallapausa.h"
#include "PantallaFinal/pantallafinal.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui{};
    QStackedWidget* m_Pantallas{};
    PantallaMenu* m_PantallaMenu{};
    PantallaPartida* m_PantallaPartida{};
    PantallaAjusteDePartida* m_PantallaDeAjusteDePartida{};
    PantallaTransicion* m_PantallaTransicion{};
    PantallaPausa* m_PantallaPausa{};
    PantallaFinal* m_PantallaFinal{};

    int centroPantallaX;
    int centroPantallaY;
    void setPantallaPartidaCargada();

    int pantallaAnterior;
    void volverAJugar();
    void resizeEvent(QResizeEvent *event);
    void setPantallaFinal(const QList<Jugador*>& jugadores);
    void transicion(QWidget* nuevoWidget, bool izquierdaAderecha);
    void transicion(QWidget* oldWidget, QWidget* newWidget);
    void setMenuDeInicio();
    void setPantallaAjusteDePartida();
    void conexionesPantallaDeAjusteDePartida();
    void conexionesPantallaFinal();
    void setPantallaCompleta();
    void volverALaPartida();
    void setModoVentana();
    void calcularCentroPantalla();
    void conexionesPantallaMenu();
    void crearPantallas();
    void setPantallaPartida(const QList<QPair<QString, QColor>>& jugadores, int cantCasillas, int cantDados);
    void setPantallaPausa();
    void conexionesPantallaPartida();
    void conexionesPantallaPausa();
    void setVolumenSliders();
    void setSonidosSliders();
};
#endif // MAINWINDOW_H
