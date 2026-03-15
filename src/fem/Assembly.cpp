#include "Assembly.h"

Assembly::AssemblyResult Assembly::assembleSystem(const Mesh& mesh, const UniversalElement& ue, double conductivity, double density, double specificHeat, double alpha, double ambientTemperature)
{
    AssemblyResult result;

    int n = mesh.nodesCount;

    result.H = GlobalMatrix(n, std::vector<double>(n, 0.0));
    result.Hbc = GlobalMatrix(n, std::vector<double>(n, 0.0));
    result.C = GlobalMatrix(n, std::vector<double>(n, 0.0));
    result.P = GlobalVector(n, 0.0);

    for (const auto& element : mesh.elements)
    {
        Element::Matrix4 Hlocal = ElementMatrices::computeH(element, mesh, ue, conductivity);
        Element::Matrix4 Hbclocal = ElementMatrices::computeHbc(element, mesh, ue, alpha);
        Element::Matrix4 Clocal = ElementMatrices::computeC(element, mesh, ue, density, specificHeat);

        Element::Vector4 Plocal = ElementMatrices::computeP(element, mesh, ue, alpha, ambientTemperature);

        addLocalMatrixToGlobal(result.H, element, Hlocal);
        addLocalMatrixToGlobal(result.Hbc, element, Hbclocal);
        addLocalMatrixToGlobal(result.C, element, Clocal);

        addLocalVectorToGlobal(result.P, element, Plocal);
    }

    return result;
}


void Assembly::addLocalMatrixToGlobal(GlobalMatrix& globalMatrix, const Element& element, const Element::Matrix4& localMatrix)
{
    for (int i = 0; i < 4; i++)
    {
        int globalI = element.nodeIds[i];

        for (int j = 0; j < 4; j++)
        {
            int globalJ = element.nodeIds[j];
            globalMatrix[globalI][globalJ] += localMatrix[i][j];
        }
    }
}

void Assembly::addLocalVectorToGlobal(GlobalVector& globalVector, const Element& element, const Element::Vector4& localVector)
{
    for (int i = 0; i < 4; i++)
    {
        int globalI = element.nodeIds[i];
        globalVector[globalI] += localVector[i];
    }
}
