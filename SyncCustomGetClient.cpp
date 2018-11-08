#include "SyncCustomGetClient.h"

SyncCustomGetClient::SyncCustomGetClient(io_context_type &io) : SyncGetClient(io){

}

// 解析
void SyncCustomGetClient::handle_resolve(const string &server) {

}
void SyncCustomGetClient::handle_resolve(const string &server, const int port) {

}
// 连接
void SyncCustomGetClient::handle_connect(const results_type& endpoints, error_code_type& err) {

}
// 写入 request
void SyncCustomGetClient::handle_write_request(error_code_type& err) {

}
// 读取response状态
void SyncCustomGetClient::handle_read_status_line(error_code_type& err) {

}
// 读取response header
void SyncCustomGetClient::handle_read_headers(std::istream& response_stream, error_code_type& err) {

}
// 读取response content
void SyncCustomGetClient::handle_read_content(error_code_type& err) {

}
