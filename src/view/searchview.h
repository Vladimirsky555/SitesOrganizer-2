#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QWidget>

namespace Ui {
class SearchView;
}

class SearchView : public QWidget
{
    Q_OBJECT

public:
    explicit SearchView(QWidget *parent = nullptr);
    ~SearchView();

private slots:
    void on_edtText_returnPressed();

signals:
    void sendPattern(QString,bool);
    void changeMode(bool);


private:
    Ui::SearchView *ui;
};

#endif // SEARCHVIEW_H
