#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSqlDatabase>

class QTranslator;


class Application : public QApplication
{
    Q_OBJECT

    QSqlDatabase db;
    QTranslator *pTranslator;

public:
    Application(int argc, char *argv[]);
    virtual ~Application(){};

    void connectToDataBase();

    // Внутренние методы для работы с базой данных
private:
    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание таблицы в базе данных
};

#endif // APPLICATION_H
