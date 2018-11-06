//
//  AsyncGetClient.cpp
//  BaseHttp
//
//  Created by 胡欣妍 on 2018/10/31.
//  Copyright © 2018 胡欣妍. All rights reserved.
//

#include "AsyncGetClient.hpp"
#include <QDebug>
#include <sstream>
AsyncGetClient::AsyncGetClient(io_context_type& io_context) : resolver_(io_context), socket_(io_context) {}

void AsyncGetClient::get(const string &server, const string &path) {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    resolver_.async_resolve(server, "http",
                            boost::bind(&AsyncGetClient::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::results));

}

void AsyncGetClient::get(const string &server, const int port, const string &path) {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server + ":" + std::to_string(port) << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // create endpoint
    endpoint_type ep(boost::asio::ip::address::from_string(server), port);
    resolver_.async_resolve(ep,boost::bind(&AsyncGetClient::handle_resolve, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::results));
}

void AsyncGetClient::handle_resolve(const error_code_type& err,
                                    const results_type& endpoints) {
    if (!err) {
        // Attempt a connection to each endpoint in the list until we
        // successfully establish a connection.
        boost::asio::async_connect(socket_, endpoints,
                                   boost::bind(&AsyncGetClient::handle_connect, this,
                                               boost::asio::placeholders::error));
    }
}

void AsyncGetClient::handle_connect(const boost::system::error_code& err) {
    if (!err) {
        boost::asio::async_write(socket_, request_,
                                 boost::bind(&AsyncGetClient::handle_write_request, this,
                                             boost::asio::placeholders::error));
    }
}

void AsyncGetClient::handle_write_request(const error_code_type& err) {
    if (!err) {
        // Read the response status line. The response_ streambuf will
        // automatically grow to accommodate the entire line. The growth may be
        // limited by passing a maximum size to the streambuf constructor.
        boost::asio::async_read_until(socket_, response_, "\r\n",
                                      boost::bind(&AsyncGetClient::handle_read_status_line, this,
                                                  boost::asio::placeholders::error));
    }
}

void AsyncGetClient::handle_read_status_line(const error_code_type& err) {
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
            std::cout << "Invalid response\n";
            return;
        }
        if (status_code != 200){
            std::cout << "Response returned with status code ";
            std::cout << status_code << "\n";
            return;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                      boost::bind(&AsyncGetClient::handle_read_headers, this,
                                                  boost::asio::placeholders::error));
    }
}

void AsyncGetClient::handle_read_headers(const boost::system::error_code& err) {
    if (!err) {
        // Process the response headers.
        std::istream response_stream(&response_);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&AsyncGetClient::handle_read_content, this,
                                            boost::asio::placeholders::error));
    }
}

void AsyncGetClient::handle_read_content(const boost::system::error_code& err) {
    if (!err) {
        // Write all of the data that has been read so far.

        // Continue reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&AsyncGetClient::handle_read_content, this,
                                            boost::asio::placeholders::error));
    }
}


