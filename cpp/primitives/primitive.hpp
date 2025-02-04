#pragma once

#include "../drawable.hpp"

namespace engine {

class Primitive : public Drawable {
protected:
  using vtx = Math::Vertex;
  using v4 = Math::vec4;
  using v3 = Math::vec3;
  using v2 = Math::vec2;

public:
};

} // namespace engine