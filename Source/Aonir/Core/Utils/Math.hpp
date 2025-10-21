#pragma once

#include <cstddef>

// NOLINTBEGIN(misc-include-cleaner)

#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm.hpp>

namespace Aonir
{
    template<typename T, std::size_t R, std::size_t C>
    using Matrix = glm::mat<C, R, T>;

    using Matrix3 = Matrix<float, 3, 3>;
    using Matrix4 = Matrix<float, 4, 4>;

    template<typename T, std::size_t S>
    using Vector = glm::vec<S, T>;

    using Vector2 = Vector<float, 3>;
    using Vector3 = Vector<float, 3>;
    using Vector4 = Vector<float, 3>;
}

// NOLINTEND(misc-include-cleaner)
