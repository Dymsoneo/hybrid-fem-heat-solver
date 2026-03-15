#pragma once

#include <array>

#include "../mesh/Element.h"
#include "../mesh/Mesh.h"
#include "UniversalElement.h"

// Representation of geometric transformation between local (ksi, eta) and global (r, z) coordinates
class Jacobian
{
public:
    using Matrix2 = std::array<std::array<double, 2>, 2>;
    using Derivatives4 = std::array<double, 4>;

    // Jacobian matrix:
    // [ dr/dksi   dz/dksi ]
    // [ dr/deta   dz/deta ]
    Matrix2 J{};

    // Inverse of Jacobian matrix
    Matrix2 invJ{};

    // Jacobian determinant
    double detJ{};

    // Derivatives of shape functions in global coordinates
    Derivatives4 dNdR{};
    Derivatives4 dNdZ{};

    Jacobian() = default;
    
    //Builds Jacobian for a given element and Gauss integration point.
    Jacobian(const Element& element,
        const Mesh& mesh,
        const UniversalElement& ue,
        int integrationPointIndex);

private:
    void computeJacobianMatrix(const Element& element,
        const Mesh& mesh,
        const UniversalElement& ue,
        int integrationPointIndex);

    void computeInverse();
    void computeGlobalDerivatives(const UniversalElement& ue, int integrationPointIndex);
};