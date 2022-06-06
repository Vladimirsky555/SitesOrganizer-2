#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>

#include "link.h"

class Folder : public QObject
{
    Q_OBJECT

    QString name;
    QList<Link*> links;

public:
    bool isNew;
    bool isEdited;
    bool isDeleted;
    bool isMoved;

public:
    Folder(QObject *parent = nullptr);
    Folder(QString name, QObject *parent = nullptr);
    virtual ~Folder();

    QString Name();
    void setName(QString name);

    void addLink(Link* li);
    void markDeletedLinkById(int id);
    void markRestoredLinkById(int id);
    void deleteLinkById(int id);
    void deleteLink(Link *li);
    Link* getLinkById(int id);
    Link* getLinkByDbId(QVariant id);
    Link* getLinkByName(QString name);
    int Count();
    void insertLink(int id, Link *li);

};

#endif // FOLDER_H
