#include "UniversalElement.h"

UniversalElement::UniversalElement()
{
    initializeGaussPoints();
    initializeShapeFunctions();
    initializeEdgeShapeFunctions();
}

double UniversalElement::shapeFunction(int nodeIndex, double ksi, double eta) const
{
    switch (nodeIndex)
    {
    case 0: return 0.25 * (1.0 - ksi) * (1.0 - eta);
    case 1: return 0.25 * (1.0 + ksi) * (1.0 - eta);
    case 2: return 0.25 * (1.0 + ksi) * (1.0 + eta);
    case 3: return 0.25 * (1.0 - ksi) * (1.0 + eta);
    default: return 0.0;
    }
}

double UniversalElement::dShapeFunctionDksi(int nodeIndex, double ksi, double eta) const
{
    switch (nodeIndex)
    {
    case 0: return -0.25 * (1.0 - eta);
    case 1: return  0.25 * (1.0 - eta);
    case 2: return  0.25 * (1.0 + eta);
    case 3: return -0.25 * (1.0 + eta);
    default: return 0.0;
    }
}

double UniversalElement::dShapeFunctionDeta(int nodeIndex, double ksi, double eta) const
{
    switch (nodeIndex)
    {
    case 0: return -0.25 * (1.0 - ksi);
    case 1: return -0.25 * (1.0 + ksi);
    case 2: return  0.25 * (1.0 + ksi);
    case 3: return  0.25 * (1.0 - ksi);
    default: return 0.0;
    }
}

void UniversalElement::initializeGaussPoints()
{
    // Gauss points for 2x2 integration scheme
    const double invSqrt3 = 1.0 / std::sqrt(3.0);

    gaussPoints2D = { {
        {-invSqrt3, -invSqrt3, 1.0},
        { invSqrt3, -invSqrt3, 1.0},
        { invSqrt3,  invSqrt3, 1.0},
        {-invSqrt3,  invSqrt3, 1.0}
    } };

    gaussPoints1D = { {
        {-invSqrt3, 1.0},
        { invSqrt3, 1.0}
    } };
}

void UniversalElement::initializeShapeFunctions()
{
    for (int p = 0; p < integrationPoints2D; p++)
    {
        double ksi = gaussPoints2D[p].ksi;
        double eta = gaussPoints2D[p].eta;

        for (int i = 0; i < nodesCount; i++)
        {
            NValues[p][i] = shapeFunction(i, ksi, eta);
            dNdKsi[p][i] = dShapeFunctionDksi(i, ksi, eta);
            dNdEta[p][i] = dShapeFunctionDeta(i, ksi, eta);
        }
    }
}

void UniversalElement::initializeEdgeShapeFunctions()
{
    const double invSqrt3 = 1.0 / std::sqrt(3.0);

    // Edge 0: bottom, eta = -1
    for (int p = 0; p < integrationPoints1D; p++)
    {
        double ksi = gaussPoints1D[p].point;
        double eta = -1.0;

        for (int i = 0; i < nodesCount; i++)
        {
            edges[0].N[p][i] = shapeFunction(i, ksi, eta);
        }
    }

    // Edge 1: right, ksi = 1
    for (int p = 0; p < integrationPoints1D; p++)
    {
        double ksi = 1.0;
        double eta = gaussPoints1D[p].point;

        for (int i = 0; i < nodesCount; i++)
        {
            edges[1].N[p][i] = shapeFunction(i, ksi, eta);
        }
    }

    // Edge 2: top, eta = 1
    for (int p = 0; p < integrationPoints1D; p++)
    {
        double ksi = gaussPoints1D[p].point;
        double eta = 1.0;

        for (int i = 0; i < nodesCount; i++)
        {
            edges[2].N[p][i] = shapeFunction(i, ksi, eta);
        }
    }

    // Edge 3: left, ksi = -1
    for (int p = 0; p < integrationPoints1D; p++)
    {
        double ksi = -1.0;
        double eta = gaussPoints1D[p].point;

        for (int i = 0; i < nodesCount; i++)
        {
            edges[3].N[p][i] = shapeFunction(i, ksi, eta);
        }
    }
}