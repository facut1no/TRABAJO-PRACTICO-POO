#ifndef CASILLAUI_H
#define CASILLAUI_H

#include <QWidget>
#include "../logica/Casillas/casilla.h"
#include <QPainter>

#include <QLabel>
namespace Ui {
class CasillaUI;
}

class CasillaUI : public QWidget
{
    Q_OBJECT

public:
    explicit CasillaUI(QWidget *parent = nullptr, TipoCasilla tipo = TipoCasilla::Normal, int num = 0, QString mensaje = "");
    ~CasillaUI();
    void insertar(QWidget*);
    void remover(QWidget*);
    QString obtenerRutaImagen() const;
    QString obtenerMensaje() const;
    int getNumero() const {return this->num;}
    void actualizarCasilla(TipoCasilla nuevoTipo, QString nuevoMensaje, int nuevoNum);
    void actualizarImagenPath();
private:
    QPixmap* pixmap;
    void paintEvent(QPaintEvent* event);
    QString imagenPath;
    Ui::CasillaUI *ui;
    TipoCasilla tipo;
    int num;
    void setearBackground();
    QString mensaje;

    const std::map<TipoCasilla, std::pair<QColor, QString>> tipoMap = {
        {TipoCasilla::Calavera,     {QColor(0x2E2E2E), ":/Imagenes/Recursos/Imagenes/Casillas/calavera.png"}},
        {TipoCasilla::Carcel,       {QColor(0xA93226), ":/Imagenes/Recursos/Imagenes/Casillas/carcel.png"}},
        {TipoCasilla::Laberinto,    {QColor(0x1F618D), ":/Imagenes/Recursos/Imagenes/Casillas/laberinto.png"}},
        {TipoCasilla::Puente,       {QColor(0x117A65), ":/Imagenes/Recursos/Imagenes/Casillas/puente.png"}},
        {TipoCasilla::Posada,       {QColor(0xAF7AC5), ":/Imagenes/Recursos/Imagenes/Casillas/posada.png"}},
        {TipoCasilla::Pozo,         {QColor(0x5D6D7E), ":/Imagenes/Recursos/Imagenes/Casillas/pozo.png"}},
        {TipoCasilla::JardinDeOca,  {QColor(0x229954), ":/Imagenes/Recursos/Imagenes/Logo-oca.png"}},
        {TipoCasilla::Oca,          {QColor(0xF1C40F), ":/Imagenes/Recursos/Imagenes/Casillas/oca.png"}},
        {TipoCasilla::Normal,       {QColor(0xD5DBDB), ""}}
    };
};

#endif // CASILLAUI_H
