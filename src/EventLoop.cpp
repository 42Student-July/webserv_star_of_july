#include "EventLoop.hpp"

// EventLoop::EventLoop() : observer(new Observer) {}
EventLoop::EventLoop(const std::map<int, ASocket *> &fd2socket)
    : observer_(new Observer), fd2socket_(fd2socket) {
  std::map<int, ASocket *>::iterator it = fd2socket_.begin();
  std::map<int, ASocket *>::iterator ite = fd2socket_.end();
  for (; it != ite; it++) {
    observer_->addTargetReadFd(it->first);
  }
}

EventLoop::~EventLoop() { delete observer_; }

void EventLoop::loop() {
  while (true) {
    observer_->notifyFdEvent();
    std::set<int> readyfds = observer_->getReadyFds();
    std::set<int>::iterator it = readyfds.begin();
    std::set<int>::iterator ite = readyfds.end();
    for (; it != ite; it++) {
      fd2socket_[*it]->notifyFdEvent(observer_, &fd2socket_);
      // if (fd2socket_[*it]->getState() == CREATE) {
      //   ;
      //   ;
      // } else if (~~ == CLOSE) {
      // delete fd2socket_[*it];
      // fd2socket_.erase(*it);
      // }
    }
  }
}

// EventLoop::EventLoop(const EventLoop& other) {
//   *this = other;
// }

// EventLoop& EventLoop::operator=(const EventLoop& other) {
//   if (this != &other) {
//   }
//   return *this;
// }
