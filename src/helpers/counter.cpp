#include "counter.h"

Counter::Counter(QObject *parent) : QObject(parent){}

int Counter::getNum()
{
    return this->num;
}


void Counter::acceptCount(int num)
{
    this->num = num;
    emit sendValue(this->num);
}
