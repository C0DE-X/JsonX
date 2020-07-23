# JsonX

A small Json-library to read/write and handle json-objects

Every jsontype is hold by a jsonobject.  
To simple create an empty jsonobject use following syntax:
```cpp
  jsonx::Object jObj;
```

## Loading json

A json object can be read from file or via string. The jsonx::read function can be used to read from a file. To use a string as initialization the method jsonx::objectify or the literal _jsonx can be used. The output is an optional variable. In case there is an error of the input this object is not valid.

```cpp
std::optional<jsonx::Object> jObj;

std::string jsonString = "{"
      "\"somekey\": \"textvalue\","
      "\"somevalue\": 234.3,"
      "\"morevalue\":35, "
      "\"someboolean\":true,"
      "\"list\":[ true, 35.6 ] "
      "}";

//from file
jObj = jsonx::read("object.json");

//objectify
jObj = jsonx::objectify(jsonString);

//string literal
jObj ="{"
      "\"somekey\": \"textvalue\","
      "\"somevalue\": 234.3,"
      "\"morevalue\":35, "
      "\"someboolean\":true,"
      "\"list\":[ true, 35.6 ] "
      "}"_jsonx;
```
## Saving json

A json object can be written to a file or a string. The jsonx::write function can be used to write to file. To use a string as destination the method jsonx::stringify can be used. Also the streaming operator is overloaded to stream a json object.

```cpp
jsonx::Object jObj;

//to file
jsonx::write(jObj, "output.json");

//objectify
std::string json = jsonx::stringify(jObj);

//streaming operator
std::cout<< jObj << std::endl;
```
## Assigning values

The jsonobject automatically transforms to the type by assigning a value. The preused type gets overwritten.

```cpp
  jsonx::Object jObj;      //object

  jObj = true;             //boolean
  jObj = 3;                //number
  jObj = 3.5;              //number
  jObj = "Hello World!";   //string
  jObj = jsonx::null;      //null
  jObj = jsonx::Object();  //object
  jObj = jsonx::Array();   //array
```

Values can also be directly set via keypath or index. In case an index is set the object automatically gets transformed to an array or in case of a key to an object. If an index doesn't get set it is automatically set to an empty jsonobject, as in the follwing example. Index 0,1,3 are set and 2 is left so it is an empty object.

```cpp
  jsonx::Object jObj;

  jObj["Person"]["Adress"]["Street"]= "SomeStreet";
  jObj["Person"]["Name"]["First"]= "John";
  jObj["Person"]["Name"]["Second"]= "Doe";
  jObj["Person"]["Age"]= 43;
  jObj["Pets"][0] = "cat";
  jObj["Pets"][1] = "dog";
  jObj["Pets"][3] = "mouse";

```
Output:
```json
{
  "Person":{
    "Adress":{
      "Street":"SomeStreet"
    },
    "Age":43,
    "Name":{
      "First":"John",
      "Second":"Doe"
    }
  },
  "Pets":["cat","dog",{},"mouse"]
}
```

## Access type object

If the current object is an object type. There are different types of operators and functions that can be used to access its properties.

- index operator with std::string
  
  The index operator can be used to read or modify a json object. In case the const operator is used to read the value, either the object is an object type and the key exists or an emtpy object gets returned.  
  If the non-const version is used the object gets automatically converted to a type object and either the key exists and its value is returned or it gets created with an empty object that gets returned.
  This operator is also used to assign a value to a key. In case the current object is not an object type it gets automatically converted to object and the value is set to the given key. If a key exists can easily be done by calling the exists function.

  ```cpp
  jsonx::Object jObj; //empty object
  jObj["SomeKey"] = 3.5; //assign 3.5 to property "SomeKey"
  json::Object numberObject = jObj["SomeKey"]; //assign object of "SomeKey"
  ```
- void add(std::string const& key, Object const& obj)

  This function is an equivalent to the index operator and also turns the object into an object type in case it is any other type.

- void remove(std::string const &key)

  To remove a property from an object this function can be called. It only removes the key in case the key exists and the object's type is object.

- std::vector<std::string> keys() const
  
  This function returns a vector of all keys hold by the object. In case the current object is not an object type the list is empty.

  ```cpp
  for(std::string& key : jObj.keys())   // iterate over all keys
    std::cout<< jObj[key] << std::endl; //prints all property values of jObj
  ```
- range based loop
    
  The for loop can be used to iterate over an object and access its properties.
  ```cpp
  for(auto& prop : jObj)   // iterate over all properties
    std::cout<< prop.first << ":" << prop.second << std::endl; //prints key:value
  ```
- size_t count() const
    
  It returns the count of the keys in case the object is of type object, otherwise 0;

## Access type vector
If the current object is an array type. There are different types of operators and functions that can be used to access its properties.

- index operator with int 
  
  The index operator can be used to read or modify a json array. In case the const operator is used to read the value, either object is an array and the index exists or an emtpy object gets returned.  
  If the non-const version is used the object gets automatically converted to type array and either the index exists and its value is returned or it gets created with an empty object that gets returned.
  This operator is also used to assign a value to a key. In case the current object is not an array type it gets automatically converted to array and the value is set to the given index. If an index gets without assigning values to smaller indices the list gets expanded to the current index and empty object are added.

  ```cpp
  jsonx::Object jObj; //empty object
  jObj[0] = 3.5; //3.5 at index 0
  jObj[2] = "Hello World"; //"Hello World" at index 2
  //--> jObj[1] = jsonx::Object();
  json::Object numberObject = jObj[0]; //assign object from index 0
  ```
- void add(Object const& obj)

  This function adds the given object to the end of the array. In case the object is not an array type it gets automatically converted.

- void remove(int const &index)

  To remove an entry from an array this function can be called. It only removes the object at the given index in case the array is large enough and the object's type is array.

- std::vector<int> sequence() const
  
  This function returns a vector with all indices. In case the current object is not an array type the list is empty. E.g. the size of the array is 5, this list will hold {0,1,2,3,4};

  ```cpp
  for(int index : jObj.sequence())   // iterate over all keys
    std::cout<< jObj[index] << std::endl; //prints all entries of jObj
  ```
- size_t count() const
    
  It returns the size of the array in case the object is of type array, otherwise 0;

## Object types

The jsonx library covers and handles all possible json datatypes and values:
  - Object &rarr; jsonx::Object::Type::Object
  - Array &rarr; jsonx::Object::Type::ARRAY
  - Number &rarr; jsonx::Object::Type::NUMBER
  - Boolean &rarr; jsonx::Object::Type::BOOLEAN
  - String &rarr; jsonx::Object::Type::STRING
  - null &rarr; jsonx::Object::Type::Null
  
There are convert and check function to be used on an object for each type.

```cpp

jsonx::Object jObj;

std::cout << jObj.type() << std::endl; //returns TYPE::OBJECT
std::cout << jObj.isObject() << std::endl; //returns true
std::string s = jObj.toString(); // s = "";

jObj = jsonx::Array();
std::cout << jObj.isArray() << std::endl; //returns true
std::cout << jObj.isObject() << std::endl; //returns false

jObj = "Hello World";
std::cout << jObj.isString() << std::endl; //returns true
std::cout << jObj.isArray() << std::endl; //returns false
s = jObj.toString(); // s = "Hello World";

jObj = 43;
std::cout << jObj.isNumber() << std::endl; //returns true
std::cout << jObj.isBoolean() << std::endl; //returns false
double d = jObj.toNumber(); //d = 43;
std::vector<Object> v = jObj.toArray(); // v is empty

jObj = true;
std::cout << jObj.isBoolean() << std::endl; //returns true
std::cout << jObj.isNumber() << std::endl; //returns false
bool b = jObj.toBoolean(); // b = true

jObj = jsonx::null;
std::cout << jObj.isNull() << std::endl; //returns true
std::cout << jObj.isNumber() << std::endl; //returns false

//Can also be directly used on key path

jObj["Key"]["to"]["Number"] = 23.456;
jObj["Key"]["to"]["String"] = "Hello World";
d = jObj["Key"]["to"]["Number"].toNumber(); // d = 23.456
s = jObj["Key"]["to"]["String"].toString(); // s = "Hello World"
b = jObj["Key"]["to"]["String"].toBool(); // b = bool()
```