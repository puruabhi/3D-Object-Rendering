#include "light.h"
using namespace std;
using namespace Eigen;

Light::Light()
{
    this->position = Vector3d(0,0,1);
    this->ambient = Vector3d(0.2,0.2,0.2);
    this->diffuse = Vector3d(0.8,0.8,0.8);
    this->specular = Vector3d(0.5,0.5,0.5);
}

Light::Light(Vector3d position)
{
    this->position = position;
    this->position.normalize();
    this->ambient = Vector3d(0.2,0.2,0.2);
    this->diffuse = Vector3d(0.8,0.8,0.8);
    this->specular = Vector3d(0.5,0.5,0.5);
}

Light::Light(Vector3d position, Vector3d ambient, Vector3d diffuse,
             Vector3d specular)
{
    this->position = position;
    this->position.normalize();
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Vector3d Light::getPosition()
{
    return this->position;
}

Vector3d Light::getAmbient()
{
    return this->ambient;
}

Vector3d Light::getDiffuse()
{
    return this->diffuse;
}

Vector3d Light::getSpecular()
{
    return this->specular;
}

void Light::setPosition(Vector3d position)
{
    this->position = position;
    this->position.normalize();
}

void Light::setAmbient(Vector3d ambient)
{
    this->ambient = ambient;
}

void Light::setDiffuse(Vector3d diffuse)
{
    this->diffuse = diffuse;
}

void Light::setSpecular(Vector3d specular)
{
    this->specular = specular;
}
