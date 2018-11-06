#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btn_async_clicked();

    void on_btn_sync_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
