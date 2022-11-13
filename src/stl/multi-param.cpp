/* Proj: cpp20-advanced-programming
 * File: variant.cpp
 * Created Date: 2022/10/16
 * Author: YangYangYang (yangyangyang0110@outlook.com)
 * Description:
 * -----
 * Last Modified: 2022/10/16 23:00:04
 * Modified By: YangYangYang (yangyangyang0110@outlook.com)
 * -----
 * Copyright (c) 2022  . All rights reserved.
 */

#include <iostream>
#include <variant>

struct PersonParam {
  float threshold;

  friend std::ostream& operator<<(
      std::ostream& os, const PersonParam& param) noexcept {
    return os << "threshold: " << param.threshold;
  }
};

struct FaceParam {
  int minimum_face, maximum_face;
  friend std::ostream& operator<<(
      std::ostream& os, const FaceParam& param) noexcept {
    return os << "minimum-face: " << param.minimum_face
              << " maximum-face: " << param.maximum_face;
  }
};

using Params = std::variant<PersonParam, FaceParam>;

void test() {
  Params param;
  param = PersonParam{.threshold = 0.7f};
  param = FaceParam{.minimum_face = 10, .maximum_face = 100};

  if (const auto* face_param = std::get_if<FaceParam>(&param)) {
    std::cout << *face_param << std::endl;
  } else if (const auto* person_param = std::get_if<PersonParam>(&param)) {
    std::cout << *person_param << std::endl;
  } else {
    throw std::runtime_error("");
  }

  std::cout << "Hello, world!" << std::endl;
}

int main() {
  test();
  return 0;
}
