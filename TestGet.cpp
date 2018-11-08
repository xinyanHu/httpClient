#include "TestGet.h"

TestGet::TestGet(io_context_type &io) : AsyncCustomGetClient(io) {
    init();
}

void TestGet::init() {
//    connect(this, &AsyncCustomGetClient::download_progress, this, &TestGet::hanle_download_progress);
    connect(this, &AsyncCustomGetClient::ready_read, this, &TestGet::handle_ready_read);
    connect(this, &AsyncCustomGetClient::error, this, &TestGet::handle_error);
    connect(this, &AsyncCustomGetClient::network_finished, this, &TestGet::handle_network_finished);
}

// error
void TestGet::handle_error(const int http_code, const std::string& message) {
    std::cerr << "err code: " << http_code << " message: " << message << std::endl;
}

// finiished
void TestGet::handle_network_finished(bool successed, const int code, QByteArray content) {
    if (successed && code == 200) {
        std::cerr << content.data();
    } else {
        std::cerr << "end with err" << std::endl;
    }
}

// download_porgress
//void TestGet::hanle_download_progress(qint64 recived, qint64 total) {

//}

// ready_read
void TestGet::handle_ready_read(streambuf_type& buffer) {
//    std::cerr << &buffer;
}
