#include "object_view.h"
#include <sstream>
#include <iterator>
#include "globalvariables.h"

using namespace std;

ObjectView::ObjectView()
{
    this->scale = 100;
    this->projection = PARALLEL;
    //this->show = FACE;
    this->observerPosition = 1000;
    this->strokeWidth = 1.00;
    this->strokeOpacity = 1.00;
    this->backFacingPolygon = true;
}

ObjectView::ObjectView(int scale, string rotationString,
                       string projection)
{
    this->scale = scale;
    this->rotations = parseRotations(rotationString);
    this->projection = projection;
    //this->show = FACE;
    this->observerPosition = 1000;
    this->strokeWidth = 1.00;
    this->strokeOpacity = 1.00;
    this->backFacingPolygon = true;
}

ObjectView::ObjectView(int scale, std::vector<std::pair<string, double> > &rotations,
                       string projection)
{
    this->scale = scale;
    this->rotations = rotations;
    this->projection = projection;
    //this->show = FACE;
    this->observerPosition = 1000;
    this->strokeWidth = 1.00;
    this->strokeOpacity = 1.00;
    this->backFacingPolygon = true;
}

ObjectView::ObjectView(int scale, string rotationString,
            string projection, double strokeWidth, double strokeOpacity,
            bool backFacingPolygon)
{
    this->scale = scale;
    this->rotations = parseRotations(rotationString);
    this->projection = projection;
    //this->show = FACE;
    this->observerPosition = 1000;
    this->strokeWidth = strokeWidth;
    this->strokeOpacity = strokeOpacity;
    this->backFacingPolygon = backFacingPolygon;
}

ObjectView::ObjectView(int scale, std::vector<std::pair<string, double> > &rotations,
            string projection, double strokeWidth, double strokeOpacity,
            bool backFacingPolygon)
{
    this->scale = scale;
    this->rotations = rotations;
    this->projection = projection;
    //this->show = FACE;
    this->observerPosition = 1000;
    this->strokeWidth = strokeWidth;
    this->strokeOpacity = strokeOpacity;
    this->backFacingPolygon = backFacingPolygon;
}

vector< pair<string,double> > ObjectView::parseRotations(
        string rotationString)
{
    istringstream iss(rotationString);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    vector< pair<string,double> > rotations;
    if(tokens.size()>0){
        for(unsigned int i=0;i<tokens.size();i+=2){
            string axis = tokens[i];
            double angle = strtod(tokens[i+1].c_str(),NULL);
            pair<string,double> one_rotation(axis,angle);
            rotations.push_back(one_rotation);
        }
    }
    return rotations;
}

int ObjectView::getScale()
{
    return this->scale;
}

vector< pair<string,double> > ObjectView::getRotations()
{
    return this->rotations;
}

string ObjectView::getProjection()
{
    return this->projection;
}

//string ObjectView::getShow()
//{
//    return this->show;
//}

double ObjectView::getObserverPosition()
{
    return this->observerPosition;
}

double ObjectView::getStrokeWidth()
{
    return this->strokeWidth;
}

double ObjectView::getStrokeOpacity()
{
    return this->strokeOpacity;
}

bool ObjectView::isBackFacingPolygon()
{
    return this->backFacingPolygon;
}

void ObjectView::setScale(int scale)
{
    this->scale = scale;
}

void ObjectView::setRotations(string rotationString)
{
    this->rotations = parseRotations(rotationString);
}

void ObjectView::setRotations(std::vector<std::pair<string, double> > &rotations)
{
    this->rotations = rotations;
}

void ObjectView::setProjection(string projection)
{
    this->projection = projection;
}

//void ObjectView::setShow(string show)
//{
//    this->show = show;
//}

void ObjectView::setObserverPosition(double observerPosition)
{
    this->observerPosition = observerPosition;
}

void ObjectView::setStrokeWidth(double strokeWidth)
{
    this->strokeWidth = strokeWidth;
}

void ObjectView::setStrokeOpacity(double strokeOpacity)
{
    this->strokeOpacity = strokeOpacity;
}

void ObjectView::setBackFacingPolygon(bool backFacingPolygon)
{
    this->backFacingPolygon = backFacingPolygon;
}
