#include <iostream>
#include <type_traits>
#include <yaml-cpp/yaml.h>

#include "marco_helper.hxx"

template <bool b, typename T>
using enable_if_t = typename std::enable_if<b, T>::type;

template <typename T>
using decay_t = typename std::decay<T>::type;

#define YAML_CONFIG_PATH "/Users/yang/Code/cpp/cpp20-advanced-programming/data/config.yaml"

#define YAML_PROPERTY_FROM(v1) obj.v1 = node[#v1].as<decay_t<decltype(obj.v1)>>();
#define YAML_PROPERTY_TO(v1)   node[#v1] = obj.v1;

struct Foo {
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
  friend std::ostream& operator<<(std::ostream& os, const Foo& obj) noexcept {
    return os << obj.a1 << " " << obj.a2 << " " << obj.a3 << " " << obj.a4 << " " << obj.a5 << " "
              << obj.a6 << " " << obj.a7 << " " << obj.a8 << " " << obj.a9 << " " << obj.a10 << " "
              << obj.a11 << " " << obj.a12 << " " << obj.a13 << " " << obj.a14 << " " << obj.a15
              << " " << obj.a16;
  }
};

struct Server {
  int port{};
  std::string username;
  std::string password;
};

struct Config {
  Server server;

  friend std::ostream& operator<<(std::ostream& os, const Config& obj) noexcept {
    return os << "[Config]: " << obj.server.port << " " << obj.server.username << ":"
              << obj.server.password;
  }
};

#define REGISTRY_YAML_TYPE(Type, ...)                 \
  template <>                                         \
  struct YAML::convert<Type> {                        \
    static Node encode(const Type& obj) {             \
      Node node;                                      \
      PASTE(YAML_PROPERTY_TO, __VA_ARGS__);           \
      return node;                                    \
    }                                                 \
                                                      \
    static bool decode(const Node& node, Type& obj) { \
      if (!node.IsMap())                              \
        return false;                                 \
      PASTE(YAML_PROPERTY_FROM, __VA_ARGS__);         \
      return true;                                    \
    }                                                 \
  }

REGISTRY_YAML_TYPE(Foo, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
REGISTRY_YAML_TYPE(Server, port, username, password);
REGISTRY_YAML_TYPE(Config, server);

// namespace YAML {
//
// template <>
// struct convert<Server> {
//   static Node encode(const Server& obj) {
//     Node node;
//     node["port"] = obj.port;
//     node["username"] = obj.username;
//     node["password"] = obj.password;
//     return node;
//   }
//
//   static bool decode(const Node& node, Server& obj) {
//     if (!node.IsMap()) {
//       std::cerr << "Error decoding1" << std::endl;
//       return false;
//     }
//     obj.port = node["port"].as<decay_t<decltype(obj.port)>>();
//     obj.username = node["username"].as<decay_t<decltype(obj.username)>>();
//     obj.password = node["password"].as<decay_t<decltype(obj.password)>>();
//     return true;
//   }
// };
//
// template <>
// struct convert<Config> {
//   static Node encode(const Config& obj) {
//     Node node;
//     node["server"] = obj.server;
//     return node;
//   }
//
//   static bool decode(const Node& node, Config& obj) {
//     if (!node.IsMap()) {
//       std::cerr << "Error decoding2" << std::endl;
//       return false;
//     }
//     obj.server = node["server"].as<decay_t<decltype(obj.server)>>();
//     return true;
//   }
// };
//
// } // namespace YAML

void testMarco() {
  auto root = YAML::Load(
      "{a1: 1, a2: 2, a3: 3, a4: 4, a5: 5, a6: 6, a7: 7, a8: 8, a9: 9, a10: 10, a11: 11, a12: 12, a13: 13, a14: 14, a15: 15, a16: 16, a17: 17, a18: 18, a19: 19}");
  auto foo = root.as<Foo>();
  std::cout << foo << std::endl;

  foo.a1 += 10;
  root["output"] = foo;
  std::cout << root << std::endl;
}

void testFromFile() {
  auto root = YAML::LoadFile(YAML_CONFIG_PATH);

  auto cfg = root.as<Config>();

  std::cout << cfg << std::endl;

  cfg.server.port += 10000;

  root["output"] = cfg;
  std::cout << root << std::endl;
}

void test() {
  testMarco();
  testFromFile();
}

int main() {
  test();
  return 0;
}