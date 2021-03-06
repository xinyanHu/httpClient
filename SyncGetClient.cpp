//
//  SyncGetClient.cpp
//  BaseHttp
//
//  Created by 胡欣妍 on 2018/10/31.
//  Copyright © 2018 胡欣妍. All rights reserved.
//

#include "SyncGetClient.hpp"
#include <QDebug>

SyncGetClient::SyncGetClient(io_context_type& io_context) : resolver_(io_context), socket_(io_context) {}
SyncGetClient::SyncGetClient(): resolver_(context_), socket_(context_){}

void SyncGetClient::get(const string &server, const string &path) {
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    handle_resolve(server);
}

void SyncGetClient::get(const string& server, const int port, const string& path) {
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server + ":" + std::to_string(port) << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    handle_resolve(server, port);
}

void SyncGetClient::handle_resolve(const string &server) {
    // Get a list of endpoints corresponding to the server name.
    results_type endpoints = resolver_.resolve(server, "http", err);
    handle_connect(endpoints, err);
}

void SyncGetClient::handle_resolve(const string &server, const int port) {
    tcp::resolver::query query(server, std::to_string(port));
    results_type type = resolver_.resolve(query);
    handle_connect(type, err);
}

void SyncGetClient::handle_connect(const results_type& endpoints, error_code_type &err) {
    if (!err) {
        // Try each endpoint until we successfully establish a connection.
        boost::asio::connect(socket_, endpoints, err);
        handle_write_request(err);
    }
}

void SyncGetClient::handle_write_request(error_code_type &err) {
    if (!err) {
        // Send the request.
        boost::asio::write(socket_, request_, err);
        handle_read_status_line(err);
    }
}

void SyncGetClient::handle_read_status_line(error_code_type& err) {
    if (!err) {
        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::read_until(socket_, response_, "\r\n");
        // Check that response is OK.
        std::istream response_stream(&response_);
        string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cerr << "Invalid response" << std::endl;
            //        return 1;
        }
        if (status_code != 200) {
            std::cerr << "Response returned with status code " << status_code << std::endl;
        }
        handle_read_headers(response_stream, err);
    }
}

void SyncGetClient::handle_read_headers(std::istream& response_stream, error_code_type& err) {
    if (!err) {
        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket_, response_, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";
        handle_read_content(err) ;
    }
}

void SyncGetClient::handle_read_content(error_code_type &err) {
    if (!err) {
        while (boost::asio::read(socket_, response_,
                                 boost::asio::transfer_at_least(1), err)) {
                        std::cout << &response_;
        }
    }
}
