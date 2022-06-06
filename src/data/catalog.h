#ifndef CATALOG_H
#define CATALOG_H

#include <QObject>

#include "folder.h"

class Catalog : public QObject
{
    Q_OBJECT

    QString name;
    QList<Folder*> folders;

public:
    bool isNew;
    bool isEdited;
    bool isDeleted;

public:
    Catalog(QObject *parent = nullptr);
    Catalog(QString name, QObject *parent = nullptr);
    virtual ~Catalog(){}

    QString Name();
    void setName(QString name);

    QList<Folder*> Folders();
    void addFolder(Folder* fi);
    void deleteFolderById(int id);
    void deleteFolder(Folder *fi);
    void markDeletedFolder(Folder *f);
    void markRestoredFolder(Folder *f);
    void markDeletedFolderById(int id);
    void markRestoredFolderById(int id);
    Folder* getFolder(int id);
    Folder* getFolderByName(QString folder);
    int getCount();
    void insertFolder(int id, Folder *fi);
};

#endif // CATALOG_H
