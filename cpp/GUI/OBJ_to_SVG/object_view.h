#ifndef OBJECT_VIEW_H
#define OBJECT_VIEW_H

#include <vector>
#include <iostream>

class ObjectView
{
private:
    int scale;
    std::vector< std::pair<std::string,double> > rotations;
    std::string projection;
    //std::string show;
    double observerPosition;
    double strokeWidth;
    double strokeOpacity;
    bool backFacingPolygon;
    std::vector< std::pair<std::string,double> > parseRotations(std::string rotationString);

public:
    //Constructors
    ObjectView();
    ObjectView(int scale, std::string rotationString, std::string projection);
    ObjectView(int scale, std::vector< std::pair<std::string,double> > &rotations,
         std::string projection);
    ObjectView(int scale, std::string rotationString, std::string projection,
         double strokeWidth, double strokeOpacity, bool backFacingPolygon);
    ObjectView(int scale, std::vector< std::pair<std::string,double> > &rotations,
         std::string projection,double strokeWidth, double strokeOpacity,
         bool backFacingPolygon);

    //Getters
    int getScale();
    std::vector< std::pair<std::string,double> > getRotations();
    std::string getProjection();
    //std::string getShow();
    double getObserverPosition();
    double getStrokeWidth();
    double getStrokeOpacity();
    bool isBackFacingPolygon();

    //Setters
    void setScale(int scale);
    void setRotations(std::string rotationString);
    void setRotations(std::vector< std::pair<std::string,double> > &rotations);
    void setProjection(std::string projection);
    //void setShow(std::string show);
    void setObserverPosition(double observerPosition);
    void setStrokeWidth(double strokeWidth);
    void setStrokeOpacity(double strokeOpacity);
    void setBackFacingPolygon(bool backFacingPolygon);
};

#endif // OBJECT_VIEW_H
