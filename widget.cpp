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
    try {
        boost::asio::io_context io;
        TestGet client1(io);
//        client1.get("iso.mirrors.ustc.edu.cn", "/qtproject/archive/qt/5.8/5.8.0/qt-opensource-mac-x64-clang-5.8.0.dmg");
        client1.get("127.0.0.1", 7000, "/platform/visualization/getTree");
//        client1.get("127.0.0.1", 7000, "/platform/visualization/getStationInfo?stationName=%E5%9B%BD%E4%BF%A1%E4%B8%B4%E6%B5%B7%E9%A3%8E%E7%94%B5&time=2018-11-07");
        io.run();
    } catch(std::exception& e) {
        std::cerr << "------" << e.what() << std::endl;
    }
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
        client.get("   ", 7000, "/platform/visualization/getTree");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    qDebug() << "-----------------------------sync end";
}
