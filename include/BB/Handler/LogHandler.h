#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <iostream>
#include <time.h>

namespace bb {
  enum LogType {
    ERR, WRN, INF
  };
  class LogHandler {
  public:
    template<typename T>
    static void log(std::string message, LogType type = INF) {
      time_t now = time(0);
      struct tm tstruct;
      char buf[80];
      localtime_s(&tstruct, &now);
      strftime(buf, sizeof(buf), "%X", &tstruct);
      std::cerr << buf << " ";
      switch (type) {
        case ERR:
          std::cerr << "[ERROR]   ";
          break;
        case WRN:
          std::cerr << "[WARNING] ";
          break;
        case INF:
          std::cerr << "[INFO]    ";
          break;
      }
      std::cerr << message.c_str() << ". ";
      std::cerr << "(Called by " << typeid(T).name() << ")" << std::endl;
    }
  private:

  };
}

#endif