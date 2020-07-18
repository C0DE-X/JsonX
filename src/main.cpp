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


  jsonx::Stringify stringify;
  std::cout<< stringify(jObj) << std::endl; 

  jsonx::Writer writer;
  writer(jObj, "output.json");

  return EXIT_SUCCESS;
}
