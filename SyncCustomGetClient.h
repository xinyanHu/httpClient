#ifndef SYNCCUSTOMGETCLIENT_H
#define SYNCCUSTOMGETCLIENT_H

#include <QObject>
#include <QtGlobal>
#include <QByteArray>
#include "SyncGetClient.hpp"

class SyncCustomGetClient : public QObject, public SyncGetClient
{
public:
    SyncCustomGetClient(io_context_type& io);

signals:
    void finished(bool successed, const int code, streambuf_type& buffer);
    void network_finished(bool successed, const int code, QByteArray content);
    void download_progress(qint64 recived, qint64 total);
    void ready_read(streambuf_type& buffer);
    void error(const int http_code, const std::string& message);

protected:
    // 解析
//    virtual void handle_resolve(const string &server) override;
//    virtual void handle_resolve(const string &server, const int port) override;
//    // 连接
//    virtual void handle_connect(const results_type& endpoints) override;
//    // 写入 request
//    virtual void handle_write_request() override;
//    // 读取response状态
//    virtual void handle_read_status_line() override;
//    // 读取response header
//    virtual void handle_read_headers(std::istream& response_stream) override;
//    // 读取response content
//    virtual void handle_read_content() override;
};

#endif // SYNCCUSTOMGETCLIENT_H
