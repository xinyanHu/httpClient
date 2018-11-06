#include "AsyncCustomGetClient.h"
#include <QDebug>


AsyncCustomGetClient::AsyncCustomGetClient(io_context_type &io) : AsyncGetClient(io) {
//    connect(this, &AsyncCustomGetClient::request_error, this, &AsyncCustomGetClient::handle_request_error);
    connect(this, &AsyncCustomGetClient::response_error, this, &AsyncCustomGetClient::handle_response_error);
    connect(this, &AsyncCustomGetClient::finished, this, &AsyncCustomGetClient::handle_finished);
    connect(this, &AsyncCustomGetClient::download_progress, this, &AsyncCustomGetClient::hanle_download_progress);
    connect(this, &AsyncCustomGetClient::ready_read, this, &AsyncCustomGetClient::handle_ready_read);

    connect(this, &AsyncCustomGetClient::error, this, &AsyncCustomGetClient::handle_error);
}

//--------------------------------------override------------------------------------------
void AsyncCustomGetClient::get(const string &server, const int port, const string &path) {
    try {
        AsyncGetClient::get(server, port, path);
    } catch(std::exception& e) {
        emit response_error(e.what());
    }
}

void AsyncCustomGetClient::get(const string& server, const string& path) {
    try {
        AsyncGetClient::get(server, path);
    } catch(std::exception& e) {
        emit response_error(e.what());
    }
}

void AsyncCustomGetClient::handle_resolve(const error_code_type& err,
                                          const results_type& endpoints) {
    if(err) {
         emit error(err.value(), err.message());
    }
    AsyncGetClient::handle_resolve(err, endpoints);
}

void AsyncCustomGetClient::handle_connect(const error_code_type& err) {
    if(err) {
         emit error(err.value(), err.message());
    }
    AsyncGetClient::handle_connect(err);
}

void AsyncCustomGetClient::handle_write_request(const error_code_type& err) {
    if(err) {
         emit error(err.value(), err.message());
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
//            emit response_error("Invalid response");
            return;
        }
        if (status_code != 200){
            emit error(status_code, "Response returned with status code ");
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
            emit finished();
        } else  {
            emit error(err.value(), err.message());
        }
    }

}

//--------------------------------------slot------------------------------------------
void AsyncCustomGetClient::handle_request_error(const error_code_type& code) {
    std::cerr << "------AsyncCustomGetClient::Request Error: " << code.message() << std::endl;
//    emit finished();
}

void AsyncCustomGetClient::handle_response_error(const string& error) {
    std::cerr << "------AsyncCustomGetClient::Response Error: " << error << std::endl;
//    emit finished();
}

void AsyncCustomGetClient::handle_error(const int http_code, const std::string& message) {
    std::cerr << "error code: " << http_code << " message: " << message << std::endl;
}

void AsyncCustomGetClient::handle_finished() {
    qDebug() << "request finished";
}

void AsyncCustomGetClient::hanle_download_progress(qint64 recived, qint64 total) {
    std::cerr << recived << " / " << total << std::endl;
}

void AsyncCustomGetClient::handle_ready_read(streambuf_type& buffer) {
    std::stringstream ss;
    ss << &buffer;
    string s;
    ss >> s;
//    qDebug() << s.data();
}