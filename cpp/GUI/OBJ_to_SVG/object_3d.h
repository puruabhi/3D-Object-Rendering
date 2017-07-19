#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <fstream>
#include <math.h>
#include <array>
#include <time.h>
#include <Eigen/Dense>
#include "light.h"
#include "material.h"
#include "object_view.h"
#include "globalvariables.h"

class Object_3D
{
private:
    std::vector< Eigen::Vector3d > vertices;
    std::vector< std::vector<int> > edges;
    std::vector< std::vector<int> > faces;
    std::map< std::vector<int>,std::string> material_of_faces;
    std::map<std::string,Material> materials;
    std::string name;
    std::string type;
    Light light;
    ObjectView objectView;

public:
    Object_3D();
    Object_3D(std::string filename);
    std::vector<Eigen::Vector3d> getVertices();
    std::vector< std::vector<int> > getEdges();
    std::vector< std::vector<int> > getFaces();
    std::string getType();
    std::map< std::vector<int>, std::string> getMaterialOfFaces();
    std::map<std::string, Material> getMaterials();
    Material getMaterialFromName(std::string name);
    Light getLight();
    ObjectView getObjectView();
    void addMaterial(std::string name, Material &material);
    void addMaterialOfFaces(std::vector<int> &face, std::string name);
    void addVertex(Eigen::Vector3d point);
    void addEdge(std::vector<int> &edge);
    void addFace(std::vector<int> &face);
    void setName(std::string name);
    void setType(std::string type);
    void setLight(Light &light);
    void setObjectView(ObjectView &objectView);
    void generateSvg(std::string saveFile, std::string show);
};

#endif // OBJECT_3D_H
