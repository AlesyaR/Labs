//The Eternal Blood River Engine source file
//Started 29.08.2012 by Fedor Igumnov. Moscow Russia.
//mailto:igumnovfedor@gmail.com
//Propietary code. Contact me if you find this useful.

/*Awaken my child, and embrace the glory that is your birthright
Know that I am the Overmind. The ETERNAL will of the swarm. 
And that you have been created to serve me. 
										Overmind(�������)
														 */


//������� ��������
//�������, �����������, �������, ����������
//V4 - OpenGl Vecotr 4x rotation angle in radians,x,y,z 
//  0
//  1
//  2
//  3

//V3 - OpenGl Vector 3x x,y,z
// 0
// 1
// 2
// 3

//S - Real Scalar 
//A - angle in radians
namespace vqm_math{
typedef float Q[4];
typedef float M4[16];
typedef float M3[9];
typedef float V4[4];
typedef float V3[3]; 
//typedef float S;
//typedef float A;

//Quaternion headers
//������������� ����������
//��������� �������
inline void Q_Q_SetIdentity(Q& dest);
//���-���� � ����������
inline void Q_V4_V4AToQ(Q& dest, V4 & const src);
//���������� � ���-���� 
inline void V4_Q_QToV4A(V4& dest, Q& const src);
//C���������� ���������� � ���������� 
inline void Q_V3_SphericalToQ(Q& dest, V3& const src);
//���� ������ �������� � ����������
inline void Q_V3_RPYToQ(Q& dest, V3& const src);
//���������� � ���� ������ ��������
inline void V3_Q_QToRPY(V3& dest, Q& const src);
//��������� ���� ������������  
inline void Q_QQ_Minus(Q& dest, Q& const src1, Q& const src2);
//C������� ���� ������������
inline void Q_QQ_Plus(Q& dest, Q& const src1, Q& const src2);
//��������� ����������� �� ������
inline void Q_SQ_Multiply(Q& dest, float src1, Q& const src2);
//��������� ���� ������������	
inline void Q_QQ_Product(Q& dest, Q& const src1, Q& const src2);
//�����(����� ��������� ���������)
inline float S_Q_Norm(Q& const src);
//������(������ �� �����)
inline float S_Q_Magnitude(Q& const src);
//����������
inline void Q_Q_Conjugate(Q& dest, Q& const src);
//�������� ����������
inline void Q_Q_Inverse(Q& dest, Q& const src);
//������������� ����������
inline void Q_Q_Normalize(Q& dest, Q& const src);
//��������� ������������ ���� ������������
inline float S_QQ_InnerProduct(Q& const src1, Q& const src2);
//�������� ������� ������������
inline void V3_QV3_Rotate(V3& dest, Q& const src1, V3& const src2);
//���������� � ������������� ������� ��������
inline void M4_Q_ToM4_Orthogonal(M4& dest, Q& const src);
//���������� � ������� ������� ��������
inline void M4_Q_ToM4(M4& dest, Q& const src);
//������� �������� � ����������
inline void Q_M4_ToQ(Q& dest, M4& const src);
//���������� � ������������� ������� ��������
inline void M3_Q_ToM3_Orthogonal(M3& dest, Q& const src);
//���������� � ������� ������� ��������
inline void M3_Q_ToM3(M3& dest, Q& const src);
//������� �������� � ����������
inline void Q_M3_ToQ(Q& dest, M3& const src);
//����������� �����������
inline void Q_Q_Copy(Q& dest, Q& const src);
//SLERP
void Slerp(Q & dest, Q & const src1, Q & const src2, float t);

//Matrix Headers
//��������� �������
inline void M4_M4_SetIdentity(M4& dest);
//������� ���������������
inline void M4_V3_SetScaleM(M4& dest ,V3& const src);
//������� �������� ����� ��� � ���� (�� ����� ��������� �������)
inline void M4_V4_SetRotate(M4& dest, V4& const src);
//������� �������� ����� ���� ������ (�� ����� ��������� �������)
inline void M4_V4_SetRotateEuler(M4& dest, V3& const src);
//������� ����������(�����������) (�� ����� ��������� �������)
inline void M4_M4V3_SetTranslate(M4& dest, V3& const src);
//������������ ������ (dest!=const src1||const src2 const src1 ����� ���� ����� const src2)
inline void M4_M4M4_Multiply(M4& dest,M4& const src1, M4& const src2);
//������������ ������� �� ������
inline void M4_M4V4_Multiply(V4& dest, M4& const src1, V4& const src2);
//�������� �������
inline void M4_M4_Invert(M4& dest, M4& const src);
//���������������� �������
inline void M4_M4_Transpose(M4& dest, M4& const src);
//LookAt �������
inline void M4_V3V3V3_SetLookAt(M4& dest, V3& const src1,V3& const src2, V3& const src3);
//Frustum M. ��������� �������
inline void M4_V3V3_Frustrum(M4& dest,V3& const src1, V3& const src2);
//������� ������������� ��������
inline void M4_V3V3_Ortho(M4& dest,V3& const src1, V3& const src2);
//���� �������
inline float S_M4_Trace(M4 &const src);
//����� ����������
inline float S_M4_FrobeniusNorm(M4 &const src);
//������������ ������� 4
inline float S_M4_Determinant(M4 &const src);
//����������� �������
inline void M4_M4_Copy(M4& dest, M4& const src);
//������� 4 � ������� 3
inline void M4_M3_ToM4(M4& dest,M3& const src);

//Matrix Headers 3
//��������� �������
inline void M3_M3_SetIdentity(M3& dest);
//������� ���������������
inline void M3_V4_SetScaleM(M3& dest ,V3& const src);
//������� �������� ����� ��� � ���� (�� ����� ��������� �������)
inline void M3_V4_SetRotate(M3& dest, V4& const src);
//������� �������� ����� ���� ������ (�� ����� ��������� �������)
inline void M3_V4_SetRotateEuler(M3& dest, V4& const src);
//������������ ������ (dest!= src1|| src2  src1 ����� ���� �����  src2)
inline void M3_M3M3_Multiply(M3& dest,M3& const src1, M3& const src2);
//������������ ������� �� ������
inline void M3_MV3_Multiply(V3& dest, M3& const src1, V3& const src2);
//�������� �������
inline void M3_M3_Invert(M3& dest, M3& const src);
//���������������� �������
inline void M3_M3_Transpose(M3& dest, M3& const src);
//���� �������
inline float S_M3_Trace(M3& const src);
//����� ����������
inline float S_M3_FrobeniusNorm(M3& const src);
//������������ ������� 3
inline float S_M3_Determinant(M3 &const src);
//����������� �������
inline void M3_M3_Copy(M3& dest, M3& const src);
//������� 3 � ������� 4
inline void M3_M4_ToM3(M3& dest,M4& const src);
//Vector Headers
//������� ��������� ������ 3
inline void V3_V3_SetIdentity(V3& dest);
// ������� ��������� ������ 4
inline void V4_V4_SetIdentity(V4& dest);
//��������� ������������
inline void V3_V3V3_CrossProduct(V3& dest, V3& const src1, V3& const src2);
//��������� ������������
inline float S_V3V3_DotProduct(V3& const src1, V3& const src2);
//��������� ������������
inline float S_V3V3V3_TrippleProduct(V3& dest, V3& const src1, V3& const src2, V3& const src3);
// ������ �������
inline float S_V3_EuclideanNorm(V3& const src);
//���� ����� ���������
inline float A_V3V3_Angle(V3& const src1, V3& const src2);
//������������ ������� 3
inline void V3_V3_Normalize(V3& dest, V3& const src);
//������������ ������� �� ������� ���������
inline void V4_V4_Normalize(V4& dest, V4&const src);
//�������� ��������
inline void V3_V3V3_Plus(V3& dest, V3& const src1, V3& const src2);
inline void V4_V4V4_Plus(V4& dest, V4& const src1, V4& const src2);
//��������� ��������
inline void V3_V3V3_Minus(V3& dest, V3& const src1,V3& const src2);
inline void V4_V4V4_Minus(V4& dest, V4& const src1, V4& const src2);
//��������� ������� 3 �� ������
inline void V3_SV3_Product(V3& dest, V3& const src1, float src2);
//��������� ������� 4 �� ������
inline void V4_SV4_Product(V4& dest, V4& const src1, float src2);
//����������� ������� 3
inline void V3_V3_Copy(V3& dest, V3& const src);
//����������� ������� 4
inline void V4_V4_Copy(V4& dest, V4& const src);
//������ 3 � ������ 4
inline void V4_V3_ToV4(V4& dest, V3& const src);
//������ 4 � ������ 3
inline void V3_V4_ToV3(V3& dest, V4& const src);
}