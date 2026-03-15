#pragma once

#include <array>

// Representation of an Element containing local matrices
class Element
{
public:
    using Matrix4 = std::array<std::array<double, 4>, 4>;
    using Vector4 = std::array<double, 4>;

    int id{};
    std::array<int, 4> nodeIds{};

    Matrix4 H{};
    Matrix4 C{};
    Vector4 P{};

    Element() = default;

    Element(int id, const std::array<int, 4>& nodeIds)
        : id(id), nodeIds(nodeIds)
    {
    }
};