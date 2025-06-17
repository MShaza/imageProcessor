#include "server.h"

HttpServer::HttpServer(boost::asio::io_context &io_context, int port )
    : _acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    _socket(io_context){
        std::cout << "Server started on port " << port << std::endl;
}

void HttpServer::startAccept(){
    auto self = shared_from_this();
        _acceptor.async_accept(
            [self](boost::system::error_code ec, boost::asio::ip::tcp::socket socket ){
                if(!ec){
                     std::cout << "[SERVER] Request Recieved" << std::endl;
                    std::make_shared<HttpSession>(std::move(socket))->start();
                }
                else{
                    self->startAccept();
                }
            }
        );
}