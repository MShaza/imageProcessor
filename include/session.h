#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <fstream>
#include "imageProcessor.h"
#include "generateId.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

const std::string absolutePath = "/Users/muhammadshazab/Documents/C++ Projects/Image Processor/media/";
namespace http = boost::beast::http;
namespace property_tree = boost::property_tree;
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
    public:
    HttpSession(boost::asio::ip::tcp::socket _socket);
    void start();
    void doRead();
    void onRead(boost::beast::error_code ec, size_t bytes );
    void doWrite(std::size_t length);
    void onWrite(bool close, boost::beast::error_code ec, size_t bytes);
    void doClose();
    void prepareImgSucessRes(string fileName);
    void prepareImgFailRes(std::string responseBody);

    private:
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer buffer;
    http::request<http::string_body> req;
    std::unique_ptr<http::response<http::string_body>> textRes;
    std::unique_ptr<http::response<http::vector_body<char>>> imgRes;

};