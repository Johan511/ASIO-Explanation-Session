#include <boost/asio.hpp>
#include <iostream>
// https://www.youtube.com/watch?v=rwOv_tw2eA4&list=PLjd5OcY5hdAt3JOaFMClZVD75KmQ_sMnO

/*(A)
Work1
Work2
Work3
Done
Timer
*/

/*(B)
Work1
Work2
Work3
Timer
Done
*/

/*(C)
Work1
Timer
Work2
Work3
Done
*/

int main() {
  boost::asio::io_service service;
  // creates a queue where work can be pushed to

  service.post([] { std::cout << "Work 1\n"; });

  boost::asio::deadline_timer timer1(service, boost::posix_time::seconds(1));

  // // once wait is done queues to the service
  timer1.async_wait([&](auto... vn) { std::cout << "timer\n"; });

  service.post([] { std::cout << "Work 2\n"; });
  service.post([] { std::cout << "Work 3\n"; });

  service.run();

  /*
    Strands  => only one effect handler happens at a given time, wrap service in
    a strad and use the strad
  */

  std::cout << "Done\n";
}
