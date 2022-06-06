#include "folder.h"

Folder::Folder(QObject *parent) : QObject(parent)
{
    this->name = "";
    this->isNew = false;
    this->isEdited = false;
    this->isDeleted = false;
    this->isMoved = false;
}

Folder::Folder(QString name, QObject *parent) : QObject(parent)
{
    this->name = name;
    this->isNew = false;
    this->isEdited = false;
    this->isDeleted = false;
    this->isMoved = false;
}

Folder::~Folder(){}

QString Folder::Name()
{
    return this->name;

}

void Folder::setName(QString name)
{
    this->name = name;
}

void Folder::addLink(Link *li)
{
    if(links.contains(li)) return;
    links.append(li);
}

//Элемент из базы помечаем для удаления из базы после закрытия программы
void Folder::markDeletedLinkById(int id)
{
    links.at(id)->isDeleted = true;
}

void Folder::markRestoredLinkById(int id)
{
    links.at(id)->isDeleted = false;
}

//Удаляем только новый элемент
void Folder::deleteLinkById(int id)
{
    links.removeAt(id);
}

void Folder::deleteLink(Link *li)
{
    if(li != NULL)
    links.removeAll(li);
}

Link *Folder::getLinkById(int id)
{
    return links.at(id);
}

Link *Folder::getLinkByDbId(QVariant id)
{
    for(int i = 0; i < links.count(); i++)
    {
        if(links.at(i)->Id() == id)
        {
            return links.at(i);
        }
    }

    return NULL;
}

Link *Folder::getLinkByName(QString name)
{
    for(int i = 0; i < links.count(); i++)
    {
        if(links.at(i)->linkName() == name)
        {
            return links.at(i);
        }
    }

    return NULL;
}

int Folder::Count()
{
    return links.size();
}

void Folder::insertLink(int id, Link *li)
{
    if (links.contains(li)) return;

    if(links.count() == 0){
        links.append(li);
        return;
    }

    this->links.insert(id+1, li);
    return;
}
