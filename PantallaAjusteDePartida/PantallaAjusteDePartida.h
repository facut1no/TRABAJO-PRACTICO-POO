#ifndef PANTALLAAJUSTEDEPARTIDA_H
#define PANTALLAAJUSTEDEPARTIDA_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <QColor>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include "ConfiguracionJuego.h"
#include <QCoreApplication>

namespace Ui {
class PantallaAjusteDePartida;
}


struct ConfiguracionPartida {
    int cantidadCasillas = 63;
    int cantidadDados = 2;
    int cantidadJugadores = 3;
    QList<QPair<QString, QColor>> jugadores;
};

enum TipoError {
    NombreRepetido,
    NombreVacio,
    ColorVacio,
    ColorRepetido,
    CantidadCasillas,
    CantidadDados,
    None,
};

class PantallaAjusteDePartida : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaAjusteDePartida(QWidget *parent = nullptr);
    ~PantallaAjusteDePartida();
    void resetear();
signals:
    void volverAlMenuInicial();
    void comenzarPartida(const QList<QPair<QString, QColor>>& jugadores, int, int);
private slots:
    void slideCantCasillasCambio();
    void btnMenuInicalClickeado();
    void comboBoxCambioDeValor(int);
    void extracted(); // Esta funcion no hace nada, nose porque QT me da error al no tenerla en la clase.
    void btnSiguienteClickeado();
    void btnVolverClickeado();
    void btnComenzarClickeado();
    void showEvent(QShowEvent *event);
private:
    const QString direccionCarpeta { QCoreApplication::applicationDirPath() + "/configuracion" };
    QString direccionArchivo;
    Ui::PantallaAjusteDePartida *ui;
    QMap<QString, QColor> ajustesJugadores;
    void setearValoresComboBox();
    void resetearLinesEditNombres();
    void limpiarLayout(QLayout*);
    TipoError validarDatos();
    void setTextError(TipoError);
    void enlistarJugadores();
    QVector<QLineEdit*> m_Nombres;
    QVector<QComboBox*> m_Colores;
    QList<QPair<QString, QColor>> m_Jugadores;
    ConfiguracionJuego configuracion;
    void guardaConfiguracion();
    void cargarConfiguracion();
};

#endif // PANTALLAAJUSTEDEPARTIDA_H
