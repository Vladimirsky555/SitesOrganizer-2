#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>

#include "catalog.h"

class Storage : public QObject
{
    Q_OBJECT

    QList<Catalog*> catalogs;

public:
    Storage(QObject *parent = nullptr);
    virtual ~Storage(){}

    QList<Catalog*> Catalogs();
    void addCatalog(Catalog* cat);
    void deleteCatalogById(int id);
    void deleteCatalog(Catalog *ci);
    Catalog* getCatalogById(int id);
    Catalog* getCatalogByName(QString name);
    int getCount();
    void insertCatalog(int id, Catalog *cat);
};

#endif // STORAGE_H
