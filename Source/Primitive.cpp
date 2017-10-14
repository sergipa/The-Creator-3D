
#include "Globals.h"
#include "OpenGL.h"
#include "Primitive.h"
// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

vec3 Primitive::GetPosition() const
{
	vec3 pos;
	pos.x = transform.M[12];
	pos.y = transform.M[13];
	pos.z = transform.M[14];
	return pos;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	//if(wire)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
pCube::pCube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
}

pCube::pCube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

void pCube::InnerRender() const
{	
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();
}
// CUBEVArray ============================================
pCubeVArray::pCubeVArray() : Primitive()//Default Size 10.f
{
	type = PrimitiveTypes::Primitive_CubeArray;
	vertices =
	{
		-5.f, -5.f, 5.f,//o
		5.f, -5.f, 5.f,
		5.f, 5.f, 5.f,

		-5.f, -5.f, 5.f,//o
		5.f, 5.f, 5.f,
		-5.f, 5.f, 5.f,
		//cara2
		-5.f, -5.f, 5.f,//o
		-5.f, 5.f, -5.f,
		-5.f, -5.f, -5.f,

		-5.f, -5.f, 5.f,//o
		-5.f, 5.f, 5.f,
		-5.f, 5.f, -5.f,
		//cara3
		5.f, -5.f, 5.f,
		5.f, -5.f, -5.f,
		5.f, 5.f, 5.f,

		5.f, -5.f, -5.f,
		5.f, 5.f, -5.f,
		5.f, 5.f, 5.f,
		//cara4
		-5.f, -5.f, 5.f,//o
		-5.f, -5.f, -5.f,
		5.f, -5.f, -5.f,

		-5.f, -5.f, 5.f,
		5.f, -5.f, -5.f,
		5.f, -5.f, 5.f,
		//cara5
		-5.f, -5.f, -5.f,
		-5.f, 5.f, -5.f,
		5.f, -5.f, -5.f,

		5.f, 5.f, -5.f,
		5.f, -5.f, -5.f,
		-5.f, 5.f, -5.f,
		//cara 6
		-5.f, 5.f, 5.f,
		5.f, 5.f, 5.f,
		-5.f, 5.f, -5.f,

		5.f, 5.f, 5.f,
		5.f, 5.f, -5.f,
		-5.f, 5.f, -5.f,
	};
	vbo_id = 0;
	num_vertex = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex * 3, vertices.CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

pCubeVArray::pCubeVArray(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_CubeArray;
	vertices =
	{
		-sizeX / 2, -sizeY / 2, sizeZ / 2,
		sizeX/2, -sizeY / 2, sizeZ / 2,
		sizeX/2, sizeY/2, sizeZ / 2,

		-sizeX / 2, -sizeY / 2, sizeZ / 2,
		sizeX / 2, sizeY / 2, sizeZ / 2,
		-sizeX / 2, sizeY / 2, sizeZ / 2,
		//cara2
		-sizeX / 2, -sizeY / 2, sizeZ / 2,
		-sizeX / 2, sizeY / 2, -sizeZ/2,
		-sizeX / 2, -sizeY / 2, -sizeZ / 2,

		-sizeX / 2, -sizeY / 2, sizeZ / 2,
		-sizeX / 2, sizeY/2, sizeZ / 2,
		-sizeX / 2, sizeY/2, -sizeZ / 2,
		//cara3
		sizeX/2, -sizeY / 2, sizeZ / 2,
		sizeX/2, -sizeY / 2, -sizeZ / 2,
		sizeX/2, sizeY / 2, sizeZ / 2,

		sizeX/2, -sizeY / 2, -sizeZ / 2,
		sizeX/2, sizeY/2, -sizeZ / 2,
		sizeX/2, sizeY/2, sizeZ / 2,
		//cara4
		-sizeX / 2, -sizeY / 2, sizeZ / 2,
		-sizeX / 2, -sizeY / 2, -sizeZ / 2,
		sizeX / 2, -sizeY / 2, sizeZ / 2,

		sizeX / 2, -sizeY / 2, sizeZ / 2,
		-sizeX / 2, -sizeY / 2, -sizeZ / 2,
		sizeX / 2, -sizeY / 2, -sizeZ / 2,
		//cara5
		-sizeX / 2, -sizeY / 2, -sizeZ / 2,
		-sizeX / 2, sizeY / 2, -sizeZ / 2,
		sizeX / 2, -sizeY / 2, -sizeZ / 2,

		sizeX/2, sizeY / 2, -sizeZ / 2,
		sizeX/2, -sizeY / 2, -sizeZ / 2,
		-sizeX / 2, sizeY / 2, -sizeZ / 2,
		//cara 6
		-sizeX / 2, sizeY / 2, sizeZ / 2,
		sizeX / 2, sizeY / 2, sizeZ / 2,
		-sizeX / 2, sizeY / 2, -sizeZ / 2,

		sizeX/2, sizeY / 2, sizeZ / 2,
		sizeX/2, sizeY / 2, -sizeZ / 2,
		-sizeX / 2, sizeY / 2, -sizeZ / 2,
	};
	vbo_id = 0;
	num_vertex = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex * 3, vertices.CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void pCubeVArray::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, num_vertex * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//CUBE INDICES ==================================================
pCubeIndices::pCubeIndices()
{
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3
	type = PrimitiveTypes::Primitive_CubeIndices;
	vertices_indices = 
	{
		5.f, 5.f, 5.f, //v0
		-5.f, 5.f, 5.f, //v1
		-5.f, -5.f, 5.f, //v2
		5.f, -5.f, 5.f, //v3
		5.f, -5.f, -5.f, //v4
		5.f, 5.f, -5.f, //v5
		-5.f, 5.f, -5.f, //v6
		-5.f, -5.f, -5.f, //v7
		//INDICES
		0, 1, 2,   2, 3, 0,
		0, 3, 4,   4, 5, 0,
		0, 5, 6,   6, 1, 0,
		1, 6, 7,   7, 2, 1,
		4, 3, 2,   2, 7, 4,
		7, 6, 5,   5, 4, 7
	};
	vbo_id = 0;
	num_vertex = 24;
	ibo_id = 0;
	num_indices = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, vertices_indices.CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, vertices_indices.Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

pCubeIndices::pCubeIndices(float sizeX, float sizeY, float sizeZ)
{
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3
	type = PrimitiveTypes::Primitive_CubeIndices;
	vertices_indices =
	{
		sizeX/2, sizeY / 2, -sizeZ / 2, //v0
		-sizeX / 2, sizeY / 2, -sizeZ / 2, //v1
		-sizeX / 2, -sizeY / 2, -sizeZ / 2, //v2
		sizeX / 2, -sizeY / 2, -sizeZ / 2, //v3
		sizeX / 2, -sizeY / 2, sizeZ / 2, //v4
		sizeX / 2, sizeY / 2, sizeZ / 2, //v5
		-sizeX / 2, sizeY / 2, sizeZ / 2, //v6
		-sizeX / 2, -sizeY / 2, sizeZ / 2, //v7
		//INDICES
		2,1,0,   0,3,2,
		4,3,0,   0,5,4,
		6,5,0,   0,1,6,
		7,6,1,   1,2,7,
		2,3,4,   4,7,2,
		5,6,7,   7,4,5

	};
	vbo_id = 0;
	num_vertex = 24;
	ibo_id = 0;
	num_indices = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, vertices_indices.CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, vertices_indices.Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void pCubeIndices::InnerRender() const
{
	//VERTICES
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glDrawElements(GL_TRIANGLES, ibo_id, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// SPHERE ============================================
pSphere::pSphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;

	uint rings = 20;//DEFAULT rings
	uint sectors = 20;//DEFAULT sectors

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator v = vertices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	indices.resize(rings * sectors * 4);
	std::vector<uint>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + s;
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);
	}

	num_indices = indices.size();
	num_vertex = vertices.size();
	vbo_id = 0;
	ibo_id = 0;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

pSphere::pSphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;

	uint rings = 20;//DEFAULT rings
	uint sectors = 20;//DEFAULT sectors

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator v = vertices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	indices.resize(rings * sectors * 4);
	std::vector<uint>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + s;
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);
	}
	num_indices = indices.size();
	num_vertex = vertices.size();
	vbo_id = 0;
	ibo_id = 0;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

pSphere::pSphere(float radius, uint rings, uint sectors)
{
	type = PrimitiveTypes::Primitive_Sphere;

	num_vertex = 0;
	num_indices = 0;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator v = vertices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	indices.resize(rings * sectors * 4);
	std::vector<uint>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + s;
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);
	}
	num_indices = indices.size();
	num_vertex = vertices.size();
	vbo_id = 0;
	ibo_id = 0;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void pSphere::InnerRender() const
{
	//VERTICES
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glDrawElements(GL_QUADS, ibo_id, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// CYLINDER ============================================
pCylinder::pCylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

pCylinder::pCylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void pCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
pLine::pLine() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

pLine::pLine(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void pLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
pPlane::pPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

pPlane::pPlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void pPlane::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 2.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	glLineWidth(1.0f);
}

pRay::pRay() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Ray;
}

pRay::pRay(float ox, float oy, float oz, float dx, float dy, float dz) : Primitive(), origin(ox, oy, oz), destination(dx, dy, dz)
{
	type = PrimitiveTypes::Primitive_Ray;
}

void pRay::InnerRender() const
{
	glLineWidth(2.0f);
	vec3 dir(destination - origin);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(dir.x * 999, dir.y * 999, dir.z * 999);

	glEnd();

	glLineWidth(1.0f);
}

pFrustum::pFrustum()
{
	//    v4----- v5
	//   /|      /|
	//  v0------v1|
	//  | |     | |
	//  | |v6---|-|v7
	//  |/      |/
	//  v2------v3
	vertices_indices =
	{
		-5.f,5.f,0.f,     //v0
		 5.f,5.f,0.f,     //v1
		-5.f,-5.f,0.f,    //v2
		 5.f,-5.f,0.f,    //v3
						  
		-20.f,20.f,-20.f, //v4
		 20.f,20.f,-20.f, //v5
		-20.f,-20.f,-20.f,//v6
		 20.f,-20.f,-20.f,//v7

		 //INDICES
		 0,2,1,	2,3,1,
		 7,4,5,	7,6,4,
		 4,0,5,	5,0,1,
		 5,1,7,	7,1,3,
		 2,6,3,	3,6,7,
		 2,4,6,	2,0,4	
	};
	vbo_id = 0;
	num_vertex = 24;
	ibo_id = 0;
	num_indices = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, vertices_indices.FrustumVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, vertices_indices.Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

pFrustum::pFrustum(float w1, float h1, float l, float w2, float h2)
{
	//    v4----- v5
	//   /|      /|
	//  v0------v1|
	//  | |     | |
	//  | |v6---|-|v7
	//  |/      |/
	//  v2------v3
	vertices_indices =
	{
		-w1/2,h1/2,0.f,     //v0
		w1 / 2,h1 / 2,0.f,  //v1
		-w1 / 2,-h1 / 2,0.f,//v2
		w1 / 2,-h1 / 2,0.f, //v3

		-w2/2,h2/2,-l,		//v4
		w2 / 2,h2 / 2,-l,   //v5
		-w2 / 2,-h2 / 2,-l, //v6
		w2 / 2,-h2 / 2,-l,  //v7

		//INDICES
		0,2,1,	2,3,1,
		7,4,5,	7,6,4,
		4,0,5,	5,0,1,
		5,1,7,	7,1,3,
		2,6,3,	3,6,7,
		2,4,6,	2,0,4
	};
	vbo_id = 0;
	num_vertex = 24;
	ibo_id = 0;
	num_indices = 36;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex, vertices_indices.FrustumVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*num_indices, vertices_indices.Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void pFrustum::InnerRender() const
{
	//VERTICES
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glDrawElements(GL_TRIANGLES, ibo_id, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
