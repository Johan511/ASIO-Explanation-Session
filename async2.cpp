#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

boost::asio::io_context io_context;
tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8888));

void start_accept() {
  std::shared_ptr<boost::asio::ip::tcp::socket> virtualSocket(
      new boost::asio::ip::tcp::socket(io_context));
  acceptor.async_accept(*virtualSocket, [virtualSocket](auto... vn) {
    std::cout << "HI FROM CLIENT" << std::endl;
    virtualSocket->write_some(boost::asio::buffer("HELLO FROM SERVER"));
    delete virtualSocket;
    start_accept();
  });
}

int main() {
  start_accept();
  io_context.run();
}
