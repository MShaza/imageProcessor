#include <iostream>
#include <boost/asio.hpp>
#include "session.h"

class HttpServer : public std::enable_shared_from_this<HttpServer>
{
    public:
        void startAccept();
        HttpServer(boost::asio::io_context &io_context, int port);
    private:
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::ip::tcp::socket _socket;
        void handleAccept();
};