#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QVariant>
#include <QDateTime>

class Link : public QObject
{
    Q_OBJECT

    QVariant id;
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
    Link(QObject *parent = nullptr);
    Link(QVariant id, QString link_name, QString link_real,
         QString description, QDateTime date,
         QObject *parent = nullptr);
    virtual ~Link(){}

    QVariant Id();

    QString linkName();
    void setlinkName(QString name);

    QString linkReal();
    void setlinkReal(QString link);


    QDateTime Date();
    void setDate(QDateTime date);

    QString Description();
    void setDescription(QString description);

};

#endif // LINK_H
