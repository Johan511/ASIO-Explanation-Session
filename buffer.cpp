#include <boost/asio.hpp>
#include <iostream>
#include <string>
// https://www.youtube.com/watch?v=rwOv_tw2eA4&list=PLjd5OcY5hdAt3JOaFMClZVD75KmQ_sMnO

/*
Acceptor => it creates resources for the socket in the OS
*/

using namespace boost;
int main() {
  boost::asio::io_service service;

  boost::asio::ip::tcp::acceptor acceptor(service);

  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 1234);

  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();

  while (1) {
    boost::asio::ip::tcp::socket virtualSocket(service);

    acceptor.accept(virtualSocket);
    boost::asio::streambuf buf;
    boost::asio::read_until(virtualSocket, buf, "\n");

    std::cout << boost::asio::buffer_cast<const char *>(buf.data())
              << std::endl;

    boost::asio::write(virtualSocket, boost::asio::buffer("abc"));
  }
}
