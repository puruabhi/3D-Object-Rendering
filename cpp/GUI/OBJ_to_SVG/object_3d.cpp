#include "object_3d.h"

using namespace std;
using namespace Eigen;

int IMG_WIDTH = 10000;
int IMG_HEIGHT = 10000;
double SCREEN_DIST = 400;
const double PERSP_SCALE = 500;

void parseObject(Object_3D *obj, string filename);
void parseMaterial(Object_3D *obj, string filename);
vector<Vector3d> get_transformed_vertices(vector<Vector3d> vertices,
    vector< pair<string,double> > rotations, double scale,
    pair<string,double> projection);
Matrix3d get_transfortation_matrix(vector< pair<string,double> >& rotations,
                                   double scale);
Vector3d get_object_center(vector<Vector3d>& vertices);
void move_obj_to_center(vector<Vector3d>& vertices, Vector3d obj_cent);
vector<Vector3d> transform_perspective(vector<Vector3d>& vertices, double observer);
Matrix3d rotate(Matrix3d matrix, double angle, string axis);
Matrix3d rot_x(Matrix3d matrix, double angle);
Matrix3d rot_y(Matrix3d matrix, double angle);
Matrix3d rot_z(Matrix3d matrix, double angle);
vector< vector<int> > clip_faces(vector< vector<int> > faces,vector<Vector3d>& vertices,
    double observer, map<vector<int>,string>& transformed_material_of_faces,
    map<vector<int>,string> material_of_faces);
void set_image_dimension(vector<Vector3d>& vertices);
void write_SVG_header(ofstream& file, string saveFile);
void write_SVG_dot(ofstream& file, Vector3d vertex, ObjectView &view);
void write_SVG_footer(ofstream& file);
vector< vector<int> > make_edge_list(vector< vector<int> >& face_list);
void remove_duplicates(vector< vector<int> >& list);
void write_edges(ofstream& file,vector< vector<int> >& edge_list,
    vector<Vector3d>& points, ObjectView &view);
void write_SVG_line(ofstream& file, Vector3d p1, Vector3d p2, ObjectView &view);
vector< pair<double,int> > get_z_list(vector< vector<int> >& faces,
                                      vector<Vector3d>& vertices);
void write_faces(ofstream& file, vector< pair<double,int> >& z_list,
    vector< vector<int> >& face_list, vector<Vector3d>& points,
    map<vector<int>,string>& material_name_of_faces, map<string,Material>& materials,
    Light& light, ObjectView &view);
Vector3d get_normal(vector<int>& face, vector<Vector3d>& points);
vector<Vector3d> three_diff_vertices(vector<int>& face, vector<Vector3d>& points);
Vector3i get_face_color(Light& light, Material& material, Vector3d& normal);
Vector3d get_ele_wise_prod(Vector3d a, Vector3d b);
Vector3d check_color(Vector3d color);
Vector3d get_half_vector(Vector3d a, Vector3d b);
Vector3i get_floor(Vector3d a);
void write_SVG_poly(ofstream& file, vector<int>& face,
    vector<Vector3d>& points, Vector3i fill,ObjectView &view,
    double fill_opacity);
string get_fill_string(Vector3i fill);

Object_3D::Object_3D()
{

}

Object_3D::Object_3D(string filename)
{
    parseObject(this,filename);
}

vector<Vector3d> Object_3D::getVertices()
{
    return this->vertices;
}

vector< vector<int> > Object_3D::getEdges()
{
    return this->edges;
}

vector< vector<int> > Object_3D::getFaces()
{
    return this->faces;
}

string Object_3D::getType()
{
    return this->type;
}

map< vector<int>, string> Object_3D::getMaterialOfFaces()
{
    return this->material_of_faces;
}

map<string, Material> Object_3D::getMaterials()
{
    return this->materials;
}

Material Object_3D::getMaterialFromName(string name)
{
    return this->materials[name];
}

Light Object_3D::getLight()
{
    return this->light;
}

ObjectView Object_3D::getObjectView()
{
    return this->objectView;
}

void Object_3D::addMaterial(string name, Material &material)
{
    this->materials[name] = material;
}

void Object_3D::addMaterialOfFaces(vector<int> &face, string name)
{
    this->material_of_faces[face] = name;
}

void Object_3D::addVertex(Vector3d point)
{
    this->vertices.push_back(point);
}

void Object_3D::addEdge(vector<int> &edge)
{
    this->edges.push_back(edge);
}

void Object_3D::addFace(vector<int> &face)
{
    this->faces.push_back(face);
}

void Object_3D::setName(string name)
{
    this->name = name;
}

void Object_3D::setType(string type)
{
    if(type==FACE_TYPE)
    {
        if (this->faces.size() != 0)
        {
            this->type = FACE_TYPE;
        }
        else
        {
            try
            {
                throw "Face list is empty!";
            }
            catch(char const* e)
            {
                cout<<"An error occurred: "<< e <<endl;
            }
        }
    }

    if(type==EDGE_TYPE)
    {
        if (this->edges.size() != 0)
        {
            this->type = EDGE_TYPE;
        }
        else
        {
            try
            {
                throw "Edge list is empty!";
            }
            catch(char const* e)
            {
                cout<<"An error occurred: "<< e <<endl;
            }
        }
    }
}

void Object_3D::setLight(Light &light)
{
    this->light = light;
}

void Object_3D::setObjectView(ObjectView &objectView)
{
    this->objectView = objectView;
}

void Object_3D::generateSvg(string saveFile, string show)
{
    ObjectView view = this->getObjectView();
    Light light = this->getLight();
    string projection = view.getProjection();
    double observerPosition = view.getObserverPosition();
    vector<Vector3d> transformed_vertices = get_transformed_vertices(
                this->getVertices(),view.getRotations(),view.getScale(),
                make_pair(view.getProjection(),view.getObserverPosition()));

    vector< vector<int> > transformed_faces = this->getFaces();
    map<vector<int>,string>transformed_material_of_faces;
    if(projection == PERSPECTIVE)
    {
        transformed_faces = clip_faces(transformed_faces,transformed_vertices,
                    observerPosition,transformed_material_of_faces,this->getMaterialOfFaces());
    }
    else
    {
        transformed_material_of_faces = this->getMaterialOfFaces();
    }

    set_image_dimension(transformed_vertices);

    ofstream file;
    file.open(saveFile.c_str());

    write_SVG_header(file, saveFile);

    if(show == VERTEX){
        for(unsigned int i=0;i<transformed_vertices.size();i++){
            write_SVG_dot(file,transformed_vertices[i], view);
        }
    }
    else if(show == EDGE)
    {
        vector< vector<int> > edge_list;
        if(this->getType() == FACE_TYPE)
        {
            edge_list = make_edge_list(transformed_faces);
        }
        else
        {
            edge_list = this->getEdges();
        }

        write_edges(file,edge_list,transformed_vertices,view);
    }
    else if(show == FACE)
    {
        if(this->getType() == FACE_TYPE)
        {
            map<string,Material> materials = this->getMaterials();

            vector< pair<double,int> >z_list;
            z_list = get_z_list(transformed_faces, transformed_vertices);

            sort(z_list.begin(),z_list.end());

            write_faces(file,z_list,transformed_faces,transformed_vertices,
                transformed_material_of_faces, materials, light,view);
        }
    }
    write_SVG_footer(file);
    file.close();
}

void parseObject(Object_3D *obj, string filename)
{
    string line;
    ifstream file (filename.c_str());
    if(file.is_open())
    {
        Material material;
        while(getline(file,line))
        {
            istringstream iss(line);
            vector<string> tokens;
            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(tokens));
            if(tokens.size()>0)
            {
                Vector3d vertex;
                vector<int> edge;
                vector<int> face;
                if(tokens[0][0]=='#');
                else if(tokens[0]=="v")
                {
                    vertex(0) = strtod(tokens[1].c_str(),NULL);
                    vertex(1) = strtod(tokens[2].c_str(),NULL);
                    vertex(2) = strtod(tokens[3].c_str(),NULL);
                    obj->addVertex(vertex);
                }
                else if(tokens[0]=="l")
                {
                    for(unsigned int i=1;i<tokens.size();i++)
                    {
                        edge.push_back(atoi(tokens[i].c_str()));
                    }
                    obj->addEdge(edge);
                }
                else if(tokens[0]=="f")
                {
                    for(unsigned int i=1;i<tokens.size();i++)
                    {
                        int vertex_no = atoi(tokens[i].c_str());
                        face.push_back(vertex_no);
                    }
                    obj->addFace(face);
                    obj->addMaterialOfFaces(face,material.getName());
                }
                else if(tokens[0]=="mtllib")
                {
                    string material_file_name = filename.substr(0, filename.find_last_of("\\/"))
                            + DIR_SLASH + tokens[1];
                    parseMaterial(obj,material_file_name);
                }
                else if(tokens[0]=="usemtl")
                {
                    string name = tokens[1];
                    material = obj->getMaterialFromName(name);
                }
            }
        }
        file.close();
    }
    else
    {
        try
        {
            throw "Unable to open file ";
        }
        catch(char const* e)
        {
            cout<<e<<filename<<endl;
        }
    }
}

void parseMaterial(Object_3D *obj, string filename)
{
    string line;
    ifstream file (filename.c_str());
    if(file.is_open())
    {
        int count_newmtl = 0;
        Material material;
        while(getline(file,line))
        {
            istringstream iss(line);
            vector<string> tokens;
            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(tokens));
            if(tokens.size()>0)
            {
                if(tokens[0][0]=='#');
                else if(tokens[0] == "newmtl")
                {
                    if(count_newmtl>0)
                    {
                        obj->addMaterial(material.getName(), material);
                    }
                    material = Material();
                    material.setName(tokens[1]);
                    count_newmtl++;
                }
                else if(tokens[0] == "Ka")
                {
                    Vector3d ambient;
                    ambient(0) = strtod(tokens[1].c_str(),NULL);
                    ambient(1) = strtod(tokens[2].c_str(),NULL);
                    ambient(2) = strtod(tokens[3].c_str(),NULL);
                    material.setAmbient(ambient);
                }
                else if(tokens[0] == "Kd")
                {
                    Vector3d diffuse;
                    diffuse(0) = strtod(tokens[1].c_str(),NULL);
                    diffuse(1) = strtod(tokens[2].c_str(),NULL);
                    diffuse(2) = strtod(tokens[3].c_str(),NULL);
                    material.setDiffuse(diffuse);
                }
                else if(tokens[0] == "Ks")
                {
                    Vector3d specular;
                    specular(0) = strtod(tokens[1].c_str(),NULL);
                    specular(1) = strtod(tokens[2].c_str(),NULL);
                    specular(2) = strtod(tokens[3].c_str(),NULL);
                    material.setSpecular(specular);
                }
                else if(tokens[0] == "Ns")
                {
                    double shininess = strtod(tokens[1].c_str(),NULL);
                    material.setShininess(shininess);
                }
                else if(tokens[0] == "d")
                {
                    double opacity = strtod(tokens[1].c_str(),NULL);
                    material.setOpacity(opacity);
                }
                else if(tokens[0] == "illum")
                {
                    int illum_no = atoi(tokens[1].c_str());
                    material.setIllum_no(illum_no);
                }
            }
        }
        if(count_newmtl>0){
            obj->addMaterial(material.getName(), material);
        }
    }
    else{

    }
}

vector<Vector3d> get_transformed_vertices(vector<Vector3d> vertices,
    vector< pair<string,double> > rotations, double scale,
    pair<string,double> projection)
{
    vector<Vector3d> transformed_vertices;
    Matrix3d tranformation_matrix = get_transfortation_matrix(rotations,scale);
    for(unsigned int i=0;i<vertices.size();i++){
        transformed_vertices.push_back(tranformation_matrix*vertices[i]);
    }

    Vector3d obj_cent = get_object_center(transformed_vertices);
    move_obj_to_center(transformed_vertices, obj_cent);

    if(projection.first == PERSPECTIVE){
        transformed_vertices = transform_perspective(transformed_vertices,projection.second);
    }

    return transformed_vertices;
}

Matrix3d get_transfortation_matrix(vector< pair<string,double> >& rotations, double scale){
    Matrix3d trans_mat = Matrix3d::Identity(3,3);
    for(unsigned int i=0;i<rotations.size();i++){
        string axis = rotations[i].first;
        double angle = (rotations[i].second*PI)/180;
        trans_mat = rotate(trans_mat,angle,axis);
    }
    return trans_mat*scale;
}

Vector3d get_object_center(vector<Vector3d>& vertices){
    Vector3d center(0,0,0);
    for(unsigned int i=0;i<vertices.size();i++){
        center = center + vertices[i];
    }
    if(vertices.size()>0){
        center = center/(vertices.size());
    }
    return center;
}

void move_obj_to_center(vector<Vector3d>& vertices, Vector3d obj_cent){
    for(unsigned int i=0;i<vertices.size();i++){
        vertices[i] -= obj_cent;
    }
}

vector<Vector3d> transform_perspective(vector<Vector3d>& vertices, double observer){
    vector<Vector3d> transformed_vertices;
    for(unsigned int i=0;i<vertices.size();i++){
        Vector3d vertex = vertices[i];
        double z = vertex(2);
        if(z==observer)
            z+=0.1;
        double factor = PERSP_SCALE/abs(observer-z);
        vertex(0) = vertex(0)*factor;
        vertex(1) = vertex(1)*factor;
        vertex(2) = vertex(2)*factor;
        //vertex = vertex*factor;
        transformed_vertices.push_back(vertex);
    }
    return transformed_vertices;
}

Matrix3d rotate(Matrix3d matrix, double angle, string axis){
    if(axis == "x" || axis == "X")
        matrix = rot_x(matrix,angle);
    else if(axis == "y" || axis == "Y")
        matrix = rot_y(matrix, angle);
    else if(axis == "z" || axis == "Z"){
        matrix = rot_z(matrix, angle);
    }
    return matrix;
}

Matrix3d rot_x(Matrix3d matrix, double angle){
    Matrix3d trans_mat;
    trans_mat << 	1,     0     ,      0     ,
                    0, cos(angle), -sin(angle),
                    0, sin(angle),  cos(angle);
    return trans_mat*matrix;
}

Matrix3d rot_y(Matrix3d matrix, double angle){
    Matrix3d trans_mat;
    trans_mat << 	cos(angle),     0     , sin(angle),
                        0     ,     1     ,     0     ,
                   -sin(angle),     0     , cos(angle);
    return trans_mat*matrix;
}

Matrix3d rot_z(Matrix3d matrix, double angle){
    Matrix3d trans_mat;
    trans_mat << 	cos(angle), -sin(angle),    0    ,
                    sin(angle),  cos(angle),    0    ,
                        0     ,      0     ,    1    ;
    return trans_mat*matrix;
}

vector< vector<int> > clip_faces(vector< vector<int> > faces,vector<Vector3d>& vertices,
    double observer, map<vector<int>,string>& transformed_material_of_faces,
    map<vector<int>,string> material_of_faces) {
    double screen = observer - SCREEN_DIST;
    vector< vector<int> > transformed_faces;

    for(unsigned int j=0;j<faces.size();j++){
        vector<int> face = faces[j];
        string material_name = material_of_faces[face];
        vector<int> clipped_face;
        for(unsigned int i=0;i<face.size();i++){
            Vector3d v1 = vertices[face[i]-1];
            Vector3d v2 = vertices[face[(i+1)%face.size()]-1];
            if((v1(2)>screen) && (v2(2)<=screen)){
                double factor = (screen-v1(2))/(v2(2)-v1(2));
                double x = v1(0)+(factor*(v2(0)-v1(0)));
                double y = v1(1)+(factor*(v2(1)-v1(1)));
                double z = screen;
                Vector3d v3(x,y,z);
                vertices.push_back(v3);
                clipped_face.push_back(vertices.size());
            }
            else if((v1(2)<=screen) && (v2(2)>screen)){
                double factor = (screen-v1(2))/(v2(2)-v1(2));
                double x = v1(0)+(factor*(v2(0)-v1(0)));
                double y = v1(1)+(factor*(v2(1)-v1(1)));
                double z = screen;
                Vector3d v3(x,y,z);
                vertices.push_back(v3);
                clipped_face.push_back(face[i]);
                clipped_face.push_back(vertices.size());
            }
            else if((v1(2)<=screen) && (v2(2)<=screen)){
                clipped_face.push_back(face[i]);
            }
        }
        if(clipped_face.size()>0){
            transformed_material_of_faces[clipped_face] = material_name;
            transformed_faces.push_back(clipped_face);
        }
    }
    return transformed_faces;
}

void set_image_dimension(vector<Vector3d>& vertices)
{
    if(vertices.size()>0)
    {
        double max_x = vertices[0](0), max_y = vertices[0](1);
        for(unsigned int i=0;i<vertices.size();i++)
        {
            Vector3d vertex = vertices[i];
            if(max_x<abs(vertex(0)))
                max_x = abs(vertex(0));
            if(max_y<abs(vertex(1)))
                max_y = abs(vertex(1));
        }

        IMG_WIDTH = 2*max_x + 100*(1-pow(2,max_x/(-100)));
        IMG_HEIGHT = 2*max_y + 100*(1-pow(2,max_y/(-100)));
    }
}

void write_SVG_header(ofstream& file, string saveFile) {
    string title = saveFile.substr(saveFile.find_last_of("\\/")+1);
    title = title.substr(0,title.find_last_of("."));

    file << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl
         << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"" << endl
         << " \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl
         << "<svg width=\"" << IMG_WIDTH
         << "\" height=\"" << IMG_HEIGHT <<"\"" << endl
         << "xmlns=\"http://www.w3.org/2000/svg\" "
         << "xmlns:xlink= \"http://www.w3.org/1999/xlink\">" <<endl
         << "<title>"<<title<<"</title>" << endl;
}

void write_SVG_dot(ofstream& file, Vector3d vertex, ObjectView &view){
    double delta_x = (double) (IMG_WIDTH/2);
    double delta_y = (double)(IMG_HEIGHT/2);

    double cx = delta_x+vertex[0], cy = delta_y-vertex[1];
    string fill_col = "rgb(255,0,0)";
    file<<"<circle cx=\""<<cx<<"\" cy=\""<<cy<<"\" r=\"2\""
        <<" style=\"stroke:rgb(0,0,0);stroke-width:"<<view.getStrokeWidth()
        <<";fill:"<<fill_col<<";stroke-opacity:"<<view.getStrokeOpacity()
        <<";fill-opacity:1\"/>"<<endl;
}

void write_SVG_footer(ofstream& file){
    file << "</svg>" << endl;
}

vector< vector<int> > make_edge_list(vector< vector<int> >& face_list){
    vector< vector<int> > edge_list;
    for(unsigned int i=0;i<face_list.size();i++){
        int edges = face_list[i].size();
        for(int j=0;j<edges;j++){
            vector<int> new_edge;
            int vertex1 = face_list[i][j];
            int vertex2 = face_list[i][(j+1)%edges];
            if(vertex1<vertex2){
                new_edge.push_back(vertex1);
                new_edge.push_back(vertex2);
            }
            else{
                new_edge.push_back(vertex2);
                new_edge.push_back(vertex1);
            }
            edge_list.push_back(new_edge);
        }
    }
    remove_duplicates(edge_list);
    return edge_list;
}

void remove_duplicates(vector< vector<int> >& list){
    vector< vector<int> > new_list;
    map<vector<int>,bool> entries;
    for(unsigned int i=0;i<list.size();i++){
        vector<int> edge = list[i];
        if(entries.find(edge)==entries.end()){
            entries[edge] = true;
            new_list.push_back(edge);
        }
    }
    list = new_list;
}

void write_edges(ofstream& file,vector< vector<int> >& edge_list,
    vector<Vector3d>& points, ObjectView &view){
    for(unsigned int i=0;i<edge_list.size();i++){
        vector<int> edge = edge_list[i];
        Vector3d point_1 = points[edge[0]-1];
        Vector3d point_2 = points[edge[1]-1];
        write_SVG_line(file, point_1, point_2, view);
    }
}

void write_SVG_line(ofstream& file, Vector3d p1, Vector3d p2, ObjectView &view){
    double delta_x = (double) (IMG_WIDTH/2);
    double delta_y = (double)(IMG_HEIGHT/2);
    double x1 = delta_x + p1(0), y1 = delta_y - p1(1);
    double x2 = delta_x + p2(0), y2 = delta_y - p2(1);

    file<<"<line x1=\""<<x1<<"\" y1=\""<<y1<<"\" x2=\""<<x2<<"\" y2=\""<<y2
        <<"\" style=\"stroke:rgb(0,0,0);stroke-width:"<<view.getStrokeWidth()
        <<";stroke-opacity:"<<view.getStrokeOpacity()<<";"
        <<"stroke-linecap:round;\" />"<<endl;
}

vector< pair<double,int> > get_z_list(vector< vector<int> >& faces,
                                      vector<Vector3d>& vertices){
    vector< pair<double,int> > z_list;
    for(unsigned int j=0;j<faces.size();j++){
        vector<int> face = faces[j];
        double sum = 0;
        for(unsigned int i=0;i<face.size();i++){
            sum = sum + vertices[face[i]-1](2);
        }
        sum = sum/face.size();
        z_list.push_back(make_pair(sum,j));
    }
    return z_list;
}

void write_faces(ofstream& file, vector< pair<double,int> >& z_list,
    vector< vector<int> >& face_list, vector<Vector3d>& points,
    map<vector<int>,string>& material_name_of_faces, map<string,Material>& materials,
    Light& light, ObjectView &view){

    for(unsigned int i=0;i<z_list.size();i++){
        pair<double,int> face_data = z_list[i];
        int face_no = face_data.second;
        vector<int> face = face_list[face_no];
        Material face_material = materials[material_name_of_faces[face]];
        Vector3d face_norm = get_normal(face,points);
        if(face_norm == Vector3d(0,0,0)){
            continue;
        }
        if(view.isBackFacingPolygon() || face_norm(2)>0){
            face_norm.normalize();
            Vector3i fill = get_face_color(light, face_material, face_norm);
            write_SVG_poly(file, face, points, fill, view, face_material.getOpacity());
        }
    }
}

Vector3d get_normal(vector<int>& face, vector<Vector3d>& points){
    Vector3d cross(0,0,0);
    vector<Vector3d> vertices = three_diff_vertices(face, points);
    if(vertices.size() == 0)
        return cross;
    Vector3d a = vertices[1] - vertices[0];
    Vector3d b = vertices[2] - vertices[0];
    cross = a.cross(b);
    return cross;
}

vector<Vector3d> three_diff_vertices(vector<int>& face, vector<Vector3d>& points){
    Vector3d v1 = points[face[0]-1],v2,v3;
    vector<Vector3d> vertices;
    unsigned int index = 0;
    while(index<face.size()){
        v2 = points[face[index]-1];
        if(v1==v2){
            index++;
        }
        else
            break;
    }
    if(v1 == v2){
        return vertices;
    }
    index++;
    while(index<face.size()){
        v3 = points[face[index]-1];
        if((v1==v3) || (v2==v3)){
            index++;
        }
        else
            break;
    }
    if((v1==v3) || (v2==v3)){
        return vertices;
    }
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    return vertices;
}

Vector3i get_face_color(Light& light, Material& material, Vector3d& normal){
    int illum_no = material.getIllum_no();
    Vector3d color(255,255,255);
    if(illum_no==0){
        Vector3d diffuse = normal.dot(light.getPosition())*
            get_ele_wise_prod(light.getDiffuse(),material.getDiffuse());
        color = check_color(get_ele_wise_prod(color, diffuse));
    }
    else if(illum_no == 1){
        Vector3d ambient = get_ele_wise_prod(light.getAmbient(),material.getAmbient());
        Vector3d ambient_color = check_color(get_ele_wise_prod(color, ambient));
        Vector3d diffuse = normal.dot(light.getPosition())*
            get_ele_wise_prod(light.getDiffuse(),material.getDiffuse());
        Vector3d diffuse_color = check_color(get_ele_wise_prod(color, diffuse));
        color = check_color(ambient_color+diffuse_color);
    }
    else if(illum_no == 2){
        Vector3d ambient = get_ele_wise_prod(light.getAmbient(),material.getAmbient());
        Vector3d ambient_color = check_color(get_ele_wise_prod(color, ambient));
        Vector3d diffuse = normal.dot(light.getPosition())*
            get_ele_wise_prod(light.getDiffuse(),material.getDiffuse());
        Vector3d diffuse_color = check_color(get_ele_wise_prod(color, diffuse));
        Vector3d half = get_half_vector(light.getPosition(), normal);
        Vector3d specular = pow((normal.dot(half)),material.getShininess())*
            get_ele_wise_prod(light.getSpecular(),material.getSpecular());
        Vector3d specular_color = check_color(get_ele_wise_prod(color, specular));
        color = check_color(ambient_color+diffuse_color+specular_color);
    }
    return get_floor(color);
}

Vector3d get_ele_wise_prod(Vector3d a, Vector3d b){
    Vector3d c;
    for(int i=0;i<3;i++){
        c(i) = a(i)*b(i);
    }
    return c;
}

Vector3d check_color(Vector3d color){
    for(int i=0;i<3;i++){
        if(color(i)<0)
            color(i)=0;
        else if(color(i)>255)
            color(i)=255;
    }
    return color;
}

Vector3d get_half_vector(Vector3d a, Vector3d b){
    Vector3d c = a + b;
    c.normalize();
    return c;
}

Vector3i get_floor(Vector3d a){
    Vector3i b;
    for(int i=0;i<3;i++){
        b(i) = (int)a(i);
    }
    return b;
}

void write_SVG_poly(ofstream& file, vector<int>& face,
    vector<Vector3d>& points, Vector3i fill,ObjectView &view,
    double fill_opacity){
    double delta_x = (double) (IMG_WIDTH/2);
    double delta_y = (double)(IMG_HEIGHT/2);
    string str = "<path d=\"";
    for(unsigned int i=0;i<face.size();i++){
        if(i==0)
            str += "M ";
        else
            str += "L ";
        double x = delta_x + points[face[i]-1](0), y = delta_y - points[face[i]-1](1);
        str += to_string(x)+" "+to_string(y)+" ";
    }
    string fill_col = get_fill_string(fill);
    str +="Z\"";
    str += " style=\"stroke:rgb(0,0,0);stroke-width:"+to_string(view.getStrokeWidth());
    str += ";stroke-linejoin:round;";
    str	+="stroke-opacity:"+ to_string(view.getStrokeOpacity())+";fill:";
    //str += " style=\"stroke:None;fill:";
    str += (fill_col);
    str +=";fill-opacity:"+to_string(fill_opacity)+"\" />\n";
    file.write(str.c_str(),str.length());
}

string get_fill_string(Vector3i fill){
    string fill_col = "";
    fill_col += ("rgb("+to_string(fill(0))+","
                       +to_string(fill(1))+","
                       +to_string(fill(2))+")");
    return fill_col;
}
