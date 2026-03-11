#ifndef PANTALLAMENU_H
#define PANTALLAMENU_H

#include <QWidget>
#include <QSoundEffect>

namespace Ui {
class PantallaMenu;
}

class PantallaMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaMenu(QWidget *parent = nullptr);
    ~PantallaMenu();
    void setSliderMusicaValue();
    void setSliderSonidoValue();
    void cambiarBtnPantalla(bool);
signals:
    void setPantallaCompleta();
    void setModoVentana();
    void setPantallaJugar();
    void volumenMusicaCambiado();
    void volumenSonidoCambiado();
    void cargarPartida();
private slots:
    void btnJugarClickeado();
    void btnOpcionesClickeado();
    void btnCreditosClickeado();
    void btnSalirClickeado();
    void musicaSliderCambio();
    void sonidosSliderCambio();
    void btnModoVentanaClickeado();
    void btnPantallaCompletaClickeado();
    void btnVolverClickeado();
    void showEvent(QShowEvent *event);
private:
    Ui::PantallaMenu *ui;
};

#endif // PANTALLAMENU_H
