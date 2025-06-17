#include "generateId.h"

std::string generateTransactionId(){
    std::cout << "[DEBUG - generateTransactionId] Enter function"<< std::endl;
    std::ostringstream oss;
    //std::srand(std::time(nullptr));
    //oss<<"TRANX"<<std::rand() % 100000;
    std::random_device rd;                  // Non-deterministic random number generator (uses hardware entropy)
    std::mt19937 gen(rd());                 // Mersenne Twister engine (high-quality PRNG), seeded with `rd`
    std::uniform_int_distribution<> dis(10000, 99999); // Uniform distribution between 10,000 and 99,999
    oss << "TRANX" << dis(gen);
    std::cout << "[DEBUG - generateTransactionId] Id generated" << std::endl;
    std::cout << "[DEBUG - generateTransactionId] Exit function successfully"<< std::endl;
    return oss.str();
}