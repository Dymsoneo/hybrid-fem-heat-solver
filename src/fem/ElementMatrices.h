#pragma once
#define _USE_MATH_DEFINES

#include<cmath>

#include "../mesh/Element.h"
#include "../mesh/Mesh.h"
#include "UniversalElement.h"
#include "Jacobian.h"


// Computes local FEM matrices for single Q4 element
class ElementMatrices
{
public:
    static constexpr double PI = M_PI;

   
    // Computes local conductivity matrix H for one element
   
    static Element::Matrix4 computeH(const Element& element, const Mesh& mesh, const UniversalElement& ue, double conductivity);

    //  Computes local capacity matrix C for one element
    static Element::Matrix4 computeC(const Element& element, const Mesh& mesh, const UniversalElement& ue, double density, double specificHeat);

private:
    
    // Computes radial coordinate r at the current Gauss point (r = sum(N_i * r_i)). This is required by the axisymmetric formulation
    static double computeRadiusAtGaussPoint(const Element& element, const Mesh& mesh, const UniversalElement& ue, int integrationPointIndex);
};