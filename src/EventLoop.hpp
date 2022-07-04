#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include <map>
#include <set>

#include "ASocket.hpp"
#include "Observer.hpp"

class EventLoop {
 public:
  EventLoop(const std::map<int, ASocket*>& fd2socket);
  ~EventLoop();

  void loop();

 private:
  EventLoop(const EventLoop& other);
  EventLoop& operator=(const EventLoop& other);

  Observer* observer_;
  std::map<int, ASocket*> fd2socket_;
  ;
};

#endif /* EVENTLOOP_HPP */
