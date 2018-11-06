#ifndef ASYNCCUSTOMGETCLIENT_H
#define ASYNCCUSTOMGETCLIENT_H

#include <QObject>
#include <QtGlobal>
#include "AsyncGetClient.hpp"

class AsyncCustomGetClient : public QObject, public AsyncGetClient
{
    Q_OBJECT

public:
    AsyncCustomGetClient(io_context_type& io);

signals:
    void finished();
    void request_error(const error_code_type& code);
    void response_error(const string& error);
    void download_progress(qint64 recived, qint64 total);
    void ready_read(streambuf_type& buffer);

    void error(const int http_code, const std::string& message);
protected:
    unsigned long long recived_ = 0;
    unsigned long long total_ = 0;

    // 解析
    virtual void handle_resolve(const error_code_type& err,
                                const results_type& endpoints) override;
    // 连接
    virtual void handle_connect(const error_code_type& err) override;
    // 写入 request
    virtual void handle_write_request(const error_code_type& err) override;
    // 读取response状态
    virtual void handle_read_status_line(const error_code_type& err) override;
    // 读取response header
    virtual void handle_read_headers(const error_code_type& err) override;
    // 读取response content
    virtual void handle_read_content(const error_code_type& err) override;

public:
    void get(const string& server, const string& path) override;
    void get(const string& server, const int port, const string& path) override;

    // error
    void handle_request_error(const error_code_type& code);
    void handle_response_error(const string& error);

    void handle_error(const int http_code, const std::string& message);
    // finiished
    void handle_finished();
    // download_porgress
    void hanle_download_progress(qint64 recived, qint64 total);
    // ready_read
    void handle_ready_read(streambuf_type& buffer);
};

#endif // ASYNCCUSTOMGETCLIENT_H
