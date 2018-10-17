/*
	Vertex data structs
*/


#ifndef VERTEX_H
#define VERTEX_H

typedef struct tVertex
{
	float x, y, z;
} Vertex, NormalT;

struct Interleaved
{
	float u, v;
	int x, y, z;
};

struct vertexNoNormal
{
	float u, v;
	float x, y, z;
};

struct UVMap
{
	float u, v;
};

#endif