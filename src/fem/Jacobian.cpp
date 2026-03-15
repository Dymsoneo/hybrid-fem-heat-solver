#include "Jacobian.h"

#include <stdexcept>

Jacobian::Jacobian(const Element& element, const Mesh& mesh, const UniversalElement& ue, int integrationPointIndex)
{
    computeJacobianMatrix(element, mesh, ue, integrationPointIndex);
    computeInverse();
    computeGlobalDerivatives(ue, integrationPointIndex);
}

void Jacobian::computeJacobianMatrix(const Element& element, const Mesh& mesh, const UniversalElement& ue, int integrationPointIndex)
{
        
    // Jacobian is built using shape function derivatives in local coordinates and node coordinates in global space

    double dr_dKsi = 0.0;
    double dz_dKsi = 0.0;
    double dr_dEta = 0.0;
    double dz_dEta = 0.0;

    for (int i = 0; i < 4; i++)
    {
        const Node& node = mesh.nodes[element.nodeIds[i]];

        dr_dKsi += ue.dNdKsi[integrationPointIndex][i] * node.r;
        dz_dKsi += ue.dNdKsi[integrationPointIndex][i] * node.z;

        dr_dEta += ue.dNdEta[integrationPointIndex][i] * node.r;
        dz_dEta += ue.dNdEta[integrationPointIndex][i] * node.z;
    }

    J[0][0] = dr_dKsi;
    J[0][1] = dz_dKsi;
    J[1][0] = dr_dEta;
    J[1][1] = dz_dEta;

    detJ = J[0][0] * J[1][1] - J[0][1] * J[1][0];

    if (detJ == 0.0)
    {
        throw std::runtime_error("Jacobian determinant is zero.");
    }
}

void Jacobian::computeInverse()
{

    invJ[0][0] = J[1][1] / detJ;
    invJ[0][1] = -J[0][1] / detJ;
    invJ[1][0] = -J[1][0] / detJ;
    invJ[1][1] = J[0][0] / detJ;
}

void Jacobian::computeGlobalDerivatives(const UniversalElement& ue, int integrationPointIndex)
{
  
    // Local derivatives (dN/dksi, dN/deta) are transformed into global derivatives (dN/dr, dN/dz) using:
    // [ dN/dr ]   [ invJ11 invJ12 ] [ dN/dksi ]
    // [ dN/dz ] = [ invJ21 invJ22 ] [ dN/deta ]
    for (int i = 0; i < 4; i++)
    {
        double dN_dKsi = ue.dNdKsi[integrationPointIndex][i];
        double dN_dEta = ue.dNdEta[integrationPointIndex][i];

        dNdR[i] = invJ[0][0] * dN_dKsi + invJ[0][1] * dN_dEta;
        dNdZ[i] = invJ[1][0] * dN_dKsi + invJ[1][1] * dN_dEta;
    }
}