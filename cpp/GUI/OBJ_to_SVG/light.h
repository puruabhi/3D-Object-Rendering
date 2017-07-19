#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Dense>

class Light
{
private:
    Eigen::Vector3d position;
    Eigen::Vector3d ambient;
    Eigen::Vector3d diffuse;
    Eigen::Vector3d specular;

public:
    Light();
    Light(Eigen::Vector3d position);
    Light(Eigen::Vector3d position,Eigen::Vector3d ambient,
          Eigen::Vector3d diffuse,Eigen::Vector3d specular);
    Eigen::Vector3d getPosition();
    Eigen::Vector3d getAmbient();
    Eigen::Vector3d getDiffuse();
    Eigen::Vector3d getSpecular();

    void setPosition(Eigen::Vector3d position);
    void setAmbient(Eigen::Vector3d ambient);
    void setDiffuse(Eigen::Vector3d diffuse);
    void setSpecular(Eigen::Vector3d specular);
};

#endif // LIGHT_H
