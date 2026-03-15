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

bool ElementMatrices::isBoundaryEdge(const Element& element, const Mesh& mesh, int edgeIndex)
{
    // Local edge numbering for Q4 element:
    // edge 0 -> nodes (0,1) bottom
    // edge 1 -> nodes (1,2) right
    // edge 2 -> nodes (2,3) top
    // edge 3 -> nodes (3,0) left

    // if both nodes of the edge are boundary nodes then the edge belongs to the boundary

    int firstNodeIndex = 0;
    int secondNodeIndex = 0;

    switch (edgeIndex)
    {
    case 0:
        firstNodeIndex = 0;
        secondNodeIndex = 1; 
        break;
    case 1:
        firstNodeIndex = 1;
        secondNodeIndex = 2;
        break;
    case 2:
        firstNodeIndex = 2;
        secondNodeIndex = 3;
        break;
    case 3:
        firstNodeIndex = 3;
        secondNodeIndex = 0;
        break;
    default:
        return false;
    }

    const Node& n1 = mesh.nodes[element.nodeIds[firstNodeIndex]];
    const Node& n2 = mesh.nodes[element.nodeIds[secondNodeIndex]];

    return n1.isBoundary && n2.isBoundary;
}

double ElementMatrices::computeEdgeDetJ(const Element& element, const Mesh& mesh, int edgeIndex)
{

    // For 1D Gauss integration along an element edge detJ = edgeLength / 2
    int firstNodeIndex = 0;
    int secondNodeIndex = 0;

    switch (edgeIndex)
    {
    case 0:
        firstNodeIndex = 0;
        secondNodeIndex = 1;
        break;
    case 1:
        firstNodeIndex = 1;
        secondNodeIndex = 2;
        break;
    case 2:
        firstNodeIndex = 2;
        secondNodeIndex = 3;
        break;
    case 3:
        firstNodeIndex = 3;
        secondNodeIndex = 0;
        break;
    default:
        return 0.0;
    }

    const Node& n1 = mesh.nodes[element.nodeIds[firstNodeIndex]];
    const Node& n2 = mesh.nodes[element.nodeIds[secondNodeIndex]];

    double dr = n2.r - n1.r;
    double dz = n2.z - n1.z;

    double edgeLength = std::sqrt(dr * dr + dz * dz);

    return edgeLength / 2.0;
}

double ElementMatrices::computeRadiusAtEdgeGaussPoint(const Element& element, const Mesh& mesh, const UniversalElement& ue, int edgeIndex, int integrationPointIndex1D)
{
    //Radius at edge Gauss point r = sum(N_i * r_i)
    // but shape functions are taken from edge integration data
    double r = 0.0;

    for (int i = 0; i < UniversalElement::nodesCount; i++) 
    {
        const Node& node = mesh.nodes[element.nodeIds[i]];
        r += ue.edges[edgeIndex].N[integrationPointIndex1D][i] * node.r;
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

Element::Matrix4 ElementMatrices::computeHbc(const Element& element, const Mesh& mesh, const UniversalElement& ue, double alpha)
{
    Element::Matrix4 Hbc{};

    for (int edge = 0; edge < UniversalElement::edgesCount; edge++)
    {
        if (!isBoundaryEdge(element, mesh, edge))
        {
            continue;
        }

        double detJ1D = computeEdgeDetJ(element, mesh, edge);

        for (int p = 0; p < UniversalElement::integrationPoints1D; p++)
        {
            double weight = ue.gaussPoints1D[p].weight;
            double r_gp = computeRadiusAtEdgeGaussPoint(element, mesh, ue, edge, p);

            double integrationFactor = 2.0 * PI * r_gp * detJ1D * weight;

            for (int i = 0; i < UniversalElement::nodesCount; i++) 
            {
                for (int j = 0; j < UniversalElement::nodesCount; j++)
                {
                    Hbc[i][j] += alpha * ue.edges[edge].N[p][i] * ue.edges[edge].N[p][j] * integrationFactor;
                }
            }
        }
    }

    return Hbc;
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

Element::Vector4 ElementMatrices::computeP(const Element& element, const Mesh& mesh, const UniversalElement& ue, double alpha, double ambientTemperature)
{
    Element::Vector4 P{};

    for (int edge = 0; edge < UniversalElement::edgesCount; edge++)
    {
        if (!isBoundaryEdge(element, mesh, edge))
        {
            continue;
        }

        double detJ1D = computeEdgeDetJ(element, mesh, edge);

        for (int p = 0; p < UniversalElement::integrationPoints1D; p++)
        {
            double weight = ue.gaussPoints1D[p].weight;
            double r_gp = computeRadiusAtEdgeGaussPoint(element, mesh, ue, edge, p);

            double integrationFactor = 2.0 * PI * r_gp * detJ1D * weight;

            for (int i = 0; i < UniversalElement::nodesCount; i++)
            {
                P[i] += alpha * ambientTemperature * ue.edges[edge].N[p][i] * integrationFactor;
            }
        }
    }

    return P;
}
