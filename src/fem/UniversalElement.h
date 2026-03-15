#pragma once

#include <array>
#include <vector>
#include <cmath>

#include "UniversalElement.h"

// Representation of a reference finite element(Q4) used in FEM formulation
class UniversalElement
{
public:
    static constexpr int nodesCount = 4;
    static constexpr int integrationPoints2D = 4;
    static constexpr int integrationPoints1D = 2;
    static constexpr int edgesCount = 4;

    using ShapeValues = std::array<double, nodesCount>;
    using ShapeDerivatives = std::array<double, nodesCount>;
    using EdgeShapeValues = std::array<ShapeValues, integrationPoints1D>;

    struct IntegrationPoint2D
    {
        double ksi;
        double eta;
        double weight;
    };

    struct IntegrationPoint1D
    {
        double point;
        double weight;
    };

    struct EdgeData
    {
        EdgeShapeValues N;
    };

    // 2x2 Gauss integration points for 2D element
    std::array<IntegrationPoint2D, integrationPoints2D> gaussPoints2D;

    std::array<IntegrationPoint1D, integrationPoints1D> gaussPoints1D;

    // Shape function values at each integration point
    std::array<ShapeValues, integrationPoints2D> NValues;

    // Derivatives of shape functions with respect to local variables
    std::array<ShapeDerivatives, integrationPoints2D> dNdKsi;
    std::array<ShapeDerivatives, integrationPoints2D> dNdEta;
    
    // Edge integration data. Each edge uses 1D Gauss integration. Used for computing boundary conditions
    std::array<EdgeData, edgesCount> edges;

    UniversalElement();

private:
    // Returns value of shape function at given local coordinates
    double shapeFunction(int nodeIndex, double ksi, double eta) const;
    double dShapeFunctionDksi(int nodeIndex, double ksi, double eta) const;
    double dShapeFunctionDeta(int nodeIndex, double ksi, double eta) const;

    // Initializes Gauss integration points and wages
    void initializeGaussPoints();
    // Computes values of shape functions and values of derivatives of shape functions using Gauss points
    void initializeShapeFunctions();
    // Computes values of shape functions on element edges. Used for computing boundary conditions
    void initializeEdgeShapeFunctions();
};