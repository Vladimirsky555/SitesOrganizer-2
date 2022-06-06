#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QTime>
#include <QVariant>

class QSqlQuery;

class Data : public QObject
{
    Q_OBJECT

    QVariant id;
    QString catalog;
    QString folder;    
    QString link_name;
    QString link_real;

    QDateTime date;
    QString description;

public:
    bool isNew;
    bool isEdited;
    bool isDeleted;

    bool isMoved;
    bool isOpened;

public:
    Data(QObject *parent = nullptr);
    Data(QObject *parent, QSqlQuery &qry);
    virtual ~Data(){};

    QVariant Id();
    void setId(QVariant id);

    QString Catalog();
    void setCatalog(QString category);

    QString Folder();
    void setFolder(QString folder);

    QDateTime Date();
    void setDate(QDateTime date);

    QString linkName();
    void setlinkName(QString name);

    QString linkReal();
    void setlinkReal(QString link);

    QString Description();
    void setDescription(QString description);

};

#endif // DATA_H
