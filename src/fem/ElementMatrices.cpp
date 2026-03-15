#include "ElementMatrices.h"

double ElementMatrices::computeRadiusAtGaussPoint(const Element& element, const Mesh& mesh, const UniversalElement& ue, int integrationPointIndex)
{
    double r = 0.0;

    for (int i = 0; i < UniversalElement::nodesCount; i++)
    {
        const Node& node = mesh.nodes[element.nodeIds[i]];
        r += ue.NValues[integrationPointIndex][i] * node.r;
    }

    return r;
}

Element::Matrix4 ElementMatrices::computeH(const Element& element, const Mesh& mesh, const UniversalElement& ue, double conductivity)
{
    Element::Matrix4 H{};

    for (int p = 0; p < UniversalElement::integrationPoints2D; p++)
    {
        Jacobian jac(element, mesh, ue, p);

        double r_gp = computeRadiusAtGaussPoint(element, mesh, ue, p);
        double weight = ue.gaussPoints2D[p].weight;

        double integrationFactor = 2.0 * PI * r_gp * jac.detJ * weight;

        for (int i = 0; i < UniversalElement::nodesCount; i++)
        {
            for (int j = 0; j < UniversalElement::nodesCount; j++)
            {
                H[i][j] += conductivity * (jac.dNdR[i] * jac.dNdR[j] + jac.dNdZ[i] * jac.dNdZ[j]) * integrationFactor;
            }
        }
    }

    return H;
}

Element::Matrix4 ElementMatrices::computeC(const Element& element, const Mesh& mesh, const UniversalElement& ue, double density, double specificHeat)
{
    Element::Matrix4 C{};

    for (int p = 0; p < UniversalElement::integrationPoints2D; p++)
    {
        Jacobian jac(element, mesh, ue, p);

        double r_gp = computeRadiusAtGaussPoint(element, mesh, ue, p);
        double weight = ue.gaussPoints2D[p].weight;

        double integrationFactor = 2.0 * PI * r_gp * jac.detJ * weight;

        for (int i = 0; i < UniversalElement::nodesCount; i++)
        {
            for (int j = 0; j < UniversalElement::nodesCount; j++)
            {
                C[i][j] += density * specificHeat * ue.NValues[p][i] * ue.NValues[p][j] * integrationFactor;
            }
        }
    }

    return C;
}