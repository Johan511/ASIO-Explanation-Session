#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

boost::asio::io_context io_context;
tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8888));

void do_write(std::shared_ptr<boost::asio::ip::tcp::socket> virtualSocket,
              char *data, std::size_t length) {
  boost::asio::async_write(
      *virtualSocket, boost::asio::buffer(data, length),
      [virtualSocket, data](boost::system::error_code ec,
                            std::size_t /*length*/) {
        if (!ec) {
          virtualSocket->async_read_some(
              boost::asio::buffer(data, 1024),
              [virtualSocket, data](boost::system::error_code ec,
                                    std::size_t length) {
                if (!ec) {
                  do_write(virtualSocket, data, length);
                }
              });
        }
      });
}

void start_accept() {
  std::shared_ptr<boost::asio::ip::tcp::socket> virtualSocket(
      new boost::asio::ip::tcp::socket(io_context));
  acceptor.async_accept(*virtualSocket, [virtualSocket](auto... vn) {
    std::cout << "HI FROM CLIENT" << std::endl;

    char *data = new char[1024];

    virtualSocket->async_read_some(
        boost::asio::buffer(data, 1024),
        [virtualSocket, data](boost::system::error_code ec,
                              std::size_t length) {
          if (!ec) {
            do_write(virtualSocket, data, length);
          }
        });

    start_accept();
  });
}

int main() {
  start_accept();
  io_context.run();
}