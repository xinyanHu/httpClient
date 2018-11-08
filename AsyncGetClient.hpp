//
//  AsyncGetClient.hpp
//  BaseHttp
//
//  Created by 胡欣妍 on 2018/10/31.
//  Copyright © 2018 胡欣妍. All rights reserved.
//

#ifndef AsyncGetClient_hpp
#define AsyncGetClient_hpp

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class AsyncGetClient {
protected:
    using error_code_type = boost::system::error_code;
    using results_type = tcp::resolver::results_type;
    using io_context_type = boost::asio::io_context;
    using resolver_type = tcp::resolver;
    using socket_type = tcp::socket;
    using streambuf_type = boost::asio::streambuf;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using string = std::string;

    //    io_context_type context_;
    resolver_type resolver_;
    socket_type socket_;
    streambuf_type request_;
    streambuf_type response_;

    // 解析
    virtual void handle_resolve(const error_code_type& err,
                                const results_type& endpoints);
    // 连接
    virtual void handle_connect(const error_code_type& err);
    // 写入 request
    virtual void handle_write_request(const error_code_type& err);
    // 读取response状态
    virtual void handle_read_status_line(const error_code_type& err);
    // 读取response header
    virtual void handle_read_headers(const error_code_type& err);
    // 读取response content
    virtual void handle_read_content(const error_code_type& err);

public:
    AsyncGetClient(boost::asio::io_context& io_context);

    //  www.boost.org /LICENSE_1_0.txt
    virtual void get(const string& server, const string& path);
    // 127.0.0.1  8089 /SDXNY/tdfdscyb/getAllDq\?dq=1
    virtual void get(const string& server, const int port, const string& path);

    virtual void post(const string& server, const int port, const string& path, const string& content);
};

#endif /* AsyncGetClient_hpp */
