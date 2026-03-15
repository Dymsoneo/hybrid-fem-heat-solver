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
    using GlobalVector = std::vector<double>;
    

    // Structure used for returning result from assembling matrices
    struct AssemblyResult
    {
        GlobalMatrix H;
        GlobalMatrix Hbc;
        GlobalMatrix C;
        GlobalVector P;
    };

    // Assembles all global matrices and vectors needed
    static AssemblyResult assembleSystem(const Mesh& mesh, const UniversalElement& ue, double conductivity, double density, double specificHeat,
        double alpha, double ambientTemperature);

    

private:
   
    // Adds local element matrix into global matrix according to element node connectivity
    static void addLocalMatrixToGlobal(GlobalMatrix& globalMatrix, const Element& element, const Element::Matrix4& localMatrix);

    // Adds local element vector into global vector according to element node connectivity
    static void addLocalVectorToGlobal(GlobalVector& globalVector, const Element& element, const Element::Vector4& localVector);
};
