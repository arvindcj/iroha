#include <crow.h>

#include "http_server.hpp"

namespace http {
  void server() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Root";
    });

    app.port(1337).multithreaded().run();
  }
};  // namespace http