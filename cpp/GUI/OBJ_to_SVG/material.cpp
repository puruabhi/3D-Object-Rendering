#include "material.h"
using namespace std;
using namespace Eigen;

Material::Material()
{
    this->name = "";
    this-> ambient = Vector3d(0.2,0.2,0.2);
    this-> diffuse = Vector3d(1,0,0);
    this-> specular = Vector3d(0.3,0.3,0.3);
    this->shininess = 0;
    this->opacity = 1.0;
    this-> illum_no = 0;
}

Material::Material(string name, Vector3d ambient, Vector3d diffuse,
          Vector3d specular, double shininess, double opacity,
          int illum_no)
{
    this->name = name;
    this-> ambient = ambient;
    this-> diffuse = diffuse;
    this-> specular = specular;
    this->shininess = shininess;
    this->opacity = opacity;
    this-> illum_no = illum_no;
}

string Material::getName()
{
    return this->name;
}

Vector3d Material::getAmbient()
{
    return this->ambient;
}

Vector3d Material::getDiffuse()
{
    return this->diffuse;
}

Vector3d Material::getSpecular()
{
    return this->specular;
}

double Material::getShininess()
{
    return this->shininess;
}

double Material::getOpacity()
{
    return this->opacity;
}

int Material::getIllum_no()
{
    return this->illum_no;
}

void Material::setName(string name)
{
    this->name = name;
}

void Material::setAmbient(Vector3d ambient)
{
    this->ambient = ambient;
}

void Material::setDiffuse(Vector3d diffuse)
{
    this->diffuse = diffuse;
}

void Material::setSpecular(Vector3d specular)
{
    this->specular = specular;
}

void Material::setShininess(double shininess)
{
    this->shininess = shininess;
}

void Material::setOpacity(double opacity)
{
    this->opacity = opacity;
}

void Material::setIllum_no(int illum_no)
{
    this->illum_no = illum_no;
}
