//
//  SyncGetClient.hpp
//  BaseHttp
//
//  Created by 胡欣妍 on 2018/10/31.
//  Copyright © 2018 胡欣妍. All rights reserved.
//

#ifndef SyncGetClient_hpp
#define SyncGetClient_hpp

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/steady_timer.hpp>

using boost::asio::ip::tcp;

class SyncGetClient {
protected:
    using error_code_type = boost::system::error_code;
    using results_type = tcp::resolver::results_type;
    using io_context_type = boost::asio::io_context;
    using resolver_type = tcp::resolver;
    using socket_type = tcp::socket;
    using streambuf_type = boost::asio::streambuf;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using steady_timer_type = boost::asio::steady_timer;
    using string = std::string;
    
    io_context_type context_;
    resolver_type resolver_;
    socket_type socket_;
    streambuf_type request_;
    streambuf_type response_;
    error_code_type err;
    
public:
    SyncGetClient();
    SyncGetClient(io_context_type& io_context);
    void get(const string &server, const string &path);
    void get(const string& server, const int port, const string& path);
    
protected:
    // 解析
    virtual void handle_resolve(const string &server);
    virtual void handle_resolve(const string &server, const int port);
    // 连接
    virtual void handle_connect(const results_type& endpoints, error_code_type& err);
    // 写入 request
    virtual void handle_write_request(error_code_type& err);
    // 读取response状态
    virtual void handle_read_status_line(error_code_type& err);
    // 读取response header
    virtual void handle_read_headers(std::istream& response_stream, error_code_type& err);
    // 读取response content
    virtual void handle_read_content(error_code_type& err);
};

#endif /* SyncGetClient_hpp */
