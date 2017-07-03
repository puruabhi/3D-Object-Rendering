#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <iostream>
#include <Eigen/Dense>

class Material
{
private:
    std::string name;
    Eigen::Vector3d ambient;
    Eigen::Vector3d diffuse;
    Eigen::Vector3d specular;
	double shininess;
	double opacity;
	int illum_no;
    
public:
    Material();
    Material(std::string name, Eigen::Vector3d ambient, Eigen::Vector3d diffuse,
             Eigen::Vector3d specular, double shininess, double opacity,
             int illum_no);
    std::string getName();
    Eigen::Vector3d getAmbient();
    Eigen::Vector3d getDiffuse();
    Eigen::Vector3d getSpecular();
    double getShininess();
    double getOpacity();
    int getIllum_no();

    void setName(std::string name);
    void setAmbient(Eigen::Vector3d ambient);
    void setDiffuse(Eigen::Vector3d diffuse);
    void setSpecular(Eigen::Vector3d specular);
    void setShininess(double shininess);
    void setOpacity(double opacity);
    void setIllum_no(int illum_no);
};

#endif // MATERIAL_H
