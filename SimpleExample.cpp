#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
int main()
{
    boost::system::error_code ec;

    // Create a context - Unique instance of asio
    boost::asio::io_context context;

    // Get the address of somewhere we wish to connect to
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("93.184.216.34", ec), 80);

    // Create a socket
    // boost::asio::ip::tcp::socket socket(context);
    // socket.connect(endpoint, ec);

    boost::asio::ip::tcp::resolver res(context);
    boost::asio::ip::tcp::socket socket(context);
    // ws-feed.exchange.coinbase.com
    boost::asio::connect(socket, res.resolve("ws-feed.exchange.coinbase.com", std::to_string(80)));
    // boost::asio::connect(socket, res.resolve("ws-feed.prime.coinbase.com", std::to_string(80)));

    // if(!ec)
    // {
    //     std::cout << "Connected!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    // }

    if(socket.is_open())
    {
        std::cout << "Success!" << std::endl;

        json request;
        // {
        //     "type": "subscribe",
        //     "product_ids": [
        //         "ETH-USD",
        //         "BTC-USD"
        //     ],
        //     "channels": ["level2"]
        // }

        request["type"] = "subscribe";
        request["product_ids"] = {"ETH-USD", "BTC-USD"};
        request["channels"] = {"level2"};

        std::string sRequest = request.dump();
        // std::cout << sRequest.length() << std::endl;

        boost::asio::streambuf req;
        std::ostream request_stream(&req);
        request_stream << "POST /title/ HTTP/1.1 \r\n";
        request_stream << "Host:" << "ws-feed.exchange.coinbase.com" << "\r\n";
        request_stream << "User-Agent: C/1.0\r\n";
        request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Content-Length: " << sRequest.length() << "\r\n";    
        request_stream << "Connection: close\r\n\r\n";  //NOTE THE Double line feed
        request_stream << sRequest;

        boost::system::error_code write_ec;
        boost::asio::write(socket, req);

        if(!write_ec)
        {
            std::cout << "Successful Write" << std::endl;
        }
        else
        {
            std::cout << "Failed to write\n" << write_ec.message() << std::endl;
        }

        // std::string sRequest = 
        //     "GET /index.html HTTP/1.1\r\n"
        //     "Host: example.com \r\n"
        //     "Connection: close\r\n\r\n";

        // // send request
        // socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()), ec);

        // // check if bytes available
        // size_t bytes = socket.available();

        // std::cout << "Bytes available: " << bytes << std::endl;
        // if (bytes > 0)
        // {
        //     std::vector<char> vBuffer(bytes);
        //     socket.read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()), ec);
        // }
    }

    return 0;
}