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
 145;
 -1.14318;-0.37814;0.85446;,
 -0.41636;-0.37179;1.38254;,
 -0.69137;3.25800;2.13141;,
 -1.81188;3.24822;1.31729;,
 0.48203;-0.36395;1.38254;,
 0.69365;3.27009;2.13141;,
 1.20885;-0.35761;0.85446;,
 1.81416;3.27987;1.31729;,
 1.48647;-0.35519;-0.00000;,
 2.24215;3.28360;0.00000;,
 1.20885;-0.35761;-0.85446;,
 1.81416;3.27987;-1.31729;,
 0.48203;-0.36395;-1.38254;,
 0.69365;3.27009;-2.13141;,
 -0.41636;-0.37179;-1.38254;,
 -0.69137;3.25800;-2.13141;,
 -1.14318;-0.37814;-0.85446;,
 -1.81188;3.24822;-1.31729;,
 -1.42080;-0.38056;0.00000;,
 -2.23987;3.24449;0.00000;,
 -1.14318;-0.37814;0.85446;,
 -1.81188;3.24822;1.31729;,
 -0.92320;8.60521;2.70128;,
 -2.34330;8.59281;1.66948;,
 0.83213;8.62052;2.70128;,
 2.25222;8.63291;1.66948;,
 2.79464;8.63765;0.00000;,
 2.25222;8.63291;-1.66948;,
 0.83213;8.62052;-2.70128;,
 -0.92320;8.60521;-2.70128;,
 -2.34330;8.59281;-1.66948;,
 -2.88572;8.58808;0.00000;,
 -2.34330;8.59281;1.66948;,
 -0.64669;10.52280;1.79878;,
 -1.59232;10.51455;1.11171;,
 0.52219;10.53300;1.79878;,
 1.46782;10.54126;1.11171;,
 1.82903;10.54440;0.00000;,
 1.46782;10.54126;-1.11171;,
 0.52219;10.53300;-1.79878;,
 -0.64669;10.52280;-1.79878;,
 -1.59232;10.51455;-1.11171;,
 -1.95353;10.51140;0.00000;,
 -1.59232;10.51455;1.11171;,
 -0.41636;-0.37179;1.38254;,
 -1.14318;-0.37814;0.85446;,
 0.03284;-0.36787;-0.00000;,
 0.48203;-0.36395;1.38254;,
 1.20885;-0.35761;0.85446;,
 1.48647;-0.35519;-0.00000;,
 1.20885;-0.35761;-0.85446;,
 0.48203;-0.36395;-1.38254;,
 -0.41636;-0.37179;-1.38254;,
 -1.14318;-0.37814;-0.85446;,
 -1.42080;-0.38056;0.00000;,
 -1.59232;10.51455;1.11171;,
 -0.64669;10.52280;1.79878;,
 -0.06225;10.52790;0.00000;,
 0.52219;10.53300;1.79878;,
 1.46782;10.54126;1.11171;,
 1.82903;10.54440;0.00000;,
 1.46782;10.54126;-1.11171;,
 0.52219;10.53300;-1.79878;,
 -0.64669;10.52280;-1.79878;,
 -1.59232;10.51455;-1.11171;,
 -1.95353;10.51140;0.00000;,
 -0.39586;-3.50834;0.45608;,
 0.06021;-3.50436;0.64500;,
 0.05439;-2.83799;1.19180;,
 -0.78831;-2.84534;0.84273;,
 0.51627;-3.50038;0.45608;,
 0.89709;-2.83063;0.84273;,
 0.70518;-3.49874;-0.00000;,
 1.24615;-2.82759;-0.00000;,
 0.51627;-3.50038;-0.45608;,
 0.89709;-2.83063;-0.84273;,
 0.06021;-3.50436;-0.64500;,
 0.05439;-2.83799;-1.19180;,
 -0.39586;-3.50834;-0.45608;,
 -0.78831;-2.84534;-0.84273;,
 -0.58477;-3.50999;0.00000;,
 -1.13736;-2.84839;0.00000;,
 -0.39586;-3.50834;0.45608;,
 -0.78831;-2.84534;0.84273;,
 0.04569;-1.84070;1.55716;,
 -1.05535;-1.85031;1.10108;,
 1.14673;-1.83110;1.10108;,
 1.60279;-1.82712;-0.00000;,
 1.14673;-1.83110;-1.10108;,
 0.04569;-1.84070;-1.55716;,
 -1.05535;-1.85031;-1.10108;,
 -1.51141;-1.85430;0.00000;,
 -1.05535;-1.85031;1.10108;,
 0.03542;-0.66432;1.68546;,
 -1.15633;-0.67472;1.19180;,
 1.22718;-0.65392;1.19180;,
 1.72082;-0.64961;-0.00000;,
 1.22718;-0.65392;-1.19180;,
 0.03542;-0.66432;-1.68546;,
 -1.15633;-0.67472;-1.19180;,
 -1.64997;-0.67903;0.00000;,
 -1.15633;-0.67472;1.19180;,
 0.02516;0.51205;1.55716;,
 -1.07588;0.50244;1.10108;,
 1.12619;0.52166;1.10108;,
 1.58226;0.52564;0.00000;,
 1.12619;0.52166;-1.10108;,
 0.02516;0.51205;-1.55716;,
 -1.07588;0.50244;-1.10108;,
 -1.53194;0.49846;0.00000;,
 -1.07588;0.50244;1.10108;,
 0.01645;1.50934;1.19180;,
 -0.82624;1.50199;0.84273;,
 0.85915;1.51670;0.84273;,
 1.20821;1.51974;0.00000;,
 0.85915;1.51670;-0.84273;,
 0.01645;1.50934;-1.19180;,
 -0.82624;1.50199;-0.84273;,
 -1.17530;1.49894;0.00000;,
 -0.82624;1.50199;0.84273;,
 0.01064;2.17571;0.64500;,
 -0.44542;2.17173;0.45608;,
 0.46670;2.17969;0.45608;,
 0.65561;2.18134;0.00000;,
 0.46670;2.17969;-0.45608;,
 0.01064;2.17571;-0.64500;,
 -0.44542;2.17173;-0.45608;,
 -0.63434;2.17008;0.00000;,
 -0.44542;2.17173;0.45608;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.06225;-3.73835;-0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;,
 0.00860;2.40970;0.00000;;
 
 114;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;3,2,22,23;,
 4;2,5,24,22;,
 4;5,7,25,24;,
 4;7,9,26,25;,
 4;9,11,27,26;,
 4;11,13,28,27;,
 4;13,15,29,28;,
 4;15,17,30,29;,
 4;17,19,31,30;,
 4;19,21,32,31;,
 4;23,22,33,34;,
 4;22,24,35,33;,
 4;24,25,36,35;,
 4;25,26,37,36;,
 4;26,27,38,37;,
 4;27,28,39,38;,
 4;28,29,40,39;,
 4;29,30,41,40;,
 4;30,31,42,41;,
 4;31,32,43,42;,
 3;44,45,46;,
 3;47,44,46;,
 3;48,47,46;,
 3;49,48,46;,
 3;50,49,46;,
 3;51,50,46;,
 3;52,51,46;,
 3;53,52,46;,
 3;54,53,46;,
 3;45,54,46;,
 3;55,56,57;,
 3;56,58,57;,
 3;58,59,57;,
 3;59,60,57;,
 3;60,61,57;,
 3;61,62,57;,
 3;62,63,57;,
 3;63,64,57;,
 3;64,65,57;,
 3;65,55,57;,
 4;66,67,68,69;,
 4;67,70,71,68;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;69,68,84,85;,
 4;68,71,86,84;,
 4;71,73,87,86;,
 4;73,75,88,87;,
 4;75,77,89,88;,
 4;77,79,90,89;,
 4;79,81,91,90;,
 4;81,83,92,91;,
 4;85,84,93,94;,
 4;84,86,95,93;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;88,89,98,97;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 4;94,93,102,103;,
 4;93,95,104,102;,
 4;95,96,105,104;,
 4;96,97,106,105;,
 4;97,98,107,106;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;103,102,111,112;,
 4;102,104,113,111;,
 4;104,105,114,113;,
 4;105,106,115,114;,
 4;106,107,116,115;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;112,111,120,121;,
 4;111,113,122,120;,
 4;113,114,123,122;,
 4;114,115,124,123;,
 4;115,116,125,124;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 3;67,66,129;,
 3;70,67,130;,
 3;72,70,131;,
 3;74,72,132;,
 3;76,74,133;,
 3;78,76,134;,
 3;80,78,135;,
 3;82,80,136;,
 3;121,120,137;,
 3;120,122,138;,
 3;122,123,139;,
 3;123,124,140;,
 3;124,125,141;,
 3;125,126,142;,
 3;126,127,143;,
 3;127,128,144;;
 
 MeshMaterialList {
  2;
  114;
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
  120;
  -0.788770;-0.218765;0.574442;,
  -0.300139;-0.214500;0.929466;,
  0.303836;-0.209229;0.929466;,
  0.792468;-0.204966;0.574442;,
  0.979109;-0.203336;-0.000000;,
  0.792468;-0.204966;-0.574442;,
  0.303836;-0.209229;-0.929466;,
  -0.300139;-0.214500;-0.929466;,
  -0.788770;-0.218765;-0.574442;,
  -0.975411;-0.220393;0.000000;,
  -0.796907;-0.168808;0.580036;,
  -0.303517;-0.164502;0.938518;,
  0.306342;-0.159180;0.938518;,
  0.799732;-0.154874;0.580036;,
  0.988191;-0.153229;-0.000000;,
  0.799732;-0.154874;-0.580036;,
  0.306342;-0.159180;-0.938518;,
  -0.303517;-0.164502;-0.938518;,
  -0.796907;-0.168808;-0.580036;,
  -0.985366;-0.170452;0.000000;,
  -0.798076;0.167696;0.578751;,
  -0.305780;0.171992;0.936439;,
  0.302731;0.177302;0.936439;,
  0.795028;0.181599;0.578750;,
  0.983068;0.183240;0.000000;,
  0.795028;0.181599;-0.578750;,
  0.302731;0.177302;-0.936439;,
  -0.305780;0.171992;-0.936439;,
  -0.798076;0.167696;-0.578751;,
  -0.986117;0.166055;0.000000;,
  -0.728752;0.437645;0.526675;,
  -0.280754;0.441555;0.852178;,
  0.273005;0.446387;0.852178;,
  0.721004;0.450296;0.526674;,
  0.892124;0.451790;0.000000;,
  0.721004;0.450296;-0.526674;,
  0.273005;0.446387;-0.852178;,
  -0.280754;0.441555;-0.852178;,
  -0.728752;0.437645;-0.526675;,
  -0.899873;0.436152;0.000000;,
  0.008726;-0.999962;-0.000000;,
  -0.008727;0.999962;0.000000;,
  -0.398205;-0.822866;0.405370;,
  0.007151;-0.819330;0.573278;,
  0.412506;-0.815790;0.405371;,
  0.580407;-0.814327;-0.000000;,
  0.412506;-0.815790;-0.405371;,
  0.007151;-0.819330;-0.573278;,
  -0.398205;-0.822866;-0.405370;,
  -0.566106;-0.824332;0.000000;,
  -0.608929;-0.503064;0.613296;,
  0.004344;-0.497712;0.867331;,
  0.617616;-0.492360;0.613296;,
  0.871642;-0.490143;-0.000000;,
  0.617616;-0.492360;-0.613296;,
  0.004344;-0.497712;-0.867331;,
  -0.608929;-0.503064;-0.613296;,
  -0.862954;-0.505282;0.000000;,
  -0.686407;-0.234341;0.688426;,
  0.001993;-0.228333;0.973581;,
  0.690392;-0.222325;0.688426;,
  0.975537;-0.219837;-0.000000;,
  0.690392;-0.222325;-0.688426;,
  0.001993;-0.228333;-0.973581;,
  -0.686407;-0.234341;-0.688426;,
  -0.971551;-0.236829;0.000000;,
  -0.707080;-0.006170;0.707107;,
  0.000000;0.000000;1.000000;,
  0.707080;0.006171;0.707107;,
  0.999962;0.008727;0.000000;,
  0.707080;0.006171;-0.707107;,
  0.000000;0.000000;-1.000000;,
  -0.707080;-0.006170;-0.707107;,
  -0.999962;-0.008726;0.000000;,
  -0.690392;0.222324;0.688426;,
  -0.001993;0.228332;0.973581;,
  0.686407;0.234339;0.688426;,
  0.971552;0.236827;0.000000;,
  0.686407;0.234339;-0.688426;,
  -0.001993;0.228332;-0.973581;,
  -0.690392;0.222324;-0.688426;,
  -0.975537;0.219836;0.000000;,
  -0.617616;0.492361;0.613296;,
  -0.004344;0.497714;0.867331;,
  0.608929;0.503065;0.613295;,
  0.862954;0.505282;0.000000;,
  0.608929;0.503065;-0.613295;,
  -0.004344;0.497714;-0.867331;,
  -0.617616;0.492361;-0.613296;,
  -0.871641;0.490144;0.000000;,
  -0.412504;0.815792;0.405370;,
  -0.007151;0.819331;0.573276;,
  0.398203;0.822867;0.405369;,
  0.566104;0.824334;0.000000;,
  0.398203;0.822867;-0.405369;,
  -0.007151;0.819331;-0.573276;,
  -0.412504;0.815792;-0.405370;,
  -0.580405;0.814328;0.000000;,
  0.008727;-0.999962;0.000000;,
  -0.008727;0.999962;0.000000;,
  0.008725;-0.999962;0.000001;,
  0.008726;-0.999962;0.000002;,
  0.008726;-0.999962;-0.000000;,
  0.008727;-0.999962;-0.000000;,
  0.008726;-0.999962;-0.000000;,
  0.008727;-0.999962;0.000000;,
  0.008726;-0.999962;0.000000;,
  0.008725;-0.999962;-0.000001;,
  0.008726;-0.999962;-0.000002;,
  0.008727;-0.999962;-0.000000;,
  -0.008726;0.999962;0.000003;,
  -0.008729;0.999962;0.000001;,
  -0.008729;0.999962;-0.000001;,
  -0.008726;0.999962;-0.000002;,
  -0.008726;0.999962;0.000000;,
  -0.008726;0.999962;0.000002;,
  -0.008729;0.999962;0.000001;,
  -0.008729;0.999962;-0.000001;,
  -0.008726;0.999962;-0.000003;,
  -0.008724;0.999962;0.000000;;
  114;
  4;0,1,11,10;,
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;6,7,17,16;,
  4;7,8,18,17;,
  4;8,9,19,18;,
  4;9,0,10,19;,
  4;10,11,21,20;,
  4;11,12,22,21;,
  4;12,13,23,22;,
  4;13,14,24,23;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  4;17,18,28,27;,
  4;18,19,29,28;,
  4;19,10,20,29;,
  4;20,21,31,30;,
  4;21,22,32,31;,
  4;22,23,33,32;,
  4;23,24,34,33;,
  4;24,25,35,34;,
  4;25,26,36,35;,
  4;26,27,37,36;,
  4;27,28,38,37;,
  4;28,29,39,38;,
  4;29,20,30,39;,
  3;100,101,40;,
  3;102,100,40;,
  3;103,102,40;,
  3;104,103,40;,
  3;105,104,40;,
  3;106,105,40;,
  3;107,106,40;,
  3;108,107,40;,
  3;109,108,40;,
  3;101,109,40;,
  3;110,111,41;,
  3;111,112,41;,
  3;112,113,41;,
  3;113,114,41;,
  3;114,115,41;,
  3;115,116,41;,
  3;116,117,41;,
  3;117,118,41;,
  3;118,119,41;,
  3;119,110,41;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,42,50,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,50,58,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,58,66,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,66,74,81;,
  4;74,75,83,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,74,82,89;,
  4;82,83,91,90;,
  4;83,84,92,91;,
  4;84,85,93,92;,
  4;85,86,94,93;,
  4;86,87,95,94;,
  4;87,88,96,95;,
  4;88,89,97,96;,
  4;89,82,90,97;,
  3;43,42,98;,
  3;44,43,98;,
  3;45,44,98;,
  3;46,45,98;,
  3;47,46,98;,
  3;48,47,98;,
  3;49,48,98;,
  3;42,49,98;,
  3;90,91,99;,
  3;91,92,99;,
  3;92,93,99;,
  3;93,94,99;,
  3;94,95,99;,
  3;95,96,99;,
  3;96,97,99;,
  3;97,90,99;;
 }
 MeshTextureCoords {
  145;
  0.375000;0.687500;,
  0.400000;0.687500;,
  0.400000;0.562187;,
  0.375000;0.562187;,
  0.425000;0.687500;,
  0.425000;0.562187;,
  0.450000;0.687500;,
  0.450000;0.562187;,
  0.475000;0.687500;,
  0.475000;0.562187;,
  0.500000;0.687500;,
  0.500000;0.562187;,
  0.525000;0.687500;,
  0.525000;0.562187;,
  0.550000;0.687500;,
  0.550000;0.562187;,
  0.575000;0.687500;,
  0.575000;0.562187;,
  0.600000;0.687500;,
  0.600000;0.562187;,
  0.625000;0.687500;,
  0.625000;0.562187;,
  0.400000;0.436873;,
  0.375000;0.436873;,
  0.425000;0.436873;,
  0.450000;0.436873;,
  0.475000;0.436873;,
  0.500000;0.436873;,
  0.525000;0.436873;,
  0.550000;0.436873;,
  0.575000;0.436873;,
  0.600000;0.436873;,
  0.625000;0.436873;,
  0.400000;0.311560;,
  0.375000;0.311560;,
  0.425000;0.311560;,
  0.450000;0.311560;,
  0.475000;0.311560;,
  0.500000;0.311560;,
  0.525000;0.311560;,
  0.550000;0.311560;,
  0.575000;0.311560;,
  0.600000;0.311560;,
  0.625000;0.311560;,
  0.548284;0.992353;,
  0.626409;0.935591;,
  0.500000;0.850000;,
  0.451716;0.992353;,
  0.373591;0.935591;,
  0.343750;0.843750;,
  0.373591;0.751909;,
  0.451716;0.695147;,
  0.548284;0.695147;,
  0.626409;0.751909;,
  0.656250;0.843750;,
  0.626409;0.064409;,
  0.548284;0.007647;,
  0.500000;0.162500;,
  0.451716;0.007647;,
  0.373591;0.064409;,
  0.343750;0.156250;,
  0.373591;0.248091;,
  0.451716;0.304853;,
  0.548284;0.304853;,
  0.626409;0.248091;,
  0.656250;0.156250;,
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
  0.937500;0.000000;;
 }
}
