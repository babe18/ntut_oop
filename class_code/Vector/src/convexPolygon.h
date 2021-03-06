#ifndef POLYGON
#define POLYGON
#include "vector.h"
#include "bubbleSort.h"
#include "shape.h"
/*
static mathVector c;
static mathVector old;

bool a(mathVector a1, mathVector a2){
    double d1 = (a1-c).angle(old-c);
    double d2 = (a2-c).angle(old-c);

    return (d1 > d2);
}*/

class PolygonFactory; // forward declaration, for friend class

class Polygon : public Shape {
private:
  int _numVertices;
  mathVector * _vertices;
protected:
  Polygon(int inputVerties, mathVector * input){
      _numVertices = inputVerties;
      _vertices = new mathVector[_numVertices];
      for (int i = 0; i < _numVertices; i++) {
        _vertices[i] = input[i];
      }
  }
public:
  friend class PolygonFactory; //friend can get private variable & functino

  /*static Polygon CreatePolygon(int numVertices, mathVector * input){
      c = mathVector::centroid(input, numVertices);
      old = input[0];

      std::sort(input,input+numVertices, a);

      return Polygon(numVertices,input);
  }*/
  static Polygon CreatePolygon(int numVertices, mathVector * input){
      mathVector c = mathVector::centroid(input, numVertices);
      mathVector b = input[0] - c;
      //angleComparator comp = angleComparator(c, b);
      //std::sort(input,input+numVertices,comp);
      //BubbleSort(input,input+numVertices,comp);
      //BubbleSort(input,input+numVertices, [&c, &b](mathVector &i, mathVector &j){
      BubbleSort(input,input+numVertices, [c, b](mathVector &i, mathVector &j)mutable{
        return b.angle(i - c) < b.angle(j - c);
      });
      return Polygon(numVertices,input);
  }
  Polygon(){
    _numVertices = 0;
    _vertices = new mathVector[_numVertices];
  }
  ~Polygon() override {
    if(_vertices != NULL) {
      delete [] _vertices;
    }
  }
  int getNumberOfVerties() const {
    return _numVertices;
  }
  double getPrimeter() const override {
    double primeter = 0;
    for (int i = 0; i < _numVertices; i++) {
      primeter += (_vertices[(i+1)%_numVertices] - _vertices[i]).length();
    }
    return primeter;
  }
  double getArea() const override {
    double area = 0;
    for (int i  = 0; i < (_numVertices - 2); i++ ){
      area += _vertices[0].getArea(_vertices[i+1], _vertices[i+2]);
    }
    return area;
  }
  mathVector centroid(){
    mathVector sum(2);
    for(int i=0;i<_numVertices;i++){
      sum.component(1) += _vertices[i].component(1);
      sum.component(2) += _vertices[i].component(2);
    }
    sum.component(1) /= _numVertices;
    sum.component(2) /= _numVertices;
    return sum;
  }
  void changeNameTo(string input) override {
    _name = input;
  }
  string toString() const override{
    stringstream ss;
    ss << "Polygon " << "{\n" << _name << "\n" << _numVertices <<"\n";
    for(int i = 0 ; i < _numVertices; i++){
      ss << _vertices[i] << "\n";
    }
    ss <<"}";
    return ss.str();
  }
  void fromString(stringstream &s) override{
    string type;
    char c;
    int numVer;
    s >> type >> c;
    s >> _name;
    s >> numVer;

    if(_vertices != NULL && _numVertices != numVer) {
      // Remove old _vertices
      delete [] _vertices;
      // Update _numVertices
      _numVertices = numVer;
      // Prepare mathVectors from stringstream
      _vertices = new mathVector[_numVertices];
    }
    // Input stringstream into _vertices
    for(int i = 0 ; i < _numVertices; i++){
      s >> _vertices[i];
    }
    // You shuold do bubbleSort just in case _vertices[i] if not ordered
    mathVector cent = this->centroid();
    mathVector ref = _vertices[0] - cent;
    BubbleSort(_vertices,_vertices+_numVertices, [cent, ref](mathVector &i, mathVector &j)mutable{
      return ref.angle(i - cent) < ref.angle(j - cent);
    });

    s >> c;
  }
};

class PolygonFactory{
public:
  Polygon CreatePolygon(mathVector vertices[], int num){
    mathVector c = centroid(vertices, num);
    // try to make centrid as friend class :)
    mathVector b = vertices[0] - c;
    angleComparator comp = angleComparator(c, b);
    BubbleSort(vertices,vertices+num,comp);
    return Polygon(num,vertices);
  }
};

class Triangle : public Polygon{
private:
  mathVector _centrid;
public:
  Triangle(mathVector *v) : Polygon(3, v){
    _centrid = this->centroid();
  }

  mathVector getCentroid() const {
    return _centrid;
  }
};
#endif
