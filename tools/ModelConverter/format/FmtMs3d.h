/*!
 * \file FmtMs3d.h
 * \date ?
 * 
 * 
 * \author ?
 */
#ifndef __FMTMS3D_H__
#define __FMTMS3D_H__

#pragma pack(1)

#ifndef byte
typedef unsigned char byte;
#endif // byte

#ifndef word
typedef unsigned short word;
#endif // word

typedef struct
{
	char    id[10];                                     // always "MS3D000000"
	int     version;                                    // 4
} ms3d_header_t;

typedef struct
{
	byte    flags;                                      // SELECTED | SELECTED2 | HIDDEN
	float   vertex[3];                                  //
	char    boneId;                                     // -1 = no bone
	byte    referenceCount;
} ms3d_vertex_t;

typedef struct
{
	word    flags;                                      // SELECTED | SELECTED2 | HIDDEN
	word    vertexIndices[3];                           //
	float   vertexNormals[3][3];                        //
	float   s[3];                                       //
	float   t[3];                                       //
	byte    smoothingGroup;                             // 1 - 32
	byte    groupIndex;                                 //
} ms3d_triangle_t;

typedef struct
{
	byte            flags;                              // SELECTED | HIDDEN
	char            name[32];                           //
	word            numtriangles;                       //
	word*			triangleIndices;					// the groups group the triangles
	char            materialIndex;                      // -1 = no material
} ms3d_group_t;

typedef struct
{
	char            name[32];                           //
	float           ambient[4];                         //
	float           diffuse[4];                         //
	float           specular[4];                        //
	float           emissive[4];                        //
	float           shininess;                          // 0.0f - 128.0f
	float           transparency;                       // 0.0f - 1.0f
	char            mode;                               // 0, 1, 2 is unused now
	char            texture[128];                        // texture.bmp
	char            alphamap[128];                       // alpha.bmp
} ms3d_material_t;

typedef struct
{
	float           time;                               // time in seconds
	float           rotation[3];                        // x, y, z angles
} ms3d_keyframe_rot_t;

typedef struct
{
	float           time;                               // time in seconds
	float           position[3];                        // local position
} ms3d_keyframe_pos_t;

typedef struct
{
	byte            flags;                              // SELECTED | DIRTY
	char            name[32];                           //
	char            parentName[32];                     //
	float           rotation[3];                        // local reference matrix
	float           position[3];

	word            numKeyFramesRot;                    //
	word            numKeyFramesTrans;                  //

	ms3d_keyframe_rot_t* keyFramesRot;      // local animation matrices
	ms3d_keyframe_pos_t* keyFramesTrans;  // local animation matrices
} ms3d_joint_t;

#pragma pack()

#endif // __FMTMS3D_H__
