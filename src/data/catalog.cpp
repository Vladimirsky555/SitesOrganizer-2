#include "catalog.h"

Catalog::Catalog(QObject *parent) : QObject(parent)
{
    this->name = "";
    this->isNew = false;
    this->isEdited = false;
    this->isDeleted = false;
}

Catalog::Catalog(QString name, QObject *parent) : QObject(parent)
{
    this->name = name;
    this->isNew = false;
    this->isEdited = false;
    this->isDeleted = false;
}

QString Catalog::Name()
{
    return this->name;
}

void Catalog::setName(QString name)
{
    this->name = name;
}

QList<Folder *> Catalog::Folders()
{
    return this->folders;
}

void Catalog::addFolder(Folder *fi)
{
    if(folders.contains(fi)) return;
    folders.append(fi);
}

void Catalog::deleteFolderById(int id)
{
    folders.removeAt(id);
}

void Catalog::deleteFolder(Folder *fi)
{
    if(fi != NULL)
        folders.removeAll(fi);
}

void Catalog::markDeletedFolder(Folder *f)
{
    f->isDeleted = true;
}

void Catalog::markRestoredFolder(Folder *f)
{
    f->isDeleted = false;
}

void Catalog::markDeletedFolderById(int id)
{
    folders.at(id)->isDeleted = true;
}

void Catalog::markRestoredFolderById(int id)
{
    folders.at(id)->isDeleted = false;
}

Folder *Catalog::getFolder(int id)
{
    return this->folders.at(id);
}

Folder *Catalog::getFolderByName(QString folder)
{
    for(int i = 0; i < folders.count(); i++)
    {
        if(folders.at(i)->Name() == folder)
        {
            return folders.at(i);
        }
    }

    return NULL;
}

int Catalog::getCount()
{
    return this->folders.size();
}

void Catalog::insertFolder(int id, Folder *fi)
{
    if (folders.contains(fi)) return;

    if(folders.count() == 0){
        folders.append(fi);
        return;
    }

    this->folders.insert(id+1, fi);
    return;
}
