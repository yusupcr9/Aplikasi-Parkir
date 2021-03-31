#include <credentials.h>
#include <odoorpc.h>

void setup() {
  OdooRPC client (
    "http://localhost:8069",
    "Bioparks",
    {
      "yusup@gmail.com",
      "123456"
    }
);

std::cout << client.raw_query(
    "bioparks.read",
    "search_read",
    {
        "[]",                        // domain
        R"(["name", "pemilik_id"])",    // fields
        0,                           // offset
        5                            // limit
    }
) << std::endl;
}

void loop() {
  // put your main code here, to run repeatedly:

}
