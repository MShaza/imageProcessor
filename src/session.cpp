#include "session.h"
HttpSession::HttpSession(boost::asio::ip::tcp::socket _socket):_socket(std::move(_socket)){}

void HttpSession::start(){
    doRead();
}

void HttpSession::doRead(){
    auto self = shared_from_this();
   http::async_read(_socket, buffer, req,
   [this, self](boost::beast::error_code ec, size_t bytes){
    if(!ec){
        std::cout << "[SESSION] Reading Bytes" << std::endl;
       self->onRead(ec, bytes); 
    }
   });
}
void HttpSession::onRead(boost::beast::error_code ec, size_t bytes){
    std::cout << "[DEBUG - HttpSession::onRead] Enter function"<< std::endl;
    if(req.method() == http::verb::get){
        textRes->result(http::status::ok);
        textRes->version(req.version());
        textRes->set(http::field::server, "Beast");
        textRes->set(http::field::content_type, "text/plain");
        textRes->body() = "Hello from Boost.Beast server!";
        textRes->prepare_payload();
        std::cout << "[SESSION] Preaparing Response" << std::endl;
    }

    else if((req.method()== http::verb::post) && (req.target()== "/upload")){
        std::cout << "[DEBUG - HttpSession::onRead] Post request recieved"<< std::endl;
        std::cout << "[DEBUG - HttpSession::onRead] Generating token"<< std::endl;
        std::string imageId = generateTransactionId();
        std::string imgFileName = imageId + "upload.jpg";
        property_tree::ptree jsonBody;
        std::ofstream MyFile(absolutePath+imgFileName,std::ios::binary);
        if(!MyFile){
            std::cerr << "Failed to open upload.jpg for writing\n";
            prepareImgFailRes("Failed to upload the image.");
        }
        MyFile.write(req.body().data(),req.body().size());
        MyFile.close();
        //std::cout << "Received image size: " << req.body().size() << std::endl;
         std::cout << "[DEBUG - HttpSession::onRead] Preparing response"<< std::endl;
        jsonBody.put("status", "sucess");
        jsonBody.put("message", "Image uploaded successfully");
        jsonBody.put("fileName", imgFileName);
        std::ostringstream buf;
        boost::property_tree::write_json(buf, jsonBody, false);
        std::string jsonStr = buf.str();
         //std::cout << "[DEBUG - HttpSession::onRead] Preparing response"<< std::endl;
        textRes = std::make_unique<http::response<http::string_body>>();
        textRes->result(http::status::ok);
        textRes->version(req.version());
        textRes->set(http::field::server, "Beast");
        textRes->set(http::field::content_type, "text/plain");
        textRes->body() = jsonStr;
        textRes->prepare_payload();
       
        
    } 
    else if(req.method()== http::verb::post){
        std::cout<<"[HttpSession::onRead] - Post Request hit"<<std::endl;
        std::stringstream ss(req.body());
        property_tree::ptree readJson;
        property_tree::read_json(ss, readJson);
        std::string imgPath = readJson.get<std::string>("fileName");
        size_t pos = imgPath.find("upload");
    
        std::string beforeUpload = imgPath.substr(0, pos);
        beforeUpload = absolutePath + beforeUpload;
        if(req.target()== "/grayscale"){
             bool sucess = ImageProcessor::processGray(absolutePath + imgPath, beforeUpload+"output.jpg");
        if(sucess){
            prepareImgSucessRes(beforeUpload+"output.jpg");
        }
        else{
            prepareImgFailRes("Failed to Process Image");
        }
        }
        else if(req.target()== "/resize"){
            int width = readJson.get<int>("width");
            int height = readJson.get<int>("height");
            bool sucess = ImageProcessor::processResize(absolutePath + imgPath, beforeUpload+"output.jpg", width, height);
        if(sucess){
            prepareImgSucessRes(beforeUpload+"output.jpg");
            }
        else{
            prepareImgFailRes("Failed to Process Image");
            }
        }
        else if(req.target() == "/filter"){
            std::cout<<"[HttpSession::onRead] - Filter Image"<<std::endl;
            if(readJson.get<std::string>("filterType") == "blur"){
                int width = readJson.get<int>("width");
                int height = readJson.get<int>("height");
                bool sucess = ImageProcessor::processBlur(absolutePath + imgPath, beforeUpload+"output.jpg", width, height);
                if(sucess){
                    prepareImgSucessRes(beforeUpload+"output.jpg");
                }
                else{
                    prepareImgSucessRes("Failed to Process Image");
                }
            }

        }
    }
    else if((req.method()== http::verb::post) && (req.target()== "/grayscale")){

    }
    else{
            textRes->result(http::status::bad_request);
            textRes->version(req.version());
            textRes->set(http::field::server, "Beast");
            textRes->set(http::field::content_type, "text/plain");
            textRes->body() = "Unsupported method.";
            textRes->prepare_payload();
        }
            doWrite(bytes);
            req = {};
            std::cout << "[DEBUG - HttpSession::onRead] function exit."<< std::endl;
}
void HttpSession::doWrite(std::size_t length){
    std::cout << "[DEBUG - HttpSession::doWrite] function enter."<< std::endl;
    auto self = shared_from_this();
    if(textRes){
          std::cout << "[DEBUG - HttpSession::doWrite] Prepare text response"<< std::endl;
   http::async_write(_socket, *textRes,
        [self](boost::beast::error_code ec, std::size_t bytes) {
          
            self->onWrite(self->textRes->need_eof(), ec, bytes);
            
        });
    }
    else if(imgRes){
          std::cout << "[DEBUG - HttpSession::doWrite] Prepare Img response"<< std::endl;
         http::async_write(_socket, *imgRes,
        [self](boost::beast::error_code ec, std::size_t bytes) {
            self->onWrite(self->imgRes->need_eof(), ec, bytes);
        });
    }
    std::cout << "[DEBUG - HttpSession::doWrite] Response send."<< std::endl;
     std::cout << "[DEBUG - HttpSession::doWrite] function exit"<< std::endl;
}
void HttpSession::onWrite(bool close, boost::beast::error_code ec, size_t bytes){
    if(ec) return;
     if (close) {
        return doClose();
     }
    textRes.reset();
    imgRes.reset();

    doRead();  // Handle next request (keep-alive)
}

void HttpSession::doClose() {
    boost::beast::error_code ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
}
void HttpSession::prepareImgSucessRes(string fileName){
    std::ifstream file(fileName,std::ios::binary);
    if(file){
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    imgRes = std::make_unique<http::response<http::vector_body<char>>>();
    imgRes->result(http::status::ok);
    imgRes->version(req.version());
    imgRes->set(http::field::server, "Beast");
    imgRes->set(http::field::content_type, "image/jpeg");
    imgRes->content_length(buffer.size());
    imgRes->keep_alive(req.keep_alive());
    imgRes->body() = std::move(buffer);
    imgRes->prepare_payload();
    std::cout << "[SESSION] Preaparing Response" << std::endl;
    }
    else{
        prepareImgFailRes("Failed to Process Image");
    }
}
void HttpSession::prepareImgFailRes(std::string responseData){
    textRes->result(http::status::internal_server_error);
    textRes->version(req.version());
    textRes->set(http::field::server, "Beast");
    textRes->set(http::field::content_type, "text/plain");
    textRes->body() = responseData;
    textRes->prepare_payload();
}