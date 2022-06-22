#include <iostream> 
#include <cbpro++/auth.h> 
#include <cbpro++/marketdata/products.h> 

int main(int argc, char **argv) {
    // key, secret, passphrase
    Auth auth(argv[0], argv[1], argv[2], Auth::Mode::SANDBOX);

    auto book = marketdata::products::getOrderBookLevelThree(auth, "BTC-USD");
    auto bids = book.getBids();

    for (auto &bid : bids) {
        if (std::stoi(bid.getSize()) > 5) {
            std::cout << bid.getOrderId() << std::endl; 
        }
    } 

    return 0; 
}