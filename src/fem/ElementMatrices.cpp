#include "ElementMatrices.h"
#include "Jacobian.h"

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


bool ElementMatrices::isBoundaryEdge(
    const Element& element,
    const Mesh& mesh,
    int edgeIndex)
{
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

    if (!n1.isBoundary || !n2.isBoundary)
        return false;

    constexpr double tolerance = 1e-10;

    if (edgeIndex == 0 || edgeIndex == 2)
    {
        // Horizontal edge: both nodes must lie on z = 0 or z = H
        if (std::abs(n1.z - n2.z) > tolerance)
            return false;

        // The edge must be on the outer boundary,
        // not merely connect two boundary nodes.
        const double zMin = mesh.nodes.front().z;
        const double zMax = mesh.nodes.back().z;

        return std::abs(n1.z - zMin) < tolerance ||
            std::abs(n1.z - zMax) < tolerance;
    }

    // Vertical edge: both nodes must lie on r = 0 or r = R
    if (std::abs(n1.r - n2.r) > tolerance)
        return false;

    double rMax = 0.0;

    for (const Node& node : mesh.nodes)
        rMax = std::max(rMax, node.r);

    return std::abs(n1.r) < tolerance ||
        std::abs(n1.r - rMax) < tolerance;
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

double ElementMatrices::computeTemperatureAtGaussPoint(const Element& element, const UniversalElement& ue, const std::vector<double>& nodalTemperatures, int integrationPointIndex)
{
	double temperature = 0.0;

	for (int i = 0; i < UniversalElement::nodesCount; i++)
    {
		int globalNodeId = element.nodeIds[i];
		temperature += ue.NValues[integrationPointIndex][i] * nodalTemperatures[globalNodeId];
    }

	return temperature;
}

double ElementMatrices::computeTemperatureAtEdgeGaussPoint(const Element& element, const UniversalElement& ue, const std::vector<double>& nodalTemperatures, int edgeIndex, int integrationPointIndex1D)
{
	double temperature = 0.0;

    for (int i = 0; i < UniversalElement::nodesCount; i++)
    {
        int globalNodeId = element.nodeIds[i];

		temperature += ue.edges[edgeIndex].N[integrationPointIndex1D][i] * nodalTemperatures[globalNodeId];
    }

	return temperature;
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

Element::Matrix4 ElementMatrices::computeHWithMaterialModel(const Element& element, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const std::vector<double>& nodalTemeratures)
{
	Element::Matrix4 H{};

    for (int p = 0; p < UniversalElement::integrationPoints2D; p++)
    {
        Jacobian jac(element, mesh, ue, p);

		double r_gp = computeRadiusAtGaussPoint(element, mesh, ue, p);

		double T_gp = computeTemperatureAtGaussPoint(element, ue, nodalTemeratures, p);

		double conductivity = material.getConductivity(T_gp);

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

Element::Matrix4 ElementMatrices::computeCWithMaterialModel(const Element& element, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const std::vector<double>& nodalTemeratures)
{
	Element::Matrix4 C{};

    for (int p = 0; p < UniversalElement::integrationPoints2D; p++)
    {
        Jacobian jac(element, mesh, ue, p);

        double r_gp = computeRadiusAtGaussPoint(element, mesh, ue, p);

        double T_gp = computeTemperatureAtGaussPoint(element, ue, nodalTemeratures, p);

        double density = material.getDensity();

        double specificHeat = material.getSpecificHeat(T_gp);

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

Element::Matrix4 ElementMatrices::computeHbcWithHTCModels(const Element& element, const Mesh& mesh, const UniversalElement& ue, const BoundaryConditionManager& boundaryConditions, const std::vector<double>& nodalTemperatures)
{
	Element::Matrix4 Hbc{};

    for (int edge = 0; edge < UniversalElement::edgesCount; edge++)
    {
        if (!isBoundaryEdge(element, mesh, edge))
        {
            continue;
        }

        BoundarySide side = getBoundarySideFromEdge(edge);

        if (!boundaryConditions.hasHTCModel(side))
        {
            continue;
        }

        const HTCModel& htcModel = boundaryConditions.getHTCModel(side);

        double detJ1D = computeEdgeDetJ(element, mesh, edge);

        for (int p = 0; p < UniversalElement::integrationPoints1D; p++)
        {
            double surfaceTemperature =
                computeTemperatureAtEdgeGaussPoint(
                    element,
                    ue,
                    nodalTemperatures,
                    edge,
                    p
                );

            double h = htcModel.getHTC(surfaceTemperature);

            double weight = ue.gaussPoints1D[p].weight;

            double r_gp =
                computeRadiusAtEdgeGaussPoint(
                    element,
                    mesh,
                    ue,
                    edge,
                    p
                );

            double integrationFactor =
                2.0 * PI * r_gp * detJ1D * weight;

            for (int i = 0; i < UniversalElement::nodesCount; i++)
            {
                for (int j = 0; j < UniversalElement::nodesCount; j++)
                {
                    Hbc[i][j] +=
                        h *
                        ue.edges[edge].N[p][i] *
                        ue.edges[edge].N[p][j] *
                        integrationFactor;
                }
            }
        }
    }

    return Hbc;
}

Element::Vector4 ElementMatrices::computePWithHTCModels(const Element& element, const Mesh& mesh, const UniversalElement& ue, const BoundaryConditionManager& boundaryConditions, const std::vector<double>& nodalTemperatures, double ambientTemperature)
{
	Element::Vector4 P{};

    for (int edge = 0; edge < UniversalElement::edgesCount; edge++)
    {
        if (!isBoundaryEdge(element, mesh, edge))
        {
            continue;
        }

        BoundarySide side = getBoundarySideFromEdge(edge);

        if (!boundaryConditions.hasHTCModel(side))
        {
            continue;
        }

        const HTCModel& htcModel = boundaryConditions.getHTCModel(side);

        double detJ1D = computeEdgeDetJ(element, mesh, edge);

        for (int p = 0; p < UniversalElement::integrationPoints1D; p++)
        {
            double surfaceTemperature =
                computeTemperatureAtEdgeGaussPoint(
                    element,
                    ue,
                    nodalTemperatures,
                    edge,
                    p
                );

            double h = htcModel.getHTC(surfaceTemperature);

            double weight = ue.gaussPoints1D[p].weight;

            double r_gp =
                computeRadiusAtEdgeGaussPoint(
                    element,
                    mesh,
                    ue,
                    edge,
                    p
                );

            double integrationFactor =
                2.0 * PI * r_gp * detJ1D * weight;

            for (int i = 0; i < UniversalElement::nodesCount; i++)
            {
                P[i] +=
                    h *
                    ambientTemperature *
                    ue.edges[edge].N[p][i] *
                    integrationFactor;
            }
        }
    }

    return P;
}
