#include "MeshGenerator.h"

Mesh MeshGenerator::generateCylinderMesh(double radius, double height, int elementsR, int elementsZ)
{
    Mesh mesh;

    double dr = radius / elementsR;
    double dz = height / elementsZ;

    int nodeId = 0;
    int elementId = 0;

    // Generate nodes
    for (int j = 0; j <= elementsZ; j++)
    {
        for (int i = 0; i <= elementsR; i++)
        {
            double r = i * dr;
            double z = j * dz;

            bool isBoundary = (i == 0 || i == elementsR || j == 0 || j == elementsZ);

            mesh.addNode(Node(nodeId, r, z, isBoundary));
            nodeId++;
        }
    }

    // Generate elements
    for (int j = 0; j < elementsZ; j++)
    {
        for (int i = 0; i < elementsR; i++)
        {
            int n1 = j * (elementsR + 1) + i;
            int n2 = n1 + 1;
            int n4 = (j + 1) * (elementsR + 1) + i;
            int n3 = n4 + 1;

            mesh.addElement(Element(elementId, { n1, n2, n3, n4 }));
            elementId++;
        }
    }

    return mesh;
}
