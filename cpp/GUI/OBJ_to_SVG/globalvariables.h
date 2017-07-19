#ifndef STRINGVARIABLES_H
#define STRINGVARIABLES_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifdef WINDOWS
#define DIR_SLASH  "\\"
#else
#define DIR_SLASH "/"
#endif

const std::string FACE_TYPE = "Face-Specified";
const std::string EDGE_TYPE = "Egde-Specified";
const std::string PARALLEL = "Parallel";
const std::string PERSPECTIVE = "Perspective";

const std::string FACE = "Faces";
const std::string EDGE = "Edges";
const std::string VERTEX = "Vertices";

#endif // STRINGVARIABLES_H
