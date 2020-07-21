#include <iostream>
#include <jsonx/JsonObject.h>
#include <jsonx/JsonX.h>

int main() {

  jsonx::Object jObj;
  jObj["hello"]["bla"] = "hello world";
  jObj["blub"] = 3.0;
  jObj["test"] = 34.543;
  jObj["object"]["name"] = "a name";
  jObj["object"]["descision"] = true;
  jObj["object"]["players"] = 64;
  jObj["list"][0] = 64;
  jObj["list"][1] = false;
  jObj["list"][2] = "hello list";
  jObj["list"][4] = "end of list";
  jObj["none"] = nullptr;


  std::string buffer = jsonx::stringify(jObj);

  std::cout << buffer << std::endl;

  jsonx::write(jObj, "output.json");

  auto obj = jsonx::objectify(buffer);

  if (obj) {
    std::cout << "Parsed object is:\n";
    std::cout << jsonx::stringify(*obj) << std::endl;
  } else {
    std::cout << "Invalid Object" << std::endl;
  }

  obj = "{"
      "\"somekey\": \"textvalue\","
      "\"somevalue\": 234.3,"
      "\"morevalue\":35, "
      "\"Test\":true,"
      "\"list\":[ true, 35.6 ] "
    "}"_jsonx;

  if (obj) {
    std::cout << "Parsed object is:\n";
    std::cout << jsonx::stringify(*obj) << std::endl;
  } else {
    std::cout << "Invalid Object" << std::endl;
  }

  obj = jsonx::read("output.json");

  if (obj) {
    std::cout << "Parsed object is:\n";
    std::cout << jsonx::stringify(*obj) << std::endl;
  } else {
    std::cout << "Invalid Object" << std::endl;
  }

  return EXIT_SUCCESS;
}