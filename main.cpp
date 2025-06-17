#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <thread>

int main(){
    try{
    boost::asio::io_context io_context;

    auto server = std::make_shared<HttpServer>(io_context, 8080);
    server ->startAccept();
    const std::size_t  thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    // Run in a separate thread so we can wait for input
    std::cout<<"[Main] starting "<< thread_count<< "Working thread."<< std::endl;
    for(std::size_t i = 0; i < thread_count; i++){
        threads.emplace_back([&io_context](){
            io_context.run();
        });
    }
    //std::thread io_thread([&]() {
    //io_context.run();
    //});

    std::cout << "[SERVER] Press ENTER to stop the server..." << std::endl;
    std::cin.get();  

    io_context.stop();
    for(auto &t : threads){
        if(t.joinable()) t.join();
    }
    std::cout << "[SERVER] Server stopped..." << std::endl;
// Gracefully stop the io_context
    } catch(std::exception &e){
        std::cerr<<"Exception: "<<e.what()<<std::endl;
    }
}