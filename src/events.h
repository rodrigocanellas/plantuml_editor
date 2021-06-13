#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <string>

namespace tenacitas {
namespace plantuml_editor {
namespace evt {

struct jar_path_set {
  jar_path_set() = default;

  explicit jar_path_set(const std::string &p_value) : value(p_value) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const jar_path_set &p_jar_path_set) {
    p_out << p_jar_path_set.value;
    return p_out;
  }

  std::string value;
};

struct title_changed {
  title_changed() = default;

  explicit title_changed(int p_id, const std::string &p_title)
      : id(p_id), title(p_title) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const title_changed &p_title_changed) {
    p_out << p_title_changed.id << "," << p_title_changed.title;
    return p_out;
  }

  int id = -1;
  std::string title;
};

struct editor_closed {
  editor_closed() = default;

  explicit editor_closed(int p_id) : id(p_id) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const editor_closed &p_editor_closed) {
    p_out << p_editor_closed.id;
    return p_out;
  }

  int id;
};

} // namespace evt
} // namespace plantuml_editor
} // namespace tenacitas

#endif // EVENTS_H
