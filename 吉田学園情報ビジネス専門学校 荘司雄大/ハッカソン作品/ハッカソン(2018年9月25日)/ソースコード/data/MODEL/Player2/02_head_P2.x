xof 0302txt 0064
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

Header{
1;
0;
1;
}

Mesh {
 23;
 0.00000;10.99655;0.00000;,
 5.10955;8.04655;0.00000;,
 0.00000;8.04655;-5.10955;,
 0.00000;10.99655;0.00000;,
 -0.00000;8.04655;5.10955;,
 0.00000;10.99655;0.00000;,
 -5.10955;8.04655;-0.00000;,
 0.00000;10.99655;0.00000;,
 0.00000;8.04655;-5.10955;,
 5.10955;2.14655;0.00000;,
 0.00000;2.14655;-5.10955;,
 -0.00000;2.14655;5.10955;,
 -5.14806;2.16879;-0.00000;,
 0.00000;2.14655;-5.10955;,
 0.00000;2.14655;-5.10955;,
 5.10955;2.14655;0.00000;,
 0.03851;0.06576;0.00000;,
 -0.00000;2.14655;5.10955;,
 0.03851;0.06576;0.00000;,
 -5.14806;2.16879;-0.00000;,
 0.03851;0.06576;0.00000;,
 0.00000;2.14655;-5.10955;,
 0.03851;0.06576;0.00000;;
 
 12;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 4;2,1,9,10;,
 4;1,4,11,9;,
 4;4,6,12,11;,
 4;6,8,13,12;,
 3;14,15,16;,
 3;15,17,18;,
 3;17,19,20;,
 3;19,21,22;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.050980;0.000000;0.800000;1.000000;;
   1.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  0.500000;0.866025;0.000000;,
  0.000000;0.866025;-0.500000;,
  -0.000000;0.866025;0.500000;,
  -0.500000;0.866025;-0.000000;,
  0.707107;0.000000;-0.707107;,
  0.707107;0.000000;0.707107;,
  -0.705771;0.002308;0.708436;,
  0.001740;-0.999999;-0.000000;,
  -0.705776;0.002317;-0.708431;,
  0.001612;-0.926142;-0.377171;,
  0.379614;-0.925145;0.000000;,
  0.001612;-0.926142;0.377171;,
  -0.375761;-0.926716;-0.000000;;
  12;
  3;0,0,1;,
  3;0,2,0;,
  3;3,3,2;,
  3;3,1,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;8,8,8,8;,
  3;9,10,7;,
  3;10,11,7;,
  3;11,12,7;,
  3;12,9,7;;
 }
 MeshTextureCoords {
  23;
  0.125000;0.000000;,
  0.250000;0.333333;,
  0.000000;0.333333;,
  0.375000;0.000000;,
  0.500000;0.333333;,
  0.625000;0.000000;,
  0.750000;0.333333;,
  0.875000;0.000000;,
  1.000000;0.333333;,
  0.250000;0.666667;,
  0.000000;0.666667;,
  0.500000;0.666667;,
  0.750000;0.666667;,
  1.000000;0.666667;,
  0.000000;0.666667;,
  0.250000;0.666667;,
  0.125000;1.000000;,
  0.500000;0.666667;,
  0.375000;1.000000;,
  0.750000;0.666667;,
  0.625000;1.000000;,
  1.000000;0.666667;,
  0.875000;1.000000;;
 }
}
