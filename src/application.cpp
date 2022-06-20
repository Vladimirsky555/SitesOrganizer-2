#include "application.h"

#include <QtSql>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTranslator>


Application::Application(int argc, char *argv[]) :
    QApplication(argc, argv)
{
    connectToDataBase();

    pTranslator = new QTranslator(this);
    pTranslator->load(":lan/rus");
    installTranslator(pTranslator);
}

void Application::connectToDataBase()
{
    // Перед подключением к базе данных производим проверку на её существование.
    // В зависимости от результата производим открытие базы данных или её восстановление
    if(!QFile("data.db").exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

//База данных открывается по заданному пути и имени базы данных, если она существует
bool Application::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("linksDB");
    db.setDatabaseName("data.db");
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

bool Application::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        QMessageBox::critical(NULL, "Сообщение!",
                              "Не удалось восстановить базу данных!");
        return false;
    }
    return false;
}

void Application::closeDataBase()
{
    db.close();
}

bool Application::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE  linksDB  ("
                    "id           INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "catalog      VARCHAR(255),"
                    "folder       VARCHAR(255),"
                    "date         TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                    "link_name    VARCHAR(255),"
                    "link_real    VARCHAR(255),"
                    "description  TEXT"
                    " )"
                    )){
        QMessageBox::critical(NULL, "Сообщение!",
                              "Ошибка создания базы данных! " +
                              query.lastError().text());

        return false;
    } else {
        return true;
    }
    return false;
}
