#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <string>

namespace tenacitas {
namespace plantuml_editor {
namespace evt {

struct jar_path_set {
  jar_path_set() = default;

  explicit jar_path_set(const char *p_value) : value(p_value) {}

  explicit jar_path_set(const std::string &p_value) : value(p_value) {}

  jar_path_set(const jar_path_set &p_jar_path_set) = default;

  jar_path_set(jar_path_set &&p_jar_path_set) = default;

  jar_path_set &operator=(const jar_path_set &p_jar_path_set) = default;

  jar_path_set &operator=(jar_path_set &&p_jar_path_set) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const jar_path_set &p_jar_path_set) {
    p_out << p_jar_path_set.value;
    return p_out;
  }

  std::string value;
};

} // namespace evt
} // namespace plantuml_editor
} // namespace tenacitas

#endif // EVENTS_H
