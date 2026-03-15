#include "Assembly.h"

Assembly::GlobalMatrix Assembly::assembleH(const Mesh& mesh, const UniversalElement& ue, double conductivity)
{
    GlobalMatrix Hglobal(mesh.nodesCount, std::vector<double>(mesh.nodesCount, 0.0));

    for (const auto& element : mesh.elements)
    {
        Element::Matrix4 Hlocal = ElementMatrices::computeH(element, mesh, ue, conductivity);

        addLocalMatrixToGlobal(Hglobal, element, Hlocal);
    }

    return Hglobal;
}

Assembly::GlobalMatrix Assembly::assembleC(const Mesh& mesh, const UniversalElement& ue, double density, double specificHeat)
{
    GlobalMatrix Cglobal(mesh.nodesCount, std::vector<double>(mesh.nodesCount, 0.0));

    for (const auto& element : mesh.elements)
    {
        Element::Matrix4 Clocal = ElementMatrices::computeC(element, mesh, ue, density, specificHeat);

        addLocalMatrixToGlobal(Cglobal, element, Clocal);
    }

    return Cglobal;
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