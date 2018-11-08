#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <boost/thread.hpp>
#include "TestGet.h"
#include "SyncGetClient.hpp"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_async_clicked()
{
    boost::asio::io_context io;
    TestGet client(io);
    std::string s = "{\"mac\" : \"00:1C:42:5D:8A:1D\", \"password\" : \"1\", \"username\" : \"李一\", \"version\" : \"\" }";
    client.post("192.168.1.134", 1340, "/core/client/gettoken", s);
    io.run();
}

void Widget::on_btn_sync_clicked()
{
    qDebug() << "-----------------------------sync start";
    try {
        boost::asio::io_context io;
        //            AsyncGetClient client1(io, "iso.mirrors.ustc.edu.cn",
        //                                               "/qtproject/archive/qt/5.8/5.8.0/qt-opensource-mac-x64-clang-5.8.0.dmg");
        boost::asio::io_context::work work(io);
        SyncGetClient client(io);
        client.get("127.0.0.1", 8080, "/");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    qDebug() << "-----------------------------sync end";
}
