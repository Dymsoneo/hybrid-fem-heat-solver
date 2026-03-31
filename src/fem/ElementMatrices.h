#pragma once


#include "../mesh/Element.h"
#include "../mesh/Mesh.h"
#include "UniversalElement.h"
#include "../material/MaterialModel.h"


// Computes local FEM matrices for single Q4 element
class ElementMatrices
{
public:
    static constexpr double PI = 3.14159265358979323846;

	

   
    // Computes local conductivity matrix H for one element
    static Element::Matrix4 computeH(const Element& element, const Mesh& mesh, const UniversalElement& ue, double conductivity);

    // Computes local boundry convection matrix. Integration only on boundary edges
    static Element::Matrix4 computeHbc(const Element& element, const Mesh& mesh, const UniversalElement& ue, double alpha);

    //  Computes local capacity matrix C for one element
    static Element::Matrix4 computeC(const Element& element, const Mesh& mesh, const UniversalElement& ue, double density, double specificHeat);

    // Computes convection load vector. Integration only on boundary edges
    static Element::Vector4 computeP(const Element& element, const Mesh& mesh, const UniversalElement& ue, double alpha, double ambientTemperature);

	// Computes local conductivity matrix H for one element using material model. Conductivity is interpolated based on nodal temperatures
    static Element::Matrix4 computeHWithMaterialModel(const Element& element, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const std::vector<double>& nodalTemeratures);

	// Computes local capacity matrix C for one element using material model. Specific heat is interpolated based on nodal temperatures
	static Element::Matrix4 computeCWithMaterialModel(const Element& element, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const std::vector<double>& nodalTemeratures);

private:
    
    // Computes radial coordinate r at the current Gauss point (r = sum(N_i * r_i)). This is required by the axisymmetric formulation
    static double computeRadiusAtGaussPoint(const Element& element, const Mesh& mesh, const UniversalElement& ue, int integrationPointIndex);

    static bool isBoundaryEdge(const Element& element, const Mesh& mesh, int edgeIndex);

    static double computeEdgeDetJ(const Element& element, const Mesh& mesh, int edgeIndex);

    static double computeRadiusAtEdgeGaussPoint(const Element& element, const Mesh& mesh, const UniversalElement& ue, int edgeIndex, int integrationPointIndex1D);

    static double computeTemperatureAtGaussPoint(const Element& element, const UniversalElement& ue, const std::vector<double>& nodalTemperatures, int integrationPointIndex);
};