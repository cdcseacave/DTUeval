#include <vector>
#if 0
#include <omp.h>
#define _USE_OPENMP
#endif

#define ASSERT(x)

struct vec3d
{
	typedef double Type;

	double x,y,z;

	vec3d() {}
	vec3d(double _x) : x(_x), y(_x), z(_x) {}
	vec3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	
	const double* ptr() const { return &x; }
	double* ptr() { return &x; }

	double Norm() const {return sqrt(x*x+y*y+z*z);}
	vec3d& operator-=(const vec3d& rhs) {x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this;} 
	vec3d operator-(const vec3d& rhs) const {vec3d ret(*this); ret-=rhs; return ret; } 
	vec3d Cross(const vec3d& Rhs) const { return vec3d(y*Rhs.z-z*Rhs.y,z*Rhs.x-x*Rhs.z, x*Rhs.y-y*Rhs.x);}
	vec3d& operator*=(const double& Rhs) {x*=Rhs;y*=Rhs;z*=Rhs; return *this;}
	vec3d operator*(const double& Rhs) const {vec3d Ret(*this); return Ret*=Rhs;}
	vec3d& operator+=(const vec3d& Rhs) {x+=Rhs.x;y+=Rhs.y;z+=Rhs.z; return *this;}
	vec3d operator+(const vec3d& Rhs) const {vec3d Ret(*this); return Ret+=Rhs;}

	double operator[](const int i) const { return ptr()[i]; }  
	double& operator[](const int i) { return ptr()[i]; }  
};

inline vec3d operator*(const double& Lhs,const vec3d&Rhs ) 
{
	return Rhs*Lhs;
}

typedef std::vector<vec3d> PointCloud;
typedef std::vector<size_t> PointIndexArr;
