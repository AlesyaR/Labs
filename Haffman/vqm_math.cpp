//The Eternal Blood River Engine source file
//Started 29.08.2012 by Fedor Igumnov. Moscow Russia.
//mailto:igumnovfedor@gmail.com
//Propietary code. Contact me if you find this useful.

/*Awaken my child, and embrace the glory that is your birthright
Know that I am the Overmind. The ETERNAL will of the swarm. 
And that you have been created to serve me. 
										Overmind(Надмозг)
														 */
//TODO: Наращивать функционал, оптимизировать
//Компрессия матриц 
//inline?
//Дополнительные функции кватернионов, больше интерполяций


#include <math.h>
#include "vqm_math.h"
//Краткое описание
//Universal vertex quaternion math module 
// Polish notation function naming convection: 
    //	(return type)_(arg 1) [arg 2] (Operand)  
    //return types and args:
    //
    //Q - quaternion (w,x,y,z) 
    //M - OpenGL 4x4 matrix 
    //V4 - OpenGl Vecotr 4x 
	//V3 - Vector 3x
    //S - Real Scalar 
    //A - angle in radians
    //RPY - roll pitch yaw
	//Slerp -  spherical linear interpolation

//Cheat Sheets:
//Matrix Indexes:
//  0 4 8  12
//	1 5 9  13
//	2 6 10 14
//	3 7 11 15
//Identity matrix:
//1 0 0 0
//0 1 0 0
//0 0 1 0
//0 0 0 1
//Axis matrix:
// xx xy xz    0 4 8
// yx yy yz    1 5 9
// zx zy zz    2 6 10

//Quaternion: w, x, y , z
//Identity Quaternion; 1, 0, 0 , 0
//Identity Vector: 1,1,1,1;
//Axis Angle Vector: Angle,x,y,z 
//RPY Vector: Roll,Pitch,Yew
//Можно применять для доступа к отдельному элементу матрицы 4*4 
const char ijToN[4][4]={{0,4,8,12},{1,5,9,13},{2,6,10,14},{3,7,11,15}};
const char T_ijToN[4][4]={{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};

//Настройка SLERP
const float DELTA =0.01; 

//inline void  M4_M4_SetIdentity ( M& const dest)
inline void  vqm_math::M4_M4_SetIdentity(M4& dest)
{
	dest[0]=1;
	dest[1]=0;
	dest[2]=0;
	dest[3]=0;
	dest[4]=0;
	dest[5]=1;
	dest[6]=0;
	dest[7]=0;
	dest[8]=0;
	dest[9]=0;
	dest[10]=1;
	dest[11]=0;
	dest[12]=0;
	dest[13]=0;
	dest[14]=0;
	dest[15]=1;
}

inline void  vqm_math::M4_V3_SetScaleM(M4& dest ,V3& const src)
{
	dest[0]=src[0];
	dest[5]=src[1];
	dest[10]=src[2];
}

//V[0] - angle V[1] = x V[2] = y V[3]= z
inline void  vqm_math::M4_V4_SetRotate(M4& dest, V4& const  src)
{
	
	float cosA = cos(src[0]);
	float sinA = sin(src[0]);
	float x = src[1];
	float y= src[2];
	float z= src[3];
	dest[0]=cosA+ (1-cosA)*x*x;
	dest[1]=(1-cosA)*y*x+sinA*z;
	dest[2]=(1-cosA)*z*x-sinA*y;
	//dest[3]=0;
	dest[4]=(1-cosA)*x*y-sinA*z;
	dest[5]=cosA+ (1-cosA)*y*y;
	dest[6]=(1-cosA)*z*y+sinA*x;
	//dest[7]=0;
	dest[8]=(1-cosA)*x*z+sinA*y;
	dest[9]=(1-cosA)*y*z-sinA*x;
	dest[10]=cosA+ (1-cosA)*z*z;
	//dest[11]=0;
	//dest[12]=0;
	//dest[13]=0;
	//dest[14]=0;
	//dest[15]=0;
}
//V[0] = alfa(прецессия) V[1] = beta(нутация) V[2]= gamma(угол собственного вращения) 

inline void vqm_math::M4_V4_SetRotateEuler(M4& dest, V3& const src)
{

	float CosAlfa = cos(src[0]);
	float CosBeta= cos(src[1]);
	float CosGamma= cos(src[2]);
	float SinAlfa =sin(src[0]);
	float SinBeta = sin(src[1]);
	float SinGamma = sin(src[2]);
	dest[0]=CosAlfa*CosGamma-SinAlfa*CosBeta*SinGamma;
	dest[1]=SinAlfa*CosGamma+CosAlfa*CosBeta*SinGamma;
	dest[2]=SinBeta*SinGamma;
	//dest[3]=0;
	dest[4]=-CosAlfa*SinGamma-SinAlfa*CosBeta*CosGamma;
	dest[5]=-SinAlfa*SinGamma+CosAlfa*CosBeta*CosGamma;
	dest[6]=SinBeta*CosGamma;
	//dest[7]=0;
	dest[8]=SinAlfa*SinBeta;
	dest[9]=-CosAlfa*SinBeta;
	dest[10]=CosBeta;
	//dest[11]=0;
	//dest[12]=0;
	//dest[13]=0;
	//dest[14]=0;
	//dest[15]=1;

}

//V[0]=x V[1]=y V[2]=z
inline void vqm_math::M4_M4V3_SetTranslate(M4& dest, V3& const src)
{
	
	//Cheat Sheet
	//  0 4 8  12
	//	1 5 9  13
	//	2 6 10 14
	//	3 7 11 15
	dest[3]=src[0];
	dest[7]=src[1];
	dest[11]=src[2];
}

inline void vqm_math::M4_M4M4_Multiply(M4& dest,M4& const src1, M4& const src2)
{
	//Cheat Sheet
	//  0 4 8  12    0 4 8  12    0*0 + 4*1 + 8*2 + 12*3  | 0*4 + 4*5 + 8*6 +  12*7 | 0*8 + 4*9 + 8*10 +  12*11 | 0*12 + 4*13 +  8*14 + 12*15
	//	1 5 9  13  * 1 5 9  13  = 1*0 + 5*1 + 9*2 + 13*3  | 1*4 + 5*5 + 9*6 +  13*7 | 1*8 + 5*9 + 9*10 +  13*11 | 1*12 + 5*13 +  9*14 + 13*15
	//	2 6 10 14    2 6 10 14    2*0 + 6*1 + 10*2 + 12*3 | 2*4 + 6*5 + 10*6 + 14*7 | 2*8 + 6*9 + 10*10 + 14*11 | 2*12 + 6*13 + 10*14 + 14*15
	//	3 7 11 15    3 7 11 15    3*0 + 7*1 + 11*2 + 12*3 | 3*4 + 7*5 + 11*6 + 15*7 | 3*8 + 7*9 + 11*10 + 15*11 | 3*12 + 7*13 + 11*14 + 15*15
	dest[0]=src1[0]* src2[0]+src1[4]* src2[1]+src1[8]* src2[2]+src1[12]* src2[3];
	dest[1]=src1[1]* src2[0]+src1[5]* src2[1]+src1[9]* src2[2]+src1[13]* src2[3];
	dest[2]=src1[2]* src2[0]+src1[6]* src2[1]+src1[10]* src2[2]+src1[14]* src2[3];
	dest[3]=src1[3]* src2[0]+src1[7]* src2[1]+src1[11]* src2[2]+src1[15]* src2[3];

	dest[4]=src1[0]* src2[4]+src1[4]* src2[5]+src1[8]* src2[6]+src1[12]* src2[7];
	dest[5]=src1[1]* src2[4]+src1[5]* src2[5]+src1[9]* src2[6]+src1[13]* src2[7];
	dest[6]=src1[2]* src2[4]+src1[6]* src2[5]+src1[10]* src2[6]+src1[14]* src2[7];
	dest[7]=src1[3]* src2[4]+src1[7]* src2[5]+src1[11]* src2[6]+src1[15]* src2[7];

	dest[8]=src1[0]* src2[8]+src1[4]* src2[9]+src1[8]* src2[10]+src1[12]* src2[11];
	dest[9]=src1[1]* src2[8]+src1[5]* src2[9]+src1[9]* src2[10]+src1[13]* src2[11];
	dest[10]=src1[2]* src2[8]+src1[6]* src2[9]+src1[10]* src2[10]+src1[14]* src2[11];
	dest[11]=src1[3]* src2[8]+src1[7]* src2[9]+src1[11]* src2[10]+src1[15]* src2[11];

	dest[12]=src1[0]* src2[12]+src1[4]* src2[13]+src1[8]* src2[14]+src1[12]* src2[15];
	dest[13]=src1[1]* src2[12]+src1[5]* src2[13]+src1[9]* src2[14]+src1[13]* src2[15];
	dest[14]=src1[2]* src2[12]+src1[6]* src2[13]+src1[10]* src2[14]+src1[14]* src2[15];
	dest[15]=src1[3]* src2[12]+src1[7]* src2[13]+src1[11]* src2[14]+src1[15]* src2[15];
}

inline void vqm_math::M4_M4V4_Multiply(V4& dest, M4& const src1, V4& const src2)
{
	//Cheat Sheet
	//  0 4 8  12   0    0*0 + 4*1 + 8*2 + 12*3  
	//	1 5 9  13 * 1  = 1*0 + 5*1 + 9*2 + 13*3
	//	2 6 10 14   2    2*0 + 6*1 + 10*2+ 14*3
	//	3 7 11 15   3    3*0 + 7*1 + 11*2+ 15*3
	dest[0]=src1[0]* src2[0] +src1[4]* src2[1]+src1[8]* src2[2]+src1[12]* src2[3];
	dest[1]=src1[1]* src2[0] +src1[5]* src2[1]+src1[9]* src2[2]+src1[13]* src2[3];
	dest[2]=src1[2]* src2[0] +src1[6]* src2[1]+src1[10]* src2[2]+src1[14]* src2[3];
	dest[3]=src1[3]* src2[0] +src1[7]* src2[1]+src1[11]* src2[2]+src1[15]* src2[3];
	
}

inline void vqm_math::M4_M4_Invert(M4& dest, M4& const src)
{
	//Cheat Sheet
	//  0 4 8  12
	//	1 5 9  13
	//	2 6 10 14
	//	3 7 11 15
	/*короче, так. предположим матрица

а1  а2  а3  а4 0 4
в1  в2  в3  в4 1 5
с1  с2  с3  с4 2
д1  д2  д3  д4 3


тогда определитель равен*/

 //Determinant определитель
float D = 
src[0]*(src[5]*(src[10]* src[15]-src[11]* src[14])- src[9]*(src[6]* src[15]-src[14]* src[7])+src[13]*(src[6]* src[11]-src[10]* src[7]))-
src[4]*(src[1]*(src[10]* src[15]-src[14]* src[11])- src[9]*(src[2]* src[15]-src[14]* src[3])+src[13]*(src[2]* src[7]-src[6]* src[3]))+
+src[8]*(src[1]*(src[6]* src[15]-src[14]* src[7])-src[5]*(src[2]* src[15]-src[14]* src[3])+src[13]*(src[2]* src[7]-src[6]* src[3]))-
-src[12]*(src[1]*(src[6]* src[11]-src[10]* src[7])-src[5]*(src[2]* src[11]-src[10]* src[3])+src[9]*(src[2]* src[7]-src[6]* src[3]));

//Cheat sheet
/*
5 9 13    1 9 13    1 5 13  1 5 9
6 10 14   2 10 14   2 6 14  2 6 10
7 11 15   3 11 15   3 7 15  3 7 11

4 8 12    0 8 12    0 4 12  0 4 8   
6 10 14   2 10 14   2 6 14  2 6 10
7 11 15   3 11 15   3 7 15  3 7 11

4 8 12    0 8 12    0 4 12  0 4 8
5 9 13    1 9 13    1 5 13  1 5 9
7 11 15   3 11 15   3 7 15  3 7 11

4 8 12    0 8 12    0 4 12  0 4 8
5 9 13    1 9 13    1 5 13  1 5 9
6 10 14   2 10 14   2 6 14  2 6 10


+ - + -
- + - +
+ - + -
- + - +
*/

dest[0]=(src[15] * src[10] * src[15]   +   src[9] * src[14] * src[7]    +   src[13] * src[6] * src[11]   -   src[13] * src[10] * src[7]   -   src[9] * src[6] * src[15]   -   src[5] * src[14] * src[11])/D;
dest[1]=-(src[4] * src[10] * src[15]   +   src[8] * src[14] * src[7]    +   src[12] * src[6] * src[11]   -   src[12] * src[10] * src[7]   -   src[8] * src[6] * src[15]   -   src[4] * src[14] * src[11])/D;
dest[2]=(src[4] * src[9] * src[15]   +   src[8] * src[13] * src[7]    +   src[12] * src[5] * src[11]   -   src[12] * src[9] * src[7]   -   src[8] * src[5] * src[15]   -   src[4] * src[13] * src[11])/D;
dest[3]=-(src[4] * src[9] * src[14]   +   src[8] * src[13] * src[6]    +   src[12] * src[5] * src[10]   -   src[12] * src[9] * src[6]   -   src[8] * src[5] * src[14]   -   src[4] * src[13] * src[10])/D;
dest[4]=-(src[1] * src[10] * src[15]   +   src[9] * src[14] * src[3]    +   src[13] * src[2] * src[11]   -   src[13] * src[10] * src[3]   -   src[9] * src[2] * src[15]   -   src[1] * src[14] * src[11])/D;
dest[5]=(src[0] * src[10] * src[15]   +   src[8] * src[14] * src[3]    +   src[12] * src[2] * src[11]   -   src[12] * src[10] * src[3]   -   src[8] * src[2] * src[15]   -   src[0] * src[14] * src[11])/D;
dest[6]=-(src[0] * src[9] * src[15]   +   src[8] * src[13] * src[3]    +   src[12] * src[1] * src[11]   -   src[12] * src[9] * src[3]   -   src[8] * src[1] * src[15]   -   src[0] * src[13] * src[11])/D;
dest[7]=(src[0] * src[9] * src[14]   +   src[8] * src[13] * src[2]   +   src[12] * src[1] * src[10]  -   src[12] * src[9] * src[2]   -   src[8] * src[1] * src[14]   -   src[0] * src[13] * src[10])/D;
//dest[8]=(1 * 6 * 15   +   5 * 14 * 3    +   13 * 2 * 7   -   13 * 6 * 3   -   5 * 2 * 15   -   1 * 14 * 7)/D;
//dest[9]=-(0 * 6 * 15   +   4 * 14 * 3    +   12 * 2 * 7   -   12 * 6 * 3   -   4 * 2 * 15   -   0 * 14 * 7)/D;
//dest[10]=(0 * 5 * 15   +   4 * 13 * 3    +   12 * 1 * 7   -   12 * 5 * 3   -   4 * 1 * 15   -   0 * 13 * 7)/D;
//dest[11]=-(0 * 5 * 14   +   4 * 13 * 2    +   12 * 1 * 6   -   12 * 5 * 2   -   4 * 1 * 14   -   0 * 13 * 6)/D;
//dest[12]=-(1 * 6 * 11   +   5 * 10 * 3    +   9 * 2 * 7   -   9 * 6 * 3   -   5 * 2 * 11   -   1 * 10 * 7)/D;
//dest[13]=(0 * 6 * 11   +   4 * 10 * 3    +   8 * 2 * 7   -   8 * 6 * 3   -   4 * 2 * 11   -   0 * 10 * 7)/D;
//dest[14]=-(0 * 5 * 11   +   4 * 9 * 3    +   8 * 1 * 7   -   8 * 5 * 3   -   4 * 1 * 11   -   0 * 9 * 7)/D;
//dest[15]=(0 * 5 * 10   +   4 * 9 * 2    +   8 * 1 * 6   -   8 * 5 * 2   -   4 * 1 * 10   -   0 * 9 * 6)/D;
dest[8]= (src[1]* src[6] * src[15] +  src[5]* src[14] * src[3] +   src[13]* src[2] * src[7]   +src[13]* src[6] * src[3]   +src[5]* src[2] * src[15] +src[1]* src[14] * src[7])/D;
dest[9]= (src[0]* src[6] * src[15] +  src[4]* src[14] * src[3] +   src[12]* src[2] * src[7]   +src[12]* src[6] * src[3]   +src[4]* src[2] * src[15] +src[0]* src[14] * src[7])/D;
dest[10]= (src[0]* src[5] * src[15] +  src[4]* src[13] * src[3] +   src[12]* src[1] * src[7]   +src[12]* src[5] * src[3]   +src[4]* src[1] * src[15] +src[0]* src[13] * src[7])/D;
dest[11]= (src[0]* src[5] * src[14] +  src[4]* src[13] * src[2] +   src[12]* src[1] * src[6]   +src[12]* src[5] * src[2]  +src[4]* src[1] * src[14] +src[0]* src[13] * src[6])/D;
dest[12]= (src[1]* src[6] * src[11] +  src[5]* src[10] * src[3] +   src[9]* src[2] * src[7]   +src[9]* src[6] * src[3]   +src[5]* src[2] * src[11] +src[1]* src[10] * src[7])/D;
dest[13]= (src[0]* src[6] * src[11] +  src[4]* src[10] * src[3] +   src[8]* src[2] * src[7]   +src[8]* src[6] * src[3]   +src[4]* src[2] * src[11] +src[0]* src[10] * src[7])/D;
dest[14]= (src[0]* src[5] * src[11] +  src[4]* src[9] * src[3] +   src[8]* src[1] * src[7]   +src[8]* src[5] * src[3]   +src[4]* src[1] * src[11] +src[0]* src[9] * src[7])/D;
dest[15]= (src[0]* src[5] * src[10] +  src[4]* src[9] * src[2] +   src[8]* src[1] * src[6]   +src[8]* src[5] * src[2]   +src[4]* src[1] * src[10] +src[0]* src[9] * src[6])/D;

}

inline void vqm_math::M4_M4_Transpose(M4& dest, M4& const src)
{
	//Cheat Sheet
	//  0 4 8  12   0  1  2  3 
	//	1 5 9  13 = 4  5  6  7
	//	2 6 10 14   8  9  10 11
	//	3 7 11 15   12 13 14 15

	dest[0]=src[0];
	dest[1]=src[4];
	dest[2]=src[8];
	dest[3]=src[12];
	dest[4]=src[1];
	dest[5]=src[5];
	dest[6]=src[9];
	dest[7]=src[13];
	dest[8]=src[2];
	dest[9]=src[6];
	dest[10]=src[10];
	dest[11]=src[14];
	dest[12]=src[3];
	dest[13]=src[7];
	dest[14]=src[11];
	dest[15]=src[15];
}
inline void vqm_math::M4_M4_Copy(M4& dest, M4& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=src[3];
	dest[4]=src[4];
	dest[5]=src[5];
	dest[6]=src[6];
	dest[7]=src[7];
	dest[8]=src[8];
	dest[9]=src[9];
	dest[10]=src[10];
	dest[11]=src[11];
	dest[12]=src[12];
	dest[13]=src[13];
	dest[14]=src[14];
	dest[15]=src[15];
}



//src1[0] - eye point X; src1[1] - eye point Y;   src1[2]- eye point Z
//src2[0] - center of view X;  src2[1] -center of view Y; src2[2] - center of view Z
//src3[0] - up vector x; src3[1] - up vector y;  srcp3[2] - up vector z
//http://gamedev.stackexchange.com/questions/8839/view-matrix-in-opengl
inline void vqm_math::M4_V3V3V3_SetLookAt(M4& dest, V3& const src1,V3& const src2, V3& const src3)
{

	V3 Xaxis;
	V3 Yaxis;
	V3 Zaxis;
	V3 buf;
	
	V3_V3V3_Minus(buf,src1,src2);
	V3_V3_Normalize(Zaxis,buf);
	V3_V3V3_CrossProduct(buf,src3,Zaxis);
	V3_V3_Normalize(Xaxis,buf);
	V3_V3V3_CrossProduct(Yaxis,Zaxis,Xaxis);
		
	dest[0]=Xaxis[0];
	dest[1]=Xaxis[1];
	dest[2]=Xaxis[2];
	dest[3]=-S_V3V3_DotProduct(Xaxis,src1);
	dest[4]=Yaxis[0];
	dest[5]=Yaxis[1];
	dest[6]=Yaxis[2];
	dest[7]=-S_V3V3_DotProduct(Yaxis,src1);
	dest[8]=Zaxis[0];
	dest[9]=Zaxis[1];
	dest[10]=Zaxis[2];
	dest[11]=-S_V3V3_DotProduct(Zaxis,src1);;
	dest[12]=0;
	dest[13]=0;
	dest[14]=0;
	dest[15]=1;

	
}

//Frustum M усеченная камера
//src1[0] = left src1[1] = right  src1[2] = bottom  src2[0] = top src2[1] = near  src2[2] = far
inline void vqm_math::M4_V3V3_Frustrum(M4& dest,V3& const src1, V3& const src2)
	{
	dest[0]=2*src2[1]/(src1[1]-src1[0]);
	//dest[1]=0;
	//dest[2]=0;
	//dest[3]=0;
	//dest[4]=0;
	dest[5]=2* src2[1]/(src2[0]-src1[2]);
	//dest[6]=0;
	//dest[7]=0;
	dest[8]=(src1[1]+src1[0])/(src1[1]-src1[0]);
	dest[9]=(src2[0]+src1[2])/(src2[0]-src1[2]);
	dest[10]=-(src2[2]+src2[1])/(src2[2]-src2[1]);
	dest[11]=-1;
	//dest[12]=0;
	//dest[13]=0;
	dest[14]=-2* src2[2]* src2[1]/(src2[2]-src2[1]);
	//dest[15]=1;
}

//Ортогональная проекция
//src1[0] = left src1[1] = right  src1[2] = bottom  src2[0] = top src2[1] = near  src2[2] = far
inline void vqm_math::M4_V3V3_Ortho(M4& dest,V3& const src1, V3& const src2)
	{
	dest[0]=2/(src1[1]-src1[0]);
//	dest[1]=0;
//	dest[2]=0;
//	dest[3]=0;
//	dest[4]=0;
	dest[5]=2/(src2[0]-src1[2]);
//	dest[6]=0;
//	dest[7]=0;
//	dest[8]=0;
//	dest[9]=0;
	dest[10]=-2/(src2[2]-src2[1]);
//	dest[11]=0;
	dest[12]=-(src1[1]+src1[0])/(src1[1]-src1[0]);
	dest[13]=-(src2[0]+src1[2])/(src2[0]-src1[2]);
	dest[14]=-(src2[2]+src2[1])/(src2[2]-src2[1]);
	dest[15]=1;
}
//Единичный вектор
inline void vqm_math::V3_V3_SetIdentity(V3& dest)
{
	dest[0]=1;
	dest[1]=1;
	dest[2]=1;
}
inline void vqm_math::V4_V4_SetIdentity(V4& dest)
{
	dest[0]=1;
	dest[1]=1;
	dest[2]=1;
	dest[3]=1;
}
//Векторное произведение
inline void vqm_math::V3_V3V3_CrossProduct(V3& dest, V3& const src1, V3& const src2)
{
	
	dest[0]=src1[1]* src2[2]-src1[2]* src2[1];
	dest[1]=src1[2]* src2[0]-src1[0]* src2[2];
	dest[2]=src1[0]* src2[1]-src1[1]* src2[0];
}
//Скалярное произведение
float vqm_math::S_V3V3_DotProduct(V3& const src1, V3& const src2)
{
	return src1[0]* src2[0] + src1[1]* src2[1]  + src1[2]* src2[2]; 
}
//Смешанное произведение
float vqm_math::S_V3V3V3_TrippleProduct(V3& dest, V3& const src1, V3& const src2, V3& const src3)
{
		V3 Cross;
		V3_V3V3_CrossProduct(Cross,src2,src3);
		return S_V3V3_DotProduct(src1,Cross);

}
//Длина вектора
float vqm_math::S_V3_EuclideanNorm(V3& const src)
{
	return sqrt(src[0]*src[0]+src[1]*src[1]+src[2]*src[2]);
}
//Угол между векторами
float vqm_math::A_V3V3_Angle(V3& const src1, V3& const src2)
{
	//3 вызова нужно переделать в китайском стиле
	return acos(S_V3V3_DotProduct(src1,src2)/ (S_V3_EuclideanNorm(src1)*S_V3_EuclideanNorm(src2)));
}
//Нормализация вектора
inline void vqm_math::V3_V3_Normalize(V3& dest, V3& const src)
{
	float norm = src[0]*src[0]+src[1]*src[1]+src[2]*src[2];
	dest[0]=src[0]/norm;
	dest[1]=src[1]/norm;
	dest[2]=src[2]/norm;
}
//Нормализация вектора
inline void vqm_math::V4_V4_Normalize(V4& dest, V4& const src)
{
float norm = sqrt(src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+src[3]*src[3]);
	dest[0]=src[0]/norm;
	dest[1]=src[1]/norm;
	dest[2]=src[2]/norm;
	dest[3]=src[3]/norm;
}
//Сложение векторов
inline void vqm_math::V3_V3V3_Plus(V3& dest, V3& const src1, V3& const src2)
{
	dest[0]=src1[0]+src2[0];
	dest[1]=src1[1]+src2[1];
	dest[2]=src1[2]+src2[2];
}
//Вычитание векторов
inline void vqm_math::V3_V3V3_Minus(V3& dest, V3& const src1, V3& const src2)
{
	dest[0]=src1[0]-src2[0];
	dest[1]=src1[1]-src2[1];
	dest[2]=src1[2]-src2[2];
}
//Сложение векторов
inline void vqm_math::V4_V4V4_Plus(V4& dest, V4& const src1, V4& const src2)
{
	dest[0]=src1[0]+src2[0];
	dest[1]=src1[1]+src2[1];
	dest[2]=src1[2]+src2[2];
}
//Вычитание векторов
inline void vqm_math::V4_V4V4_Minus(V4& dest, V4& const src1, V4& const src2)
{
	dest[0]=src1[0]-src2[0];
	dest[1]=src1[1]-src2[1];
	dest[2]=src1[2]-src2[2];
}
//Копирование вектора
inline void vqm_math::V3_V3_Copy(V3& dest, V3& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
}
//Копирование вектора
inline void vqm_math::V4_V4_Copy(V4& dest, V4& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=src[3];
}
//Умножение вектора на скаляр
inline void vqm_math::V3_SV3_Product(V3& dest, V3& const src1, float src2)
{
	dest[0]=src1[0]*src2;
	dest[1]=src1[1]*src2;
	dest[2]=src1[2]*src2;

}
//умножение вектора на скаляр
inline void vqm_math::V4_SV4_Product(V4& dest, V4& const src1, float src2)
{
	dest[0]=src1[0]*src2;
	dest[1]=src1[1]*src2;
	dest[2]=src1[2]*src2;
	dest[3]=src1[3]*src2;
}
//Копирование кватернионов
inline void vqm_math::Q_Q_Copy(Q& dest, Q& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=src[3];
}
//Вычитание двух кватернионов
inline void vqm_math::Q_QQ_Minus(Q& dest, Q& const src1, Q& const src2)
{
	 //Q - quaternion (w,x,y,z) 
	dest[0]=src1[0]-src2[0];
	dest[1]=src1[1]-src2[1];
	dest[2]=src1[2]-src2[2];
	dest[3]=src1[3]-src2[3];
}
 //Cложение двух кватернионов
inline void vqm_math::Q_QQ_Plus(Q& dest, Q& const src1, Q& const src2)
{
	 //Q - quaternion (w,x,y,z) 
	dest[0]=src1[0]+src2[0];
	dest[1]=src1[1]+src2[1];
	dest[2]=src1[2]+src2[2];
	dest[3]=src1[3]+src2[3];
}
//Умножение двух кватернионов	
inline void vqm_math::Q_QQ_Product(Q& dest, Q& const src1, Q& const src2)
{
	dest[0]= src1[0]*src2[0]- (src1[1]* src2[1] + src1[2]* src2[2]  + src1[3]* src2[3]);
	dest[1]= src1[2]* src2[3]-src1[3]* src2[2] + src1[0]*src2[1] + src2[0]*src1[1];
	dest[2]= src1[3]* src2[1]-src1[1]* src2[3] + src1[0]*src2[2] + src2[0]*src1[2];
	dest[3]= src1[1]* src2[2]-src1[2]* src2[1] + src1[0]*src2[3] + src2[0]*src1[3];

}
//Умножение скаляра на кватернион
inline void vqm_math::Q_SQ_Multiply(Q& dest, float src1, Q& const src2)
{
	dest[0]=src2[0]*src1;
	dest[1]=src2[1]*src1;
	dest[2]=src2[2]*src1;
	dest[3]=src2[3]*src1;
}
//Норма кватерниона
float vqm_math::S_Q_Norm(Q& const src)
{
	return src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+ src[3]*src[3];
}
//Корень из нормы 
float vqm_math::S_Q_Magnitude(Q& const src)
{
	return sqrt(src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+ src[3]*src[3]);
}
//Сопряжение двух кватернионов
inline void vqm_math::Q_Q_Conjugate(Q& dest, Q& const src)
{
dest[0]=src[0];
dest[1]=-src[1];
dest[2]=-src[2];
dest[3]=-src[3];
}
//Нормализация
inline void vqm_math::Q_Q_Normalize(Q& dest, Q& const src)
{
	float norm = src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+ src[3]*src[3];
	dest[0]=src[0]/norm;
	dest[1]=src[1]/norm;
	dest[2]=src[2]/norm;
	dest[3]=src[3]/norm;
}
//Обратный кватернион
inline void vqm_math::Q_Q_Inverse(Q& dest, Q& const src)
{
	float norm = src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+ src[3]*src[3];
	dest[0]=src[0]/norm;
	dest[1]=-src[1]/norm;
	dest[2]=-src[2]/norm;
	dest[3]=-src[3]/norm;
}

//Скалярное произведение
float vqm_math::S_QQ_InnerProduct(Q& const src1, Q& const src2)
{
	return  src1[0]* src2[0] + src1[1]* src2[1]  + src1[2]* src2[2]+ src1[3]*src2[3];
}

//Вращение вектора кватернионом
inline void vqm_math::V3_QV3_Rotate(V3& dest, Q& const src1, V3& const src2)
{
	Q InvertedQ;
	Q destVector;
	Q buf;
	destVector[0] = 0;
	destVector[1] = src2[0];
	destVector[2] = src2[1];
	destVector[3] = src2[2];
	Q_Q_Inverse(InvertedQ,src1);
	Q_QQ_Product(buf,src1,destVector);
	Q_QQ_Product(destVector,buf,InvertedQ);
	dest[0] = destVector[1];
	dest[1] = destVector[2];
	dest[2] = destVector[3];
	
}

//Нулевой кватернион
inline void vqm_math::Q_Q_SetIdentity(Q& dest)
{
	dest[0]=1;
	dest[1]=0;
	dest[2]=0;
	dest[3]=0;
}

//Ось-угол в кватернион src[0] = angle src[1] = x src[2] = y src[3] = z предполагается угол [0;pi/2]
inline void vqm_math::Q_V4_V4AToQ(Q& dest, V4& const src)
{
	float sinA=sin(src[0]/2);
	dest[0]=sqrt(1 - sinA*sinA);
    dest[1]=src[1]*sinA;
	dest[2]=src[2]*sinA;
	dest[3]=src[3]*sinA;
}

//Кватернион в ось-угол src[0] = angle src[1] = x src[2] = y src[3] = z
//Подразумевается что кватернион нормализован
inline void vqm_math::V4_Q_QToV4A(V4& dest, Q& const src)
{

float s= sqrt(1-src[0]*src[0]);
dest[0] = 2*acos(src[0]);
dest[1] = src[1] / s;
dest[2] = src[2] / s;
dest[3] = src[3] / s;
//Сюда можно добавить проверку деления на ноль
}

//Cферические координаты в кватренион src[0] = latitude src[1] = longitude src[2] = angle
inline void vqm_math::Q_V3_SphericalToQ(Q& dest, V3& const src)
{
  float sin_a = sin( src[2] / 2 );
  float cos_a = cos( src[2] / 2 );

  float sin_lat = sin( src[0] );
  float cos_lat = cos( src[0] );

  float sin_long = sin( src[1] );
  float cos_long = cos( src[1] );

  dest[1] = sin_a * cos_lat * sin_long;
  dest[2] = sin_a * sin_lat;
  dest[3] = sin_a * sin_lat * cos_long;
  dest[0] = cos_a;
}
//Крен тангаж рыскание в кватернион.
//http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
//src[0] =roll src[1] = pitch src[2]= yaw  (крен,  тангаж,рыскание  по нашему)
inline void vqm_math::Q_V3_RPYToQ(Q& dest, V3& const src)
{
	float cosR=cos(src[0]/2);
	float sinR=sin(src[0]/2);
	float cosP=cos(src[1]/2);
	float sinP=sin(src[1]/2);
	float cosY=cos(src[2]/2);
	float sinY=sin(src[2]/2);
	dest[0]=cosR*cosP*cosY+sinR*sinP*sinY;
	dest[1]=sinR*cosP*cosY-cosR*sinP*sinY;
	dest[2]=cosR*sinP*cosY+sinR*cosP*sinY;
	dest[3]=cosR*cosP*sinY-sinR*sinP*cosY;
}
//Кватернион в Крен Тангаж Рыскание
inline void vqm_math::V3_Q_QToRPY(V3& dest, Q& const src)
{
	dest[0]=atan(2*(src[0]* src[1] + src[2]* src[3]))/(1-2*(src[1]*src[1]+src[2]*src[2]));
	dest[1]=asin(2*(src[0]*src[2]-src[3]*src[1]));
	dest[2]=atan(2*(src[0]* src[3] + src[1]* src[2]))/(1-2*(src[2]*src[2]+src[3]*src[3]));
}
//Кватернион в ортогональную матрицу поворота
//http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
inline void vqm_math::M4_Q_ToM4_Orthogonal(M4& dest, Q& const src)
{
	//  0 4 8  12
	//	1 5 9  13
	//	2 6 10 14
	//	3 7 11 15
	
	dest[0]= src[0]*src[0]+src[1]*src[1]-src[2]*src[2]-src[3]*src[3];
	dest[1]= 2*src[1]*src[2]+2*src[0]*src[3];
	dest[2]= 2*src[1]*src[3]-2*src[0]*src[2];
	dest[3]= 0;
	dest[4]= 2*src[1]*src[2]-2*src[0]*src[3];
	dest[5]= src[0]*src[0]-src[1]*src[1]+src[2]*src[2]-src[3]*src[3];
	dest[6]= 2*src[2]*src[3] + 2*src[0]*src[1];
	dest[7]= 0;
	dest[8]= 2*src[1]*src[3] + 2*src[0]*src[2];
	dest[9]= 2*src[2]*src[3] - 2*src[0]*src[1];
	dest[10]=src[0]*src[0] -src[1]*src[1] - src[2]*src[2] + src[3]*src[3];
	dest[11]=0;
	dest[12]=0; 
	dest[13]=0; 
	dest[14]=0; 
	dest[15]=1; 

}

//Кватернион в обычную матрицу поворота
inline void vqm_math::M4_Q_ToM4(M4& dest, Q& const src)
{
    //  0 4 8  12
	//	1 5 9  13
	//	2 6 10 14
	//	3 7 11 15
	dest[0]= 1-2*src[2]*src[2]-2*src[3]*src[3];
	dest[1]= 2*src[1]*src[2]+2*src[3]*src[0];
	dest[2]= 2*src[1]*src[3]-2*src[2]*src[0];
	dest[3]= 0;
	dest[4]= 2*src[1]*src[2]-2*src[3]*src[0];
	dest[5]= 1-2*src[1]*src[1]-2*src[3]*src[3];
	dest[6]= 2*src[2]*src[3]+2*src[1]*src[0];
	dest[7]= 0;
	dest[8]= 2*src[1]*src[3]+2*src[2]*src[0];
	dest[9]= 2*src[2]*src[3]-2*src[1]*src[0];
	dest[10]=1-2*src[1]*src[1]-2*src[2]*src[2];
	dest[11]=0;
	dest[12]=0; 
	dest[13]=0; 
	dest[14]=0; 
	dest[15]=1; 
}

//Матрицу поворота в кватернион
inline void vqm_math::Q_M4_ToQ(Q& dest, M4& const src)
{
//Защищено от деления на ноль в предположении что нулевой элемент матрицы больше 5 и 10 элементов
//Только один квадратный корень
//Cheat sheet
// xx xy xz    0 4 8
// yx yy yz    1 5 9
// zx zy zz    2 6 10
float t = src[0]+src[5]+src[10];
float r = sqrt(1+src[0]-src[5]-src[10]);
float s = 0.5/r;
dest[0] = (src[6]-src[9])*s;
dest[1] = 0.5*r;
dest[2] = (src[4]+src[1])*s;
dest[3] = (src[2]+src[9])*s;
}

//Гладкая интерполяция вращения кватернионом
//http://wat.gamedev.ru/articles/quaternions?page=2
void vqm_math::Slerp(Q& dest,
           Q& const src1, Q& const src2, float t)
{
  float p1[4];
  double omega, cosom, sinom, scale0, scale1;

  // косинус угла
  cosom = src1[1]*src2[1] + src1[2]* src2[2] + src1[3]* src2[3] + src1[0]*src2[0];

  if ( cosom <0.0 )
  { 
    cosom = -cosom;
    p1[0] = -src1[1];  p1[1] = - src1[2];
    p1[2] = - src1[3];  p1[3] = - src1[0];
  }
  else
  {
    p1[0] =src1[1];    p1[1] = src1[2];
    p1[2] = src1[3];    p1[3] = src1[0];
  }

  if ( (1.0 - cosom) > DELTA )
  {
    // стандартный случай (slerp)
    omega = acos(cosom);
    sinom = sin(omega);
    scale0 = sin((1.0 - t) * omega) / sinom;
    scale1 = sin(t * omega) / sinom;
  }
  else
  {        
    // если маленький угол - линейная интерполяция
    scale0 = 1.0 - t;
    scale1 = t;
  }
  
  dest[1] = scale0 * src2[1] + scale1 * p1[0];
  dest[2] = scale0 * src2[2] + scale1 * p1[1];
  dest[3] = scale0 * src2[3] + scale1 * p1[2];
  dest[0] = scale0 * src2[0] + scale1 * p1[3];
}

//След матрицы
inline float vqm_math::S_M4_Trace(M4 &const src)
{
	return src[0]+src[5]+src[10]+src[15];
}
//Норма Фронебиуса
inline float vqm_math::S_M4_FrobeniusNorm(M4 &const src)
{
		return sqrt(src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+src[3]*src[3]+src[4]*src[4]+src[5]*src[5]+src[6]*src[6]+src[7]*src[7]+src[8]*src[8]+src[9]*src[9]+src[10]*src[10]+src[11]*src[11]+src[12]*src[12]+src[13]*src[13]+src[14]*src[14]+src[15]*src[15]);
}
//Единичная матрица
inline void vqm_math::M3_M3_SetIdentity(M3& dest)
{
	dest[0]=1;
	dest[1]=0;
	dest[2]=0;
	dest[3]=0;
	dest[4]=1;
	dest[5]=0;
	dest[6]=0;
	dest[7]=0;
	dest[8]=1;
}
//Матрица масштабирования
inline void vqm_math::M3_V4_SetScaleM(M3& dest ,V3& const src)
{
	dest[0]=src[0];
	dest[4]=src[1];
	dest[8]=src[2];
}
//Матрица поворота через ось и угол (На входе единичная матрица)
inline void vqm_math::M3_V4_SetRotate(M3& dest, V4& const src)
{
	float cosA = cos(src[0]);
	float sinA = sin(src[0]);
	float x = src[1];
	float y= src[2];
	float z= src[3];
	dest[0]=cosA+ (1-cosA)*x*x;
	dest[1]=(1-cosA)*y*x+sinA*z;
	dest[2]=(1-cosA)*z*x-sinA*y;
	
	dest[3]=(1-cosA)*x*y-sinA*z;
	dest[4]=cosA+ (1-cosA)*y*y;
	dest[5]=(1-cosA)*z*y+sinA*x;

	dest[6]=(1-cosA)*x*z+sinA*y;
	dest[7]=(1-cosA)*y*z-sinA*x;
	dest[8]=cosA+ (1-cosA)*z*z;
	
}
//Матрица поворота через углы Эйлера (На входе единичная матрица)
inline void vqm_math::M3_V4_SetRotateEuler(M3& dest, V4& const src)
{
	float CosAlfa = cos(src[0]);
	float CosBeta= cos(src[1]);
	float CosGamma= cos(src[2]);
	float SinAlfa =sin(src[0]);
	float SinBeta = sin(src[1]);
	float SinGamma = sin(src[2]);
	dest[0]=CosAlfa*CosGamma-SinAlfa*CosBeta*SinGamma;
	dest[1]=SinAlfa*CosGamma+CosAlfa*CosBeta*SinGamma;
	dest[2]=SinBeta*SinGamma;
	//dest[3]=0;
	dest[3]=-CosAlfa*SinGamma-SinAlfa*CosBeta*CosGamma;
	dest[4]=-SinAlfa*SinGamma+CosAlfa*CosBeta*CosGamma;
	dest[5]=SinBeta*CosGamma;
	//dest[7]=0;
	dest[6]=SinAlfa*SinBeta;
	dest[7]=-CosAlfa*SinBeta;
	dest[8]=CosBeta;
}
//Перемножение матриц (dest!= src1|| src2  src1 может быть равно  src2)
inline void vqm_math::M3_M3M3_Multiply(M3& dest,M3& const src1, M3& const src2)
{
	//Cheat Sheet
	//0 3 6   0 3 6   0*0 + 3*1 + 6*2 | 0*3 +3*4 +6*5 |0*6 3*7 6*8 
	//1 4 7   1 4 7 = 1*0 + 4*1 + 7*2 | 1*3 +4*4 +7*5 |1*6 4*7 7*8 
	//2 5 8   2 5 8   2*0 + 5*1 + 8*2 | 2*3 +5*4 +8*5 |2*6 5*7 8*8 

	dest[0]=src1[0]*src2[0]+src1[3]*src2[1]+src1[6]*src2[2];
	dest[1]=src1[1]*src2[0]+src1[4]*src2[1]+src1[7]*src2[2];
	dest[2]=src1[2]*src2[0]+src1[5]*src2[1]+src1[8]*src2[2];
	dest[3]=src1[0]*src2[3]+src1[3]*src2[4]+src1[6]*src2[5];
	dest[4]=src1[1]*src2[3]+src1[4]*src2[4]+src1[7]*src2[5];
	dest[5]=src1[2]*src2[3]+src1[5]*src2[4]+src1[8]*src2[5];
	dest[6]=src1[0]*src2[6]+src1[3]*src2[7]+src1[6]*src2[8];
	dest[7]=src1[1]*src2[6]+src1[4]*src2[7]+src1[7]*src2[8];
	dest[8]=src1[2]*src2[6]+src1[5]*src2[7]+src1[8]*src2[8];
}
//Произведение матрицы на вектор
inline void vqm_math::M3_MV3_Multiply(V3& dest, M3& const src1, V3& const src2)
{
	//Cheat Sheet
	//  0 3 6  0  
	//  1 4 7  1 = 0*0+3*1+6*2|1*0+4*1+7*2|2*0+5*1+8*2
	//  2 5 8  2 

	dest[0]=src1[0]*src2[0]+src1[3]*src2[1]+src1[6]*src2[2];
	dest[1]=src1[1]*src2[0]+src1[4]*src2[1]+src1[7]*src2[2];
	dest[2]=src1[2]*src2[0]+src1[5]*src2[1]+src1[8]*src2[2];
}
//Обратная матрица
inline void vqm_math::M3_M3_Invert(M3& dest, M3& const src)
{
	float D = src[8] * src[4] * src[8]   +   src[3] * src[7] * src[2]    +   src[6] * src[1] * src[5]   -   src[6] * src[4] * src[2]   -   src[3] * src[1] * src[8]   -   src[0] * src[7] * src[5];

	//Cheat Sheet
	//  0 3 6   
	//  1 4 7  
	//  2 5 8  

	// 4 7  1 7  1 4 
    // 5 8  2 8  2 5

	// 3 6  0 6  0 3 
	// 5 8  2 8  2 5

	// 3 6  0 6  0 3
	// 4 7  1 7  1 4

	// + - + 
	// - + - 
    // + - +
	dest[0]=(src[4]*src[8]-src[5]*src[7])/D;
	dest[1]=-(src[3]*src[8]-src[5]*src[6])/D;
	dest[2]=(src[3]*src[7]-src[4]*src[6])/D;
	dest[3]=-(src[1]*src[8]-src[2]*src[7])/D;
	dest[4]=(src[0]*src[8]-src[2]*src[6])/D;
	dest[5]=-(src[0]*src[7]-src[1]*src[6])/D;
	dest[6]=(src[1]*src[5]-src[2]*src[4])/D;
	dest[7]=-(src[0]*src[5]-src[2]*src[3])/D;
	dest[8]=(src[0]*src[4]-src[1]*src[3])/D;
}
//Транспонирование матрицы
inline void vqm_math::M3_M3_Transpose(M3& dest, M3& const src)
{
 // Cheat sheet
 //   0 3 6   0 1 2    
 //   1 4 7   3 4 5  
 //   2 5 8   6 7 8
	dest[0] = src[0];
	dest[1] = src[3];
	dest[2] = src[6];
	dest[3] = src[1];
	dest[4] = src[4];
	dest[5] = src[7];
	dest[6] = src[2];
	dest[7] = src[5];
	dest[8] = src[8];
}
//След матрицы
inline float vqm_math::S_M3_Trace(M3& const src)
{
	return src[0]+src[4]+src[8];
}
//Норма Фронебиуса
inline float vqm_math::S_M3_FrobeniusNorm(M3& const src)
{
	return sqrt(src[0]*src[0]+src[1]*src[1]+src[2]*src[2]+src[3]*src[3]+src[4]*src[4]+src[5]*src[5]+src[6]*src[6]+src[7]*src[7]+src[8]*src[8]);
}
//Копирование матрицы
inline void vqm_math::M3_M3_Copy(M3& dest, M3& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=src[3];
	dest[4]=src[4];
	dest[5]=src[5];
	dest[6]=src[6];
	dest[7]=src[7];
	dest[8]=src[8];

}

inline void vqm_math::M3_M4_ToM3(M3& dest,M4& const src)
{
//Сheat sheet
 //   0 3 6       0 4 8  12
 //   1 4 7       1 5 9  13
 //   2 5 8       2 6 10 14
//                3 7 11 15

	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=src[4];
	dest[4]=src[5];
	dest[5]=src[6];
	dest[6]=src[8];
	dest[7]=src[9];
	dest[8]=src[10];
	

}  
inline void vqm_math::M4_M3_ToM4(M4& dest,M3& const src)
{
//Сheat sheet
 //   0 3 6       0 4 8  12
 //   1 4 7       1 5 9  13
 //   2 5 8       2 6 10 14
//                3 7 11 15

	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=0;
	dest[4]=src[3];
	dest[5]=src[4];
	dest[6]=src[5];
	dest[7]=0;
	dest[8]=src[6];
	dest[9]=src[7];
	dest[10]=src[8];
	dest[11]=0;
	dest[12]=0;
	dest[13]=0;
	dest[14]=0;
	dest[15]=src[1];
}

//Вектор 3 в вектор 4
inline void vqm_math::V4_V3_ToV4(V4& dest, V3& const src)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
	dest[3]=1;
}
//Вектор 4 в вектор 3
inline void vqm_math::V3_V4_ToV3(V3& dest, V4& const src)
{

	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];

}

//Определитель матрицы 4
inline float  vqm_math::S_M4_Determinant(M4 &const src)
{
return 
src[0]*(src[5]*(src[10]* src[15]-src[11]* src[14])- src[9]*(src[6]* src[15]-src[14]* src[7])+src[13]*(src[6]* src[11]-src[10]* src[7]))-
src[4]*(src[1]*(src[10]* src[15]-src[14]* src[11])- src[9]*(src[2]* src[15]-src[14]* src[3])+src[13]*(src[2]* src[7]-src[6]* src[3]))+
+src[8]*(src[1]*(src[6]* src[15]-src[14]* src[7])-src[5]*(src[2]* src[15]-src[14]* src[3])+src[13]*(src[2]* src[7]-src[6]* src[3]))-
-src[12]*(src[1]*(src[6]* src[11]-src[10]* src[7])-src[5]*(src[2]* src[11]-src[10]* src[3])+src[9]*(src[2]* src[7]-src[6]* src[3]));

}
inline float  vqm_math::S_M3_Determinant(M3 &const src)
{
//5 9 13    0 3 6 
//6 10 14   1 4 7
//7 11 15   2 5 8
return
	(src[8] * src[4] * src[8]   +   src[3] * src[7] * src[2]    +   src[6] * src[1] * src[5]   -   src[6] * src[4] * src[2]   -   src[3] * src[1] * src[8]   -   src[0] * src[7] * src[5]);
}

//Кватернион в ортогональную матрицу поворота
inline void vqm_math::M3_Q_ToM3_Orthogonal(M3& dest, Q& const src)
{
	dest[0]= src[0]*src[0]+src[1]*src[1]-src[2]*src[2]-src[3]*src[3];
	dest[1]= 2*src[1]*src[2]+2*src[0]*src[3];
	dest[2]= 2*src[1]*src[3]-2*src[0]*src[2];
	
	dest[3]= 2*src[1]*src[2]-2*src[0]*src[3];
	dest[4]= src[0]*src[0]-src[1]*src[1]+src[2]*src[2]-src[3]*src[3];
	dest[5]= 2*src[2]*src[3] + 2*src[0]*src[1];

	dest[6]= 2*src[1]*src[3] + 2*src[0]*src[2];
	dest[7]= 2*src[2]*src[3] - 2*src[0]*src[1];
	dest[8]=src[0]*src[0] -src[1]*src[1] - src[2]*src[2] + src[3]*src[3];

}
//Кватернион в обычную матрицу поворота
inline void vqm_math::M3_Q_ToM3(M3& dest, Q& const src)
{

	dest[0]= 1-2*src[2]*src[2]-2*src[3]*src[3];
	dest[1]= 2*src[1]*src[2]+2*src[3]*src[0];
	dest[2]= 2*src[1]*src[3]-2*src[2]*src[0];
	dest[3]= 2*src[1]*src[2]-2*src[3]*src[0];
	dest[4]= 1-2*src[1]*src[1]-2*src[3]*src[3];
	dest[5]= 2*src[2]*src[3]+2*src[1]*src[0];
	dest[6]= 2*src[1]*src[3]+2*src[2]*src[0];
	dest[7]= 2*src[2]*src[3]-2*src[1]*src[0];
	dest[8]=1-2*src[1]*src[1]-2*src[2]*src[2];

}
//Матрица поворота в кватернион
inline void vqm_math::Q_M3_ToQ(Q& dest, M3& const src)
{
//Cheat sheet
// 0 3 6    0 4 8
// 1 4 7    1 5 9
// 2 5 8    2 6 10
float t = src[0]+src[4]+src[8];
float r = sqrt(1+src[0]-src[4]-src[8]);
float s = 0.5/r;
dest[0] = (src[5]-src[7])*s;
dest[1] = 0.5*r;
dest[2] = (src[3]+src[1])*s;
dest[3] = (src[2]+src[7])*s;
}

