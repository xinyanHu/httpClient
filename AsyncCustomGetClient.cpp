#include "AsyncCustomGetClient.h"
#include <QDebug>


AsyncCustomGetClient::AsyncCustomGetClient(io_context_type &io) : AsyncGetClient(io) {
    connect(this, &AsyncCustomGetClient::finished, this, &AsyncCustomGetClient::handle_finished);
//    connect(this, &AsyncCustomGetClient::download_progress, this, &AsyncCustomGetClient::hanle_download_progress);
//    connect(this, &AsyncCustomGetClient::ready_read, this, &AsyncCustomGetClient::handle_ready_read);
//    connect(this, &AsyncCustomGetClient::error, this, &AsyncCustomGetClient::handle_error);
//    connect(this, &AsyncCustomGetClient::network_finished, this, &AsyncCustomGetClient::handle_network_finished);
}

//--------------------------------------override------------------------------------------
void AsyncCustomGetClient::get(const string &server, const int port, const string &path) {
    try {
        AsyncGetClient::get(server, port, path);
    } catch(std::exception& e) {
        emit error(-1, e.what());
        emit finished(false, -1, response_);
    }
}

void AsyncCustomGetClient::get(const string& server, const string& path) {
    try {
        AsyncGetClient::get(server, path);
    } catch(std::exception& e) {
        emit error(-1, e.what());
        emit finished(false, -1, response_);
    }
}

void AsyncCustomGetClient::post(const string& server, const int port, const string& path, const string& content) {
    try {
        AsyncGetClient::post(server, port, path, content);
    } catch(std::exception& e) {
        emit error(-1, e.what());
        emit finished(false, -1, response_);
    }
}

void AsyncCustomGetClient::handle_resolve(const error_code_type& err,
                                          const results_type& endpoints) {
    if(err) {
         emit error(err.value(), err.message());
         emit finished(false, err.value(), response_);
    }
    AsyncGetClient::handle_resolve(err, endpoints);
}

void AsyncCustomGetClient::handle_connect(const error_code_type& err) {
    if(err) {
         emit error(err.value(), err.message());
         emit finished(false, err.value(), response_);
    }
    AsyncGetClient::handle_connect(err);
}

void AsyncCustomGetClient::handle_write_request(const error_code_type& err) {
    if(err) {
         emit error(err.value(), err.message());
         emit finished(false, err.value(), response_);
    }
    AsyncGetClient::handle_write_request(err);
}

void AsyncCustomGetClient::handle_read_status_line(const error_code_type &err) {
    if (!err) {
        // Check that response is OK.
        std::istream response_stream(&response_);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/"){
            emit error(-2, "Invalid response");
            emit finished(false, status_code, response_);

            socket_.close();
            return;
        }
        if (status_code != 200){
            emit error(status_code, "Response returned with status code " + std::to_string(status_code));
            emit finished(false, status_code, response_);

            socket_.close();
            return;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                      boost::bind(&AsyncCustomGetClient::handle_read_headers, this,
                                                  boost::asio::placeholders::error));
    } else {
        emit error(err.value(), err.message());
    }
}

void AsyncCustomGetClient::handle_read_headers(const error_code_type& err) {
    if(err) {
         emit error(err.value(), err.message());
    }
    if (!err) {
        // Process the response headers.
        std::istream response_stream(&response_);
        string header;
        while (std::getline(response_stream, header) && header != "\r") {
            if (header.substr(0, 14) == "Content-Length") {
                string total_size = header.substr(16, header.size() - 1);
                total_ = std::stoul(total_size);
            }
        }

        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&AsyncCustomGetClient::handle_read_content, this,
                                            boost::asio::placeholders::error));
    }
}

void AsyncCustomGetClient::handle_read_content(const error_code_type& err) {
    if (!err) {
        // count recived
        recived_ += response_.size();
        if (total_ != 0) {
            emit download_progress(recived_, total_);
        }

        // Write all of the data that has been read so far.
        emit ready_read(response_);

        // Continue reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&AsyncCustomGetClient::handle_read_content, this,
                                            boost::asio::placeholders::error));
    } else {
        if (err == boost::asio::error::eof) {
            emit finished(true, 200, response_);
        } else  {
            emit error(err.value(), err.message());
            emit finished(false, 200, response_);
        }
    }

}

//--------------------------------------slot------------------------------------------
//void AsyncCustomGetClient::handle_error(const int http_code, const std::string& message) {
//    // -1 exception
//    // -2 Invalid response
//    std::cerr << "error code: " << http_code << " message: " << message << std::endl;
//}

void AsyncCustomGetClient::handle_finished(bool successed, const int code, streambuf_type& buffer) {
    std::stringstream ss;
    ss << &buffer;
    string s(ss.str());

    emit network_finished(successed, code, QByteArray(s.data()));
}

//void AsyncCustomGetClient::handle_network_finished(bool successed, const int code, QByteArray content) {

//}

//void AsyncCustomGetClient::hanle_download_progress(qint64 recived, qint64 total) {
//    std::cerr << recived << " / " << total << std::endl;
//}

//void AsyncCustomGetClient::handle_ready_read(streambuf_type& buffer) {

//}
