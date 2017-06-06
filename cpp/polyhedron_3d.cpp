#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <math.h>
#include <array>
#include <Eigen/Dense>

#define PI 3.14159265358979323846

using namespace Eigen;
using namespace std;

const unsigned IMG_WIDTH = 3000;
const unsigned IMG_HEIGHT = 3000;

// struct Point
// {
// 	double x;
// 	double y;
// 	double z;
// };

class Object_3D{
private:
	vector<Vector3d> vertices;
	vector< vector<int> > edges;
	vector< vector<int> > faces;
	string name;
	string type;

public:
	Object_3D(){

	}

	vector<Vector3d> getVertices(){
		return vertices;
	}

	vector< vector<int> > getEdges(){
		return edges;
	}

	vector< vector<int> > getFaces(){
		return faces;
	}

	string getType(){
		return type;
	}

	void addVertex(Vector3d point){
		vertices.push_back(point);
	}

	void addEdge(vector<int> edge){
		edges.push_back(edge);
	}

	void addFace(vector<int> face){
		faces.push_back(face);
	}

	void setName(string name){
		this->name = name;
	}

	void setType(string type){
		if(type=="-f" || type == "--face"){
			if (this->faces.size() != 0){
				this->type = "face";
			}
			else{
				try{
					throw "Face list is empty!";
				}
				catch(char const* e){
					cout<<"An error occurred: "<< e <<endl;
				}
			}
		}

		if(type=="-e" || type == "--edge"){
			if (this->edges.size() != 0){
				this->type = "edge";
			}
			else{
				try{
					throw "Edge list is empty!";
				}
				catch(char const* e){
					cout<<"An error occurred: "<< e <<endl;
				}
			}
		}
	}

};

void get_obj_data(string filename, Object_3D* obj){
	string line;
	ifstream file (filename.c_str());
	if(file.is_open()){
		while(getline(file,line)){
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss),
			     istream_iterator<string>(),
			     back_inserter(tokens));
			//cout<<"2"<<endl;
			//cout<<tokens.size()<<endl;
			if(tokens.size()>0){
				Vector3d vertex;
				vector<int> edge;
				vector<int> face;
				switch (tokens[0][0]){
					case '#':
						break;

					case 'v':
						vertex(0) = strtod(tokens[1].c_str(),NULL);
						vertex(1) = strtod(tokens[2].c_str(),NULL);
						vertex(2) = strtod(tokens[3].c_str(),NULL);
						obj->addVertex(vertex);
						break;

					case 'l':
						for(int i=1;i<tokens.size();i++){
							edge.push_back(atoi(tokens[i].c_str()));
						}
						obj->addEdge(edge);
						break;

					case 'f':
						for(int i=1;i<tokens.size();i++){
							face.push_back(atoi(tokens[i].c_str()));
						}
						obj->addFace(face);
						break;
				}
			}
		}
		file.close();
	}
	else cout<<"Unable to open file"<<endl;
}

vector< pair<string,double> > get_rotations(){
	cout<<"Enter rotation axes and angles: "<<endl;
	string rotation_string = "";
	getline(cin,rotation_string);
	//cout<<"Entered: "<<rotation_string<<endl;
	istringstream iss(rotation_string);
	vector<string> tokens;
	copy(istream_iterator<string>(iss),
	     istream_iterator<string>(),
	     back_inserter(tokens));
	vector< pair<string,double> > rotations;
	if(tokens.size()>0){
		for(int i=0;i<tokens.size();i+=2){
			string axis = tokens[i];
			double angle = strtod(tokens[i+1].c_str(),NULL);
			pair<string,double> one_rotation(axis,angle);
			rotations.push_back(one_rotation);
		}
	}
	return rotations;
}

double get_scale(){
	cout<<"Enter scale in percent: "<<endl;
	double scale;
	cin>>scale;
	return scale;
}

string get_show(){
	cout<<"What to show? Enter:\n";
	cout<<"f for faces\n"<<
		  "e for edges\n"<<
		  "v for vertices\n";
	string in = "";
	cin>>in;
	string show = "";
	if(in == "v")
		show = "vtx";
	else if(in == "e")
		show = "edg";
	else if(in == "f")
		show = "fce";
	else{
		try{
			throw "Can not show ";
		}
		catch(char const* e){
			cout<<"An error occurred: "<<e<<in<<"!"<<endl;
		}
	}
	return show;
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

Matrix3d get_transfortation_matrix(vector< pair<string,double> > rotations, double scale){
	Matrix3d trans_mat = Matrix3d::Identity(3,3);
	for(int i=0;i<rotations.size();i++){
		string axis = rotations[i].first;
		double angle = (rotations[i].second*PI)/180;
		trans_mat = rotate(trans_mat,angle,axis);
	}
	return trans_mat*scale;
}

vector<Vector3d> get_transformed_vertices(vector<Vector3d> vertices, 
	vector< pair<string,double> > rotations, double scale){
	vector<Vector3d> transformed_vertices;
	Matrix3d tranformation_matrix = get_transfortation_matrix(rotations,scale);
	for(int i=0;i<vertices.size();i++){
		transformed_vertices.push_back(tranformation_matrix*vertices[i]);
	}
	return transformed_vertices;
}

string get_fill_string(Vector3i fill){
	string fill_col = "";
	fill_col += ("rgb("+to_string(fill(0))+","
					   +to_string(fill(1))+","
					   +to_string(fill(2))+")");
	return fill_col;
}

Vector3d get_normal(vector<int> face, vector<Vector3d> points){
	Vector3d a = points[face[0]-1] - points[face[1]-1];
	Vector3d b = points[face[0]-1] - points[face[2]-1];
	return a.cross(b);
}

double get_angle(Vector3d vector1, Vector3d vector2){
	return acos(vector1.dot(vector2));
}

double get_cent_z(vector<int> face, vector<Vector3d> points){
	double sum = 0;
	for(int i=0;i<face.size();i++){
		sum += points[face[i] - 1](2);
	}
	return (sum/face.size());
}

void write_SVG_dot(ofstream& file, Vector3d vertex, Vector3i fill){
	double center_x = IMG_WIDTH/2;
	double center_y = IMG_HEIGHT/2;
	double cx = center_x+vertex[0], cy = center_y+vertex[1];
	string fill_col = get_fill_string(fill);
	file<<"<circle cx=\""<<cx<<"\" cy=\""<<cy<<"\" r=\"2\""
		<<" style=\"stroke:rgb(0,0,0);stroke-width:1;fill:"<<fill_col
		<<";stroke-opacity:1;fill-opacity:1\"/>"<<endl;
}

void write_SVG_header(ofstream& file) {
  file << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl
       << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"" << endl
       << " \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl 
       << "<svg width=\"" << IMG_WIDTH 
       << "\" height=\"" << IMG_HEIGHT << "\">" << endl
       << "<title>Polyhedron_3d</title>" << endl;
}

void write_SVG_footer(ofstream& file){
	file << "</svg>" << endl;
}

vector< vector<int> > remove_duplicates(vector< vector<int> > list){
	sort(list.begin(),list.end());
	vector<int> last = list[list.size()-1];
	for(int i=list.size()-2; i >= 0; i--){
		if(last == list[i]){
			list.erase(list.begin()+i);
		}
		else{
			last = list[i];
		}
	}
	return list;
}

vector< vector<int> > make_edge_list(vector< vector<int> > face_list){
	vector< vector<int> > edge_list;
	for(int i=0;i<face_list.size();i++){
		int edges = face_list[i].size();
		for(int j=0;j<edges;j++){
			vector<int> new_edge;
			new_edge.push_back(face_list[i][j]);
			new_edge.push_back(face_list[i][(j+1)%edges]);
			sort(new_edge.begin(),new_edge.end());
			edge_list.push_back(new_edge);
		}
	}
	return remove_duplicates(edge_list);
}

void write_SVG_line(ofstream& file, Vector3d p1, Vector3d p2){
	double center_x = IMG_WIDTH/2;
	double center_y = IMG_HEIGHT/2;
	double x1 = center_x + p1(0), y1 = center_y + p1(1);
	double x2 = center_x + p2(0), y2 = center_y + p2(1);

	file<<"<line x1=\""<<x1<<"\" y1=\""<<y1<<"\" x2=\""<<x2<<"\" y2=\""<<y2
		<<"\" style=\"stroke:rgb(0,0,0);stroke-width:2;stroke-linecap:round;\" />"<<endl;
}

void write_edges(ofstream& file,vector< vector<int> > edge_list,
	vector<Vector3d> points){
	for(int i=0;i<edge_list.size();i++){
		vector<int> edge = edge_list[i];
		Vector3d point_1 = points[edge[0]-1];
		Vector3d point_2 = points[edge[1]-1];
		write_SVG_line(file, point_1, point_2);
	}
}

Vector3i get_darkened_color(Vector3i fill_col, double factor){
	fill_col(0) = (int)(((double)fill_col(0))*factor);
	fill_col(1) = (int)(((double)fill_col(1))*factor);
	fill_col(2) = (int)(((double)fill_col(2))*factor);
	return fill_col;
}

void write_SVG_poly(ofstream& file, vector<int> face, 
	vector<Vector3d> points, Vector3i fill){
	double center_x = IMG_WIDTH/2;
	double center_y = IMG_HEIGHT/2;
	string str = "<path d=\"";
	for(int i=0;i<face.size();i++){
		if(i==0)
			str += "M ";
		else
			str += "L ";
		double x = center_x + points[face[i]-1](0), y = center_y + points[face[i]-1](1);
		str += to_string(x)+" "+to_string(y)+" ";
	}
	string fill_col = get_fill_string(fill);
	str +="Z\"";
	str += " style=\"stroke:rgb(0,0,0);stroke-width:2;stroke-linejoin:round;";
	str	+="stroke-opacity:1;fill:";
	str += (fill_col);
	str +=";fill-opacity:1\" />\n";
	file.write(str.c_str(),str.length());
}

void write_faces(ofstream& file, vector< pair< pair<double,int>, double > > z_list,
	vector< vector<int> > face_list,vector<Vector3d> points, Vector3i fill_col){
	for(int i=0;i<z_list.size();i++){
		pair< pair<double,int>, double > face_data = z_list[i];
		Vector3i fill = get_darkened_color(fill_col, (face_data.second/PI));
		int face_no = face_data.first.second;
		write_SVG_poly(file,face_list[face_no],points,fill);
	}
}

int main(int argc, char* argv[]){
	Object_3D obj;

	if ( argc == 3 ) {
		cout<<"Reading file data..."<<endl;
    	get_obj_data(argv[2], &obj);
    	obj.setType(argv[1]);
  	}

  	else if (argc == 2){
  		cout<<"Reading file data..."<<endl;
  		get_obj_data(argv[1], &obj);
  		obj.setType("--face");
  	}
  	else {
  		cout<<"usage: "<< argv[0] <<" <filename>\n";
  		return 0;
  	}

  	cout<<"File data collected."<<endl;

  	Vector3i fill_col (255,0,0);
  	Vector3d lighting (1,1,-2);
  	lighting.normalize();
  	
  	vector< pair<string,double> > rotations = get_rotations();
  	double scale = get_scale();

  	vector<Vector3d> transformed_vertices = 
  		get_transformed_vertices(obj.getVertices(),rotations,scale);

  	string show = get_show();

  	ofstream file;
  	file.open("polyhedron_3d.svg");

  	write_SVG_header(file);

  	if(show == "vtx"){
  		cout<< "Generating SVG file..."<<endl;
  		for(int i=0;i<transformed_vertices.size();i++){
  			write_SVG_dot(file,transformed_vertices[i],fill_col);
  		}
  	}
  	else if(show == "edg"){
  		vector< vector<int> > edge_list;
  		if(obj.getType() == "face"){
  			cout<<"Making Edge list..."<<endl;
  			edge_list = make_edge_list(obj.getFaces());
  			cout<<"Edge list generated."<<endl;
  		}
  		else{
  			edge_list = obj.getEdges();
  		}
  		/*for(int i=0;i<edge_list.size();i++){
  			for(int j=0;j<edge_list[i].size();j++){
  				cout<<edge_list[i][j]<<" ";
  			}
  			cout<<endl;
  		}*/
  		cout<< "Generating SVG file..."<<endl;
  		write_edges(file,edge_list,transformed_vertices);
  	}
  	else if(show == "fce"){
  		if(obj.getType() == "face"){
  			vector< vector<int> > face_list = obj.getFaces();
  			vector< pair< pair<double,int>, double > > z_list;
  			cout<<"Making face list..."<<endl;
  			for(int i=0;i<face_list.size();i++){
  				vector<int> face = face_list[i];
  				Vector3d norm = get_normal(face,transformed_vertices);
  				norm.normalize();
  				double angle = get_angle(norm,lighting);
  				double z_sort_param = get_cent_z(face,transformed_vertices);
  				pair<double,int> pair1(z_sort_param,i);
  				z_list.push_back(make_pair(pair1,angle));
  			}
  			cout<<"Face list completed"<<endl;
  			cout<<"Sorting faces..."<<endl;
  			sort(z_list.begin(),z_list.end());
  			cout<<"Faces sorted..."<<endl;
  			// for(int i=0;i<z_list.size();i++){
  			// 	cout<<z_list[i].first.first<<" "<<z_list[i].first.second<<" "
  			// 		<<z_list[i].second<<endl;
  			// }
  			cout<< "Generating SVG file..."<<endl;
  			write_faces(file,z_list,face_list,transformed_vertices,fill_col);
  		}
  		else{
  			try{
  				throw "Face Data Not Found. Ensure file contains face data.";
  			}
  			catch(char const* e){
  				cout<<"An error occurred: "<<e<<endl;
  			}
  		}
  	}

  	write_SVG_footer(file);
  	file.close();
  	cout<<"SVG file generated."<<endl;
}