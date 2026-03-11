#ifndef PANTALLAPAUSA_H
#define PANTALLAPAUSA_H

#include <QWidget>

namespace Ui {
class PantallaPausa;
}

class PantallaPausa : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaPausa(QWidget *parent = nullptr);
    ~PantallaPausa();
    void setSliderMusicaValue();
    void setSliderSonidoValue();
    void cambiarBtnPantalla(bool);
signals:
    void renaudarPartida();
    void volverAlInicio();
    void setVolumenMusica();
    void setVolumenSonidos();
    void setModoVentana();
    void setPantallaCompleta();
    void salirDePausa();
    void guardarPartida();
private slots:
    void btnVolverAlInicioClickeado();
    void btnConfiguracionClickeado();
    void btnRenaudarClickeado();
    void btnVolverClickeado();
    void sliderMusicaCambio();
    void sliderSonidoCambio();
    void keyPressEvent(QKeyEvent *event);

    void showEvent(QShowEvent* event);
    void btnPantallaCompletaClickeado();
    void btnModoVentanaClickeado();

private:
    Ui::PantallaPausa *ui;
};

#endif // PANTALLAPAUSA_H
