#ifndef DADO_H
#define DADO_H
#include <QRandomGenerator>
class Dado
{
private:
    int valor;
public:
    Dado() = default;
    int tirar();
    int getValor();
    ~Dado() = default;
};

#endif // DADO_H
