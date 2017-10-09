
#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>
enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_CubeArray,
	Primitive_CubeIndices,
	Primitive_Ray,
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;
	vec3 GetPosition() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class pCube : public Primitive
{
public :
	pCube();
	pCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
// ============================================
typedef struct
{
	float CubeVertices[108];
}CubeV;
class pCubeVArray : public Primitive
{
public:
	pCubeVArray();
	pCubeVArray(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
private:
	CubeV vertices;
	uint vbo_id;
	float num_vertex;
};
// ============================================

// ============================================
typedef struct
{
	float CubeVertices[24];
	uint  Indices[36];
}CubeI;
class pCubeIndices : public Primitive
{
public:
	pCubeIndices();
	pCubeIndices(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
private:
	CubeI vertices_indices;
};
// ============================================
class pSphere : public Primitive
{
public:
	pSphere();
	pSphere(float radius);
	pSphere(float radius,uint rings,uint sectors);
	void InnerRender() const;
public:
	float radius;
	std::vector<float> vertices;
	std::vector<uint> indices;
};

// ============================================
class pCylinder : public Primitive
{
public:
	pCylinder();
	pCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class pLine : public Primitive
{
public:
	pLine();
	pLine(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
// ============================================
class pRay : public Primitive
{
public:
	pRay();
	pRay(float ox, float oy, float oz, float dx, float dy, float dz);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class pPlane : public Primitive
{
public:
	pPlane();
	pPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};