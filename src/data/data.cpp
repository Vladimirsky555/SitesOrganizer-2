#include "data.h"

#include <QtSql>

Data::Data(QObject *parent) :
    QObject(parent)
{
    this->date = QDateTime::currentDateTime();

    this->isDeleted = false;
    this->isEdited = false;
    this->isNew = false;

    this->isMoved = false;
    this->isOpened = false;
}

Data::Data(QObject *parent, QSqlQuery &qry) :
    QObject(parent)
{
    this->id = qry.value("id");
    this->catalog = qry.value("catalog").toString();
    this->folder = qry.value("folder").toString();
    this->date = qry.value("date").toDateTime();
    this->link_name = qry.value("link_name").toString();
    this->link_real = qry.value("link_real").toString();
    this->description = qry.value("description").toString();

    this->isDeleted = false;
    this->isEdited = false;
    this->isNew = false;
    this->isMoved = false;
}

QVariant Data::Id()
{
    return this->id;
}

void Data::setId(QVariant id)
{
    this->id = id;
}

QString Data::Catalog()
{
    return this->catalog;
}

void Data::setCatalog(QString category)
{
    this->catalog = category;
}

QString Data::Folder()
{
    return this->folder;
}

void Data::setFolder(QString folder)
{
    this->folder = folder;
}

QDateTime Data::Date()
{
    return this->date;
}

void Data::setDate(QDateTime date)
{
    this->date = date;
}

QString Data::linkName()
{
    return this->link_name;
}

void Data::setlinkName(QString name)
{
    this->link_name = name;
}

QString Data::linkReal()
{
    return this->link_real;
}

void Data::setlinkReal(QString link)
{
    this->link_real = link;
}

QString Data::Description()
{
    return this->description;
}

void Data::setDescription(QString description)
{
    this->description = description;
}
