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
 123;
 -0.60000;-3.95688;0.65956;,
 0.01559;-3.95688;0.93276;,
 0.01559;-3.17283;1.72351;,
 -1.12187;-3.17283;1.21871;,
 0.63118;-3.95688;0.65956;,
 1.15305;-3.17283;1.21871;,
 0.88616;-3.95688;-0.00000;,
 1.62420;-3.17283;-0.00000;,
 0.63118;-3.95688;-0.65956;,
 1.15305;-3.17283;-1.21871;,
 0.01559;-3.95688;-0.93276;,
 0.01559;-3.17283;-1.72351;,
 -0.60000;-3.95688;-0.65956;,
 -1.12187;-3.17283;-1.21871;,
 -0.85498;-3.95688;0.00000;,
 -1.59302;-3.17283;0.00000;,
 -0.60000;-3.95688;0.65956;,
 -1.12187;-3.17283;1.21871;,
 0.01559;-1.99943;2.25188;,
 -1.47057;-1.99943;1.59232;,
 1.50175;-1.99943;1.59232;,
 2.11734;-1.99943;-0.00000;,
 1.50175;-1.99943;-1.59232;,
 0.01559;-1.99943;-2.25188;,
 -1.47057;-1.99943;-1.59232;,
 -2.08616;-1.99943;0.00000;,
 -1.47057;-1.99943;1.59232;,
 0.01559;-0.61531;2.43741;,
 -1.59302;-0.61531;1.72351;,
 1.62420;-0.61531;1.72351;,
 2.29051;-0.61531;-0.00000;,
 1.62420;-0.61531;-1.72351;,
 0.01559;-0.61531;-2.43741;,
 -1.59302;-0.61531;-1.72351;,
 -2.25933;-0.61531;0.00000;,
 -1.59302;-0.61531;1.72351;,
 0.01559;0.76880;2.25188;,
 -1.47057;0.76880;1.59232;,
 1.50175;0.76880;1.59232;,
 2.11734;0.76880;0.00000;,
 1.50175;0.76880;-1.59232;,
 0.01559;0.76880;-2.25188;,
 -1.47057;0.76880;-1.59232;,
 -2.08616;0.76880;0.00000;,
 -1.47057;0.76880;1.59232;,
 0.01559;1.94220;1.72351;,
 -1.12187;1.94220;1.21871;,
 1.15305;1.94220;1.21871;,
 1.62420;1.94220;0.00000;,
 1.15305;1.94220;-1.21871;,
 0.01559;1.94220;-1.72351;,
 -1.12187;1.94220;-1.21871;,
 -1.59302;1.94220;0.00000;,
 -1.12187;1.94220;1.21871;,
 0.01559;2.72624;0.93276;,
 -0.60000;2.72624;0.65956;,
 0.63118;2.72624;0.65956;,
 0.88616;2.72624;0.00000;,
 0.63118;2.72624;-0.65956;,
 0.01559;2.72624;-0.93276;,
 -0.60000;2.72624;-0.65956;,
 -0.85498;2.72624;0.00000;,
 -0.60000;2.72624;0.65956;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;-4.23220;-0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 0.01559;3.00156;0.00000;,
 -1.70510;-9.64827;1.22663;,
 -0.66167;-9.64827;1.98472;,
 -0.76981;-1.07376;2.31754;,
 -1.98822;-1.07376;1.43232;,
 0.62808;-9.64827;1.98472;,
 0.73622;-1.07376;2.31754;,
 1.67151;-9.64827;1.22663;,
 1.95462;-1.07376;1.43232;,
 2.07006;-9.64827;-0.00000;,
 2.42002;-1.07376;-0.00000;,
 1.67151;-9.64827;-1.22663;,
 1.95462;-1.07376;-1.43232;,
 0.62808;-9.64827;-1.98472;,
 0.73622;-1.07376;-2.31754;,
 -0.66167;-9.64827;-1.98472;,
 -0.76981;-1.07376;-2.31754;,
 -1.70510;-9.64827;-1.22663;,
 -1.98822;-1.07376;-1.43232;,
 -2.10366;-9.64827;0.00000;,
 -2.45361;-1.07376;0.00000;,
 -1.70510;-9.64827;1.22663;,
 -1.98822;-1.07376;1.43232;,
 -0.66167;-9.64827;1.98472;,
 -1.70510;-9.64827;1.22663;,
 -0.01680;-9.64827;0.00000;,
 0.62808;-9.64827;1.98472;,
 1.67151;-9.64827;1.22663;,
 2.07006;-9.64827;-0.00000;,
 1.67151;-9.64827;-1.22663;,
 0.62808;-9.64827;-1.98472;,
 -0.66167;-9.64827;-1.98472;,
 -1.70510;-9.64827;-1.22663;,
 -2.10366;-9.64827;0.00000;,
 -1.98822;-1.07376;1.43232;,
 -0.76981;-1.07376;2.31754;,
 -0.01680;-1.07376;0.00000;,
 0.73622;-1.07376;2.31754;,
 1.95462;-1.07376;1.43232;,
 2.42002;-1.07376;-0.00000;,
 1.95462;-1.07376;-1.43232;,
 0.73622;-1.07376;-2.31754;,
 -0.76981;-1.07376;-2.31754;,
 -1.98822;-1.07376;-1.43232;,
 -2.45361;-1.07376;0.00000;;
 
 94;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;37,36,45,46;,
 4;36,38,47,45;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;46,45,54,55;,
 4;45,47,56,54;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 3;1,0,63;,
 3;4,1,64;,
 3;6,4,65;,
 3;8,6,66;,
 3;10,8,67;,
 3;12,10,68;,
 3;14,12,69;,
 3;16,14,70;,
 3;55,54,71;,
 3;54,56,72;,
 3;56,57,73;,
 3;57,58,74;,
 3;58,59,75;,
 3;59,60,76;,
 3;60,61,77;,
 3;61,62,78;,
 4;79,80,81,82;,
 4;80,83,84,81;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 4;87,89,90,88;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 4;95,97,98,96;,
 4;97,99,100,98;,
 3;101,102,103;,
 3;104,101,103;,
 3;105,104,103;,
 3;106,105,103;,
 3;107,106,103;,
 3;108,107,103;,
 3;109,108,103;,
 3;110,109,103;,
 3;111,110,103;,
 3;102,111,103;,
 3;112,113,114;,
 3;113,115,114;,
 3;115,116,114;,
 3;116,117,114;,
 3;117,118,114;,
 3;118,119,114;,
 3;119,120,114;,
 3;120,121,114;,
 3;121,122,114;,
 3;122,112,114;;
 
 MeshMaterialList {
  2;
  94;
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
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game127\\Desktop\\�Q�[���f��\\2�N��\\2018�N�A�E��i�p\\���f��\\obj\\player\\face.jpg";
   }
  }
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  68;
  -0.376470;-0.855984;0.354347;,
  0.000000;-0.863215;0.504837;,
  0.376470;-0.855984;0.354347;,
  0.528536;-0.848911;-0.000000;,
  0.376470;-0.855984;-0.354347;,
  0.000000;-0.863215;-0.504837;,
  -0.376470;-0.855984;-0.354347;,
  -0.528536;-0.848911;0.000000;,
  -0.601959;-0.559135;0.570099;,
  0.000000;-0.572116;0.820172;,
  0.601959;-0.559135;0.570099;,
  0.837300;-0.546744;-0.000000;,
  0.601959;-0.559135;-0.570099;,
  0.000000;-0.572116;-0.820172;,
  -0.601959;-0.559135;-0.570099;,
  -0.837300;-0.546745;0.000000;,
  -0.698377;-0.266874;0.664114;,
  -0.000000;-0.275482;0.961306;,
  0.698377;-0.266874;0.664114;,
  0.965934;-0.258788;-0.000000;,
  0.698377;-0.266874;-0.664114;,
  -0.000000;-0.275482;-0.961306;,
  -0.698377;-0.266874;-0.664114;,
  -0.965934;-0.258788;0.000000;,
  -0.724241;0.000000;0.689547;,
  -0.000000;0.000000;1.000000;,
  0.724241;0.000000;0.689547;,
  1.000000;0.000000;0.000000;,
  0.724241;0.000000;-0.689547;,
  -0.000000;0.000000;-1.000000;,
  -0.724241;0.000000;-0.689547;,
  -1.000000;0.000000;0.000000;,
  -0.698377;0.266875;0.664114;,
  -0.000000;0.275482;0.961306;,
  0.698377;0.266875;0.664114;,
  0.965934;0.258789;0.000000;,
  0.698377;0.266875;-0.664114;,
  -0.000000;0.275482;-0.961306;,
  -0.698377;0.266875;-0.664114;,
  -0.965934;0.258789;0.000000;,
  -0.601958;0.559137;0.570098;,
  0.000000;0.572119;0.820171;,
  0.601958;0.559137;0.570098;,
  0.837298;0.546747;0.000000;,
  0.601958;0.559137;-0.570098;,
  0.000000;0.572119;-0.820171;,
  -0.601958;0.559137;-0.570098;,
  -0.837298;0.546747;0.000000;,
  -0.376469;0.855985;0.354346;,
  0.000000;0.863216;0.504835;,
  0.376469;0.855985;0.354346;,
  0.528534;0.848912;0.000000;,
  0.376469;0.855985;-0.354346;,
  0.000000;0.863216;-0.504835;,
  -0.376469;0.855985;-0.354346;,
  -0.528534;0.848912;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.808342;-0.040779;0.587299;,
  -0.308758;-0.040779;0.950266;,
  0.308759;-0.040778;0.950266;,
  0.808343;-0.040779;0.587298;,
  0.999168;-0.040779;-0.000000;,
  0.808343;-0.040779;-0.587298;,
  0.308759;-0.040778;-0.950266;,
  -0.308758;-0.040779;-0.950266;,
  -0.808342;-0.040779;-0.587299;,
  -0.999168;-0.040779;0.000000;;
  94;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  3;1,0,56;,
  3;2,1,56;,
  3;3,2,56;,
  3;4,3,56;,
  3;5,4,56;,
  3;6,5,56;,
  3;7,6,56;,
  3;0,7,56;,
  3;48,49,57;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,48,57;,
  4;58,59,59,58;,
  4;59,60,60,59;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;62,63,63,62;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,66,66,65;,
  4;66,67,67,66;,
  4;67,58,58,67;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;,
  3;57,57,57;;
 }
 MeshTextureCoords {
  123;
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.875000;,
  0.250000;0.750000;,
  0.375000;0.875000;,
  0.375000;0.750000;,
  0.500000;0.875000;,
  0.500000;0.750000;,
  0.625000;0.875000;,
  0.625000;0.750000;,
  0.750000;0.875000;,
  0.750000;0.750000;,
  0.875000;0.875000;,
  0.875000;0.750000;,
  1.000000;0.875000;,
  1.000000;0.750000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  1.000000;0.125000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.375000;0.687500;,
  0.400000;0.687500;,
  0.400000;0.311560;,
  0.375000;0.311560;,
  0.425000;0.687500;,
  0.425000;0.311560;,
  0.450000;0.687500;,
  0.450000;0.311560;,
  0.475000;0.687500;,
  0.475000;0.311560;,
  0.500000;0.687500;,
  0.500000;0.311560;,
  0.525000;0.687500;,
  0.525000;0.311560;,
  0.550000;0.687500;,
  0.550000;0.311560;,
  0.575000;0.687500;,
  0.575000;0.311560;,
  0.600000;0.687500;,
  0.600000;0.311560;,
  0.625000;0.687500;,
  0.625000;0.311560;,
  0.548280;0.992350;,
  0.626410;0.935590;,
  0.500000;0.850000;,
  0.451720;0.992350;,
  0.373590;0.935590;,
  0.343750;0.843750;,
  0.373590;0.751910;,
  0.451720;0.695150;,
  0.548280;0.695150;,
  0.626410;0.751910;,
  0.656250;0.843750;,
  0.626410;0.064410;,
  0.548280;0.007650;,
  0.500000;0.162500;,
  0.451720;0.007650;,
  0.373590;0.064410;,
  0.343750;0.156250;,
  0.373590;0.248090;,
  0.451720;0.304850;,
  0.548280;0.304850;,
  0.626410;0.248090;,
  0.656250;0.156250;;
 }
}
