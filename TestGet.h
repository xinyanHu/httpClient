#ifndef TESTGET_H
#define TESTGET_H

#include "AsyncCustomGetClient.h"

class TestGet : public AsyncCustomGetClient
{
public:
    TestGet(io_context_type& io);

protected:
    // error
    void handle_error(const int http_code, const std::string& message);

    // finiished
    void handle_network_finished(bool successed, const int code, QByteArray content);

    // download_porgress
//    void hanle_download_progress(qint64 recived, qint64 total);

    // ready_read
    void handle_ready_read(streambuf_type& buffer);

private:
    // init connect
    void init();
};

#endif // TESTGET_H
