#include "storage.h"

Storage::Storage(QObject *parent) : QObject(parent){}

QList<Catalog *> Storage::Catalogs()
{
    return this->catalogs;
}

void Storage::addCatalog(Catalog *cat)
{
    if(catalogs.contains(cat)) return;
    catalogs.append(cat);
}

void Storage::deleteCatalogById(int id)
{
    catalogs.removeAt(id);
}

void Storage::deleteCatalog(Catalog *ci)
{
    if(ci != NULL)
        catalogs.removeAll(ci);
}

Catalog *Storage::getCatalogByName(QString name)
{
    for(int i = 0; i < catalogs.count(); i++)
    {
        if(catalogs.at(i)->Name() == name)
        {
            return catalogs.at(i);
        }
    }

    return NULL;
}


Catalog *Storage::getCatalogById(int id)
{
    return catalogs.at(id);
}

int Storage::getCount()
{
    return catalogs.count();
}

void Storage::insertCatalog(int id, Catalog *cat)
{
    if (catalogs.contains(cat)) return;

    if(catalogs.count() == 0){
        catalogs.append(cat);
        return;
    }

    this->catalogs.insert(id+1, cat);
    return;
}
