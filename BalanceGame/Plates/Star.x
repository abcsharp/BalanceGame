xof 0303txt 0032

// Generated by 3D Rad Exporter plugin for Google SketchUp - http://www.3drad.com

template Header {
<3D82AB43-62DA-11cf-AB39-0020AF71E433>
WORD major;
WORD minor;
DWORD flags;
}
template Vector {
<3D82AB5E-62DA-11cf-AB39-0020AF71E433>
FLOAT x;
FLOAT y;
FLOAT z;
}
template Coords2d {
<F6F23F44-7686-11cf-8F52-0040333594A3>
FLOAT u;
FLOAT v;
}
template Matrix4x4 {
<F6F23F45-7686-11cf-8F52-0040333594A3>
array FLOAT matrix[16];
}
template ColorRGBA {
<35FF44E0-6C7C-11cf-8F52-0040333594A3>
FLOAT red;
FLOAT green;
FLOAT blue;
FLOAT alpha;
}
template ColorRGB {
<D3E16E81-7835-11cf-8F52-0040333594A3>
FLOAT red;
FLOAT green;
FLOAT blue;
}
template IndexedColor {
<1630B820-7842-11cf-8F52-0040333594A3>
DWORD index;
ColorRGBA indexColor;
}
template Boolean {
<4885AE61-78E8-11cf-8F52-0040333594A3>
WORD truefalse;
}
template Boolean2d {
<4885AE63-78E8-11cf-8F52-0040333594A3>
Boolean u;
Boolean v;
}
template MaterialWrap {
<4885AE60-78E8-11cf-8F52-0040333594A3>
Boolean u;
Boolean v;
}
template TextureFilename {
<A42790E1-7810-11cf-8F52-0040333594A3>
STRING filename;
}
template Material {
<3D82AB4D-62DA-11cf-AB39-0020AF71E433>
ColorRGBA faceColor;
FLOAT power;
ColorRGB specularColor;
ColorRGB emissiveColor;
[...]
}
template MeshFace {
<3D82AB5F-62DA-11cf-AB39-0020AF71E433>
DWORD nFaceVertexIndices;
array DWORD faceVertexIndices[nFaceVertexIndices];
}
template MeshFaceWraps {
<4885AE62-78E8-11cf-8F52-0040333594A3>
DWORD nFaceWrapValues;
Boolean2d faceWrapValues;
}
template MeshTextureCoords {
<F6F23F40-7686-11cf-8F52-0040333594A3>
DWORD nTextureCoords;
array Coords2d textureCoords[nTextureCoords];
}
template MeshMaterialList {
<F6F23F42-7686-11cf-8F52-0040333594A3>
DWORD nMaterials;
DWORD nFaceIndexes;
array DWORD faceIndexes[nFaceIndexes];
[Material]
}
template MeshNormals {
<F6F23F43-7686-11cf-8F52-0040333594A3>
DWORD nNormals;
array Vector normals[nNormals];
DWORD nFaceNormals;
array MeshFace faceNormals[nFaceNormals];
}
template MeshVertexColors {
<1630B821-7842-11cf-8F52-0040333594A3>
DWORD nVertexColors;
array IndexedColor vertexColors[nVertexColors];
}
template Mesh {
<3D82AB44-62DA-11cf-AB39-0020AF71E433>
DWORD nVertices;
array Vector vertices[nVertices];
DWORD nFaces;
array MeshFace faces[nFaces];
[...]
}
template FrameTransformMatrix {
<F6F23F41-7686-11cf-8F52-0040333594A3>
Matrix4x4 frameMatrix;
}
template Frame {
<3D82AB46-62DA-11cf-AB39-0020AF71E433>
[...]
}
template XSkinMeshHeader {
<3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
WORD nMaxSkinWeightsPerVertex;
WORD nMaxSkinWeightsPerFace;
WORD nBones;
}
template VertexDuplicationIndices {
<b8d65549-d7c9-4995-89cf-53a9a8b031e3>
DWORD nIndices;
DWORD nOriginalVertices;
array DWORD indices[nIndices];
}
template SkinWeights {
<6f0d123b-bad2-4167-a0d0-80224f25fabb>
STRING transformNodeName;
DWORD nWeights;
array DWORD vertexIndices[nWeights];
array FLOAT weights[nWeights];
Matrix4x4 matrixOffset;
}
Frame RAD_SCENE_ROOT {
FrameTransformMatrix {
1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
}
Frame RAD_FRAME {
FrameTransformMatrix {
1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
}
Mesh RAD_MESH {
60;
0.427975;0.025000;0.139058;,
0.101031;0.025000;0.139058;,
0.163526;0.025000;-0.053133;,
0.000000;0.025000;-0.171885;,
-0.163526;0.025000;-0.053133;,
-0.264503;0.025000;-0.364057;,
-0.101112;0.025000;0.139169;,
0.000000;0.025000;0.450000;,
0.264503;0.025000;-0.364057;,
-0.427975;0.025000;0.139058;,
0.163526;-0.025000;-0.053133;,
0.264503;0.025000;-0.364057;,
0.264503;-0.025000;-0.364057;,
0.163526;0.025000;-0.053133;,
0.264503;0.025000;-0.364057;,
0.000000;-0.025000;-0.171885;,
0.264503;-0.025000;-0.364057;,
0.000000;0.025000;-0.171885;,
0.000000;-0.025000;-0.171885;,
-0.264503;0.025000;-0.364057;,
-0.264503;-0.025000;-0.364057;,
0.000000;0.025000;-0.171885;,
-0.163526;0.025000;-0.053133;,
-0.264503;-0.025000;-0.364057;,
-0.264503;0.025000;-0.364057;,
-0.163526;-0.025000;-0.053133;,
-0.163526;-0.025000;-0.053133;,
-0.427975;0.025000;0.139058;,
-0.427975;-0.025000;0.139058;,
-0.163526;0.025000;-0.053133;,
-0.427975;0.025000;0.139058;,
-0.101112;-0.025000;0.139169;,
-0.427975;-0.025000;0.139058;,
-0.101112;0.025000;0.139169;,
0.000000;0.025000;0.450000;,
-0.101112;-0.025000;0.139169;,
-0.101112;0.025000;0.139169;,
0.000000;-0.025000;0.450000;,
0.000000;-0.025000;0.450000;,
0.101031;0.025000;0.139058;,
0.101031;-0.025000;0.139058;,
0.000000;0.025000;0.450000;,
0.101031;-0.025000;0.139058;,
0.427975;0.025000;0.139058;,
0.427975;-0.025000;0.139058;,
0.101031;0.025000;0.139058;,
0.427975;0.025000;0.139058;,
0.163526;-0.025000;-0.053133;,
0.427975;-0.025000;0.139058;,
0.163526;0.025000;-0.053133;,
-0.101112;-0.025000;0.139169;,
-0.163526;-0.025000;-0.053133;,
-0.427975;-0.025000;0.139058;,
0.427975;-0.025000;0.139058;,
0.163526;-0.025000;-0.053133;,
0.101031;-0.025000;0.139058;,
0.000000;-0.025000;-0.171885;,
-0.264503;-0.025000;-0.364057;,
0.000000;-0.025000;0.450000;,
0.264503;-0.025000;-0.364057;;
36;
3;2,1,0,
3;5,4,3,
3;6,3,4,
3;7,3,6,
3;1,3,7,
3;8,3,1,
3;2,8,1,
3;4,9,6,
3;12,11,10,
3;13,10,11,
3;16,15,14,
3;17,14,15,
3;20,19,18,
3;21,18,19,
3;24,23,22,
3;25,22,23,
3;28,27,26,
3;29,26,27,
3;32,31,30,
3;33,30,31,
3;36,35,34,
3;37,34,35,
3;40,39,38,
3;41,38,39,
3;44,43,42,
3;45,42,43,
3;48,47,46,
3;49,46,47,
3;52,51,50,
3;55,54,53,
3;57,56,51,
3;50,51,56,
3;58,50,56,
3;55,58,56,
3;59,55,56,
3;54,55,59;;
MeshNormals {
60;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
-0.000000;1.000000;-0.000000;
0.951100;-0.000000;0.308884;
0.951100;-0.000000;0.308884;
0.951100;-0.000000;0.308884;
0.951100;-0.000000;0.308884;
-0.587784;0.000000;-0.809018;
-0.587784;0.000000;-0.809018;
-0.587784;0.000000;-0.809018;
-0.587784;0.000000;-0.809018;
0.587784;0.000000;-0.809018;
0.587784;0.000000;-0.809018;
0.587784;0.000000;-0.809018;
0.587784;0.000000;-0.809018;
-0.951100;-0.000000;0.308884;
-0.951100;-0.000000;0.308884;
-0.951100;-0.000000;0.308884;
-0.951100;-0.000000;0.308884;
-0.587898;-0.000000;-0.808935;
-0.587898;-0.000000;-0.808935;
-0.587898;-0.000000;-0.808935;
-0.587898;-0.000000;-0.808935;
-0.000340;0.000000;1.000000;
-0.000340;0.000000;1.000000;
-0.000340;0.000000;1.000000;
-0.000340;0.000000;1.000000;
-0.950951;0.000000;0.309340;
-0.950951;0.000000;0.309340;
-0.950951;0.000000;0.309340;
-0.950951;0.000000;0.309340;
0.951057;0.000000;0.309017;
0.951057;0.000000;0.309017;
0.951057;0.000000;0.309017;
0.951057;0.000000;0.309017;
-0.000000;-0.000000;1.000000;
-0.000000;-0.000000;1.000000;
-0.000000;-0.000000;1.000000;
-0.000000;-0.000000;1.000000;
0.587898;-0.000000;-0.808935;
0.587898;-0.000000;-0.808935;
0.587898;-0.000000;-0.808935;
0.587898;-0.000000;-0.808935;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;
-0.000000;-1.000000;0.000000;;
36;
3;2,1,0;
3;5,4,3;
3;6,3,4;
3;7,3,6;
3;1,3,7;
3;8,3,1;
3;2,8,1;
3;4,9,6;
3;12,11,10;
3;13,10,11;
3;16,15,14;
3;17,14,15;
3;20,19,18;
3;21,18,19;
3;24,23,22;
3;25,22,23;
3;28,27,26;
3;29,26,27;
3;32,31,30;
3;33,30,31;
3;36,35,34;
3;37,34,35;
3;40,39,38;
3;41,38,39;
3;44,43,42;
3;45,42,43;
3;48,47,46;
3;49,46,47;
3;52,51,50;
3;55,54,53;
3;57,56,51;
3;50,51,56;
3;58,50,56;
3;55,58,56;
3;59,55,56;
3;54,55,59;;
}
MeshTextureCoords {
60;
17.849426,-5.474711;
4.977610,-5.474711;
7.438021,2.091840;
1.000000,6.767136;
-5.438021,2.091840;
-9.413518,14.332978;
-2.980789,-5.479086;
1.000000,-17.716535;
11.413518,14.332978;
-15.849426,-5.474711;
-2.978151,0.984252;
-15.848662,-0.984252;
-15.848662,0.984252;
-2.978151,-0.984252;
17.849419,-0.984252;
4.977615,0.984252;
17.849419,0.984252;
4.977615,-0.984252;
-2.977615,0.984252;
-15.849419,-0.984252;
-15.849419,0.984252;
-2.977615,-0.984252;
4.978151,-0.984252;
17.848662,0.984252;
17.848662,-0.984252;
4.978151,0.984252;
-2.978151,0.984252;
-15.848662,-0.984252;
-15.848662,0.984252;
-2.978151,-0.984252;
17.847564,-0.984252;
4.978926,0.984252;
17.847564,0.984252;
4.978926,-0.984252;
-15.847564,-0.984252;
-2.978926,0.984252;
-2.978926,-0.984252;
-15.847564,0.984252;
17.849426,0.984252;
4.977610,-0.984252;
4.977610,0.984252;
17.849426,-0.984252;
-2.977610,0.984252;
-15.849426,-0.984252;
-15.849426,0.984252;
-2.977610,-0.984252;
17.848662,-0.984252;
4.978151,0.984252;
17.848662,0.984252;
4.978151,-0.984252;
4.980789,-5.479086;
7.438021,2.091840;
17.849426,-5.474711;
-15.849426,-5.474711;
-5.438021,2.091840;
-2.977610,-5.474711;
1.000000,6.767136;
11.413518,14.332978;
1.000000,-17.716535;
-9.413518,14.332978;;
}
MeshMaterialList {
2;
36;
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1;
Material {
1.000000;1.000000;1.000000;1.000000;;
50.000000;
0.000000;0.000000;0.000000;;
0.000000;0.000000;0.000000;;
}
Material {
1.000000;1.000000;1.000000;1.000000;;
50.000000;
0.000000;0.000000;0.000000;;
0.000000;0.000000;0.000000;;
}
}
}
}
}
