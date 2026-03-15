#pragma once

#include <vector>

#include "../mesh/Mesh.h"
#include "UniversalElement.h"
#include "ElementMatrices.h"

// Assembling global FEM matrices from local element matrices
class Assembly
{
public:

    using GlobalMatrix = std::vector<std::vector<double>>;

    // Builds global conductivity matrix H
    static GlobalMatrix assembleH(const Mesh& mesh, const UniversalElement& ue, double conductivity);

   
    // Builds global capacity matrix C
    static GlobalMatrix assembleC(const Mesh& mesh, const UniversalElement& ue, double density, double specificHeat);

private:
   
    // Adds local element matrix into global matrix according to element node connectivity
    static void addLocalMatrixToGlobal(GlobalMatrix& globalMatrix, const Element& element, const Element::Matrix4& localMatrix);
};
