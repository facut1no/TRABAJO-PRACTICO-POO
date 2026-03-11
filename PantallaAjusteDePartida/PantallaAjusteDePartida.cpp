#include "PantallaAjusteDePartida.h"
#include "ui_PantallaAjusteDePartida.h"
#include "../ManejadorDeSonidos/manejadordesonidos.h"
#include <QLineEdit>
#include <QDebug>
#include <fstream>
#include <QColorDialog>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QFile>
#include <QDir>
#include <regex>


PantallaAjusteDePartida::PantallaAjusteDePartida(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaAjusteDePartida)
{
    ui->setupUi(this);
    setearValoresComboBox();
    connect(ui->btnMenuInicial, &QPushButton::clicked, this, &PantallaAjusteDePartida::btnMenuInicalClickeado);
    connect(ui->btnSiguiente, &QPushButton::clicked, this, &PantallaAjusteDePartida::btnSiguienteClickeado);
    connect(ui->btnVolver, &QPushButton::clicked, this, &PantallaAjusteDePartida::btnVolverClickeado);
    connect(ui->btnComenzar, &QPushButton::clicked, this, &PantallaAjusteDePartida::btnComenzarClickeado);
    connect(ui->cantCasillas, &QSlider::valueChanged, this, &PantallaAjusteDePartida::slideCantCasillasCambio);
    connect(ui->cantidadJugadores, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PantallaAjusteDePartida::comboBoxCambioDeValor);

    ui->btnSiguiente->setEnabled(true);
    ui->tablaJugadores->setColumnCount(2);
    ui->tablaJugadores->setHorizontalHeaderLabels(QStringList() << "Nombre" << "Color");

    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("archivosConfiguracion"))
        dir.mkpath("archivosConfiguracion");

    this->direccionArchivo = dir.filePath("archivosConfiguracion/config.txt");
    qDebug() << "Archivo de configuración:" << this->direccionArchivo;


}

void PantallaAjusteDePartida::setTextError(TipoError error) {
    QString mensaje;
    switch (error) {
    case TipoError::ColorRepetido:
        mensaje = "Hay colores repetidos. Por favor, elija un color diferente para cada jugador.";
        break;
    case TipoError::NombreRepetido:
        mensaje = "Hay nombres repetidos. Cada jugador debe tener un nombre único.";
        break;
    case TipoError::NombreVacio:
        mensaje = "Hay nombres vacíos. Todos los jugadores deben tener un nombre.";
        break;
    case TipoError::ColorVacio:
        mensaje = "Hay colores sin seleccionar. Seleccione un color para cada jugador.";
        break;
    case TipoError::CantidadCasillas:
        mensaje = "La cantidad de casillas ingresadas no repeta el rango indicado.";
        break;
    case TipoError::CantidadDados:
        mensaje = "Debe seleccionar la cantidad de dados antes de continuar.";
        break;
    default:
        mensaje = "";
        break;
    }
    ui->labelError->setText(mensaje);
}

void PantallaAjusteDePartida::btnSiguienteClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    TipoError error = validarDatos();
    if(error == TipoError::None) {
        ui->stackAjustes->setCurrentWidget(ui->pageAjusteJugadores);
        ui->stackBtn->setCurrentWidget(ui->pageBtnComenzar);
        enlistarJugadores();
        this->guardaConfiguracion();
    } else {
        setTextError(error);
    }
}

void PantallaAjusteDePartida::btnVolverClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    ui->stackAjustes->setCurrentWidget(ui->pageCantJugadores);
    ui->stackBtn->setCurrentWidget(ui->pageBtnSiguiente);
}

void PantallaAjusteDePartida::setearValoresComboBox() {
    ui->cantidadJugadores->addItem(QString("2"), QVariant(2));
    ui->cantidadJugadores->addItem(QString("3"), QVariant(3));
    ui->cantidadJugadores->addItem(QString("4"), QVariant(4));
    ui->cantidadJugadores->addItem(QString("6"), QVariant(6));

    ui->cantidadDados->addItem(QString("1"), QVariant(1));
    ui->cantidadDados->addItem(QString("2"), QVariant(2));
}

void PantallaAjusteDePartida::btnMenuInicalClickeado() {
    ManejadorDeSonidos::instancia().reproducirBoton();
    emit volverAlMenuInicial();
}

void PantallaAjusteDePartida::slideCantCasillasCambio() {
    ui->strCantCasillas->setText(QString::number(ui->cantCasillas->value()));
}

void PantallaAjusteDePartida::btnComenzarClickeado() {
   ManejadorDeSonidos::instancia().reproducirBoton();
    ManejadorDeSonidos::instancia().pausarMusicaMenu();
   this->cargarConfiguracion();
}


void PantallaAjusteDePartida::enlistarJugadores() {
    ui->tablaJugadores->setRowCount(m_Nombres.size());

    for (int i = 0; i < m_Nombres.size(); ++i) {
        QString nombre = m_Nombres[i]->text();
        QColor color = m_Colores[i]->currentData().value<QColor>();

        QTableWidgetItem* itemNombre = new QTableWidgetItem(nombre);
        itemNombre->setTextAlignment(Qt::AlignCenter);
        ui->tablaJugadores->setItem(i, 0, itemNombre);

        QTableWidgetItem* itemColor = new QTableWidgetItem();
        itemColor->setBackground(QBrush(color));
        ui->tablaJugadores->setItem(i, 1, itemColor);
    }
    ui->tablaJugadores->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tablaJugadores->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tablaJugadores->horizontalHeader()->setStretchLastSection(true);
    ui->tablaJugadores->verticalHeader()->setVisible(false);
    ui->tablaJugadores->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tablaJugadores->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void PantallaAjusteDePartida::limpiarLayout(QLayout *layout) {
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        if (QLayout *childLayout = item->layout()) {
            limpiarLayout(childLayout);
        }
        delete item;
    }
}


TipoError PantallaAjusteDePartida::validarDatos() {
    QSet<QString> nombres;
    QSet<QString> colores;
    for(int i = 0; i < m_Nombres.size(); i++) {
        QString nombre = m_Nombres[i]->text().trimmed();
        QString color = m_Colores[i]->currentData().value<QColor>().name();

        if(color == "#ddd") {
            return TipoError::ColorVacio;
        }

        if(nombre.isEmpty()) {
            return TipoError::NombreVacio;
        }

        if(nombres.contains(nombre)) {
            return TipoError::NombreRepetido;
        } else {
            nombres.insert(nombre);
        }

        if(colores.contains(color)) {
            return TipoError::ColorRepetido;
        } else {
            colores.insert(color);
        }
    }
    int dados = ui->cantidadDados->currentData().value<int>();
    if(dados < 1) {
        return TipoError::CantidadDados;
    }

    return TipoError::None;
}

void PantallaAjusteDePartida::comboBoxCambioDeValor(int indice) {
    int cantJugadores = ui->cantidadJugadores->itemData(indice).toInt();
    limpiarLayout(ui->formLayoutJugadores);
    m_Nombres.clear();
    m_Colores.clear();
    QList<QColor> colores = { Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::cyan };
    for(int i = 0; i < cantJugadores; i++) {
        QLineEdit* nombreEdit = new QLineEdit(this);
        nombreEdit->setText(QString("Jugador %1").arg(i + 1));
        nombreEdit->setMaxLength(15);
        QComboBox* comboColor = new QComboBox(this);
        for (const QColor& color : colores) {
            QPixmap pixmap(30, 20);
            pixmap.fill(color);
            comboColor->addItem(QIcon(pixmap), "");
            comboColor->setItemData(comboColor->count() - 1, color);
        }

        comboColor->setStyleSheet(R"(
        QComboBox {
            padding: 3px;
            min-height: 25px;
        }
        QComboBox QAbstractItemView {
            padding: 2px;
            selection-background-color: #ddd;
        }
    )");

        QHBoxLayout* filaLayout = new QHBoxLayout();
        filaLayout->addWidget(nombreEdit);
        filaLayout->addWidget(comboColor);
        ui->formLayoutJugadores->addRow(filaLayout);

        m_Colores.push_back(comboColor);
        m_Nombres.push_back(nombreEdit);
    }
}

void PantallaAjusteDePartida::resetear() {
    ui->stackAjustes->setCurrentWidget(ui->pageCantJugadores);
    ui->stackBtn->setCurrentWidget(ui->pageBtnSiguiente);

    ui->labelError->clear();
    ui->tablaJugadores->clearContents();
    ui->tablaJugadores->setRowCount(0);

    limpiarLayout(ui->formLayoutJugadores);
    m_Nombres.clear();
    m_Colores.clear();
    m_Jugadores.clear();

    ui->cantCasillas->setValue(63);
    ui->strCantCasillas->setText(QString::number(ui->cantCasillas->value()));

    ui->cantidadJugadores->setCurrentIndex(0);
    ui->cantidadDados->setCurrentIndex(0);
}


void PantallaAjusteDePartida::guardaConfiguracion() {
    std::ofstream archivo(this->direccionArchivo.toStdString());

    if(!archivo.is_open()) {
        qDebug() << "No se pudo guardar la configuracion en:" << this->direccionArchivo;
        return;
    }

    archivo << "CANT_DADOS = " << ui->cantidadDados->currentData().value<int>() << "\n";
    archivo << "CANT_CASILLAS = " << ui->cantCasillas->value() << "\n";
    archivo << "CANT_JUGADORES = " << ui->cantidadJugadores->currentData().value<int>() << "\n\n";

    for (int i = 0; i < m_Nombres.size(); ++i) {
        QString nombre = m_Nombres[i]->text();
        QColor color = m_Colores[i]->currentData().value<QColor>();

        archivo << "JUGADOR" << (i + 1) << "_NOMBRE = " << nombre.toStdString() << "\n";
        archivo << "JUGADOR" << (i + 1) << "_COLOR = " << color.name().toStdString() << "\n";
    }

    archivo.close();
}

void PantallaAjusteDePartida::cargarConfiguracion() {
    std::ifstream archivo(this->direccionArchivo.toStdString());
    if (!archivo.is_open()) {
        qDebug() << "No se pudo abrir el archivo de configuración:" << this->direccionArchivo;
        return;
    }

    ConfiguracionPartida config;
    std::string linea;
    std::map<int, QPair<QString, QColor>> tempJugadores;

    auto trim = [](std::string &s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    };

    while (std::getline(archivo, linea)) {
        size_t pos = linea.find('=');
        if (pos == std::string::npos)
            continue;

        std::string clave = linea.substr(0, pos);
        std::string valor = linea.substr(pos + 1);

        trim(clave);
        trim(valor);

        if (clave == "CANTIDAD_CASILLAS" || clave == "CANT_CASILLAS")
            config.cantidadCasillas = std::stoi(valor);
        else if (clave == "CANTIDAD_DADOS" || clave == "CANT_DADOS")
            config.cantidadDados = std::stoi(valor);
        else if (clave == "CANTIDAD_JUGADORES" || clave == "CANT_JUGADORES")
            config.cantidadJugadores = std::stoi(valor);
        else if (clave.find("JUGADOR") == 0) {
            std::regex re(R"(JUGADOR(\d+)_(NOMBRE|COLOR))");
            std::smatch match;
            if (std::regex_match(clave, match, re)) {
                int index = std::stoi(match[1]) - 1;
                std::string campo = match[2];

                if (campo == "NOMBRE") {
                    tempJugadores[index].first = QString::fromStdString(valor);
                } else if (campo == "COLOR") {
                    tempJugadores[index].second = QColor(QString::fromStdString(valor));
                }
            }
        }
    }

    archivo.close();

    for (int i = 0; i < config.cantidadJugadores; ++i) {
        if (tempJugadores.count(i))
            config.jugadores.push_back(tempJugadores[i]);
    }

   emit comenzarPartida(config.jugadores, config.cantidadCasillas, config.cantidadDados);
}

void PantallaAjusteDePartida::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    this->resetear();
}

void PantallaAjusteDePartida::extracted() {}

PantallaAjusteDePartida::~PantallaAjusteDePartida()
{
    delete ui;
}
