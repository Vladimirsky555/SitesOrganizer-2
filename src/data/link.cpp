#include "link.h"

Link::Link(QObject *parent) : QObject(parent){}

Link::Link(QVariant id, QString link_name, QString link_real,
           QString description, QDateTime date, QObject *parent)
    : QObject(parent)
{
    this->id = id;
    this->link_name = link_name;
    this->link_real = link_real;
    this->description = description;
    this->date = date;

    this->isNew = false;
    this->isEdited = false;
    this->isDeleted = false;

    this->isMoved = false;
    this->isOpened = false;
}

QVariant Link::Id()
{
    return this->id;
}


QString Link::linkName()
{
    return this->link_name;
}

void Link::setlinkName(QString name)
{
    this->link_name = name;
}

QString Link::linkReal()
{
    return this->link_real;
}

void Link::setlinkReal(QString link)
{
    this->link_real = link;
}





QDateTime Link::Date()
{
    return this->date;
}

void Link::setDate(QDateTime date)
{
    this->date = date;
}

QString Link::Description()
{
    return this->description;
}

void Link::setDescription(QString description)
{
    this->description = description;
}
