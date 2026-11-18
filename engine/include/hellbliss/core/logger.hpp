#pragma once

#include <iostream>
#include <string>

namespace hellbliss::core {
  class Logger {
  public:
    using Handler = void(*)(const std::string&);

    static void set_handler(Handler handler) {
      _handler = std::move(handler);
    }
    static void log(const std::string& log) {
      if (_handler) _handler(log);
    }

  private:
    inline static Handler _handler = [](const std::string& log) {
      std::cout << log;
    };
  };
}
