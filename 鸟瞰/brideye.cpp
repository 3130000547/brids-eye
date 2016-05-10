#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <string>//�ļ�����

// using namespace cvut;
//***********----------class Img-----------***********
#include <vector>
#include "iostream"
#include "stdio.h"
/*IMPORTANT*/
using namespace std;  
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
typedef unsigned char byte ;
//#pragma comment(lib,"cxcore.lib")
//#pragma comment(lib,"cv.lib")
//#pragma comment(lib,"highgui.lib")
//"cvaux.lib" "cvcam.lib"  "ml.lib"
//class Color-----
class Color{
public:
	static	CvScalar red;	
	static	CvScalar green;	
	static	CvScalar blue;
	static	CvScalar black;
	static CvScalar white;
	
};
CvScalar Color::green=CV_RGB(0,255,0);
CvScalar Color::red  =CV_RGB(255,0,0);
CvScalar Color::blue =CV_RGB(0,0,255);
CvScalar Color::black=CV_RGB(0,0,0);
CvScalar Color::white=CV_RGB(255,255,255);
//  --- End Class Color------

// ----  CvMat ���ݹ�ȡ------
//CImag ��
class CImg{
public:
	IplImage *img;
	IplImage *img_32FC1;
	
	
	CImg(){
		img=NULL;
		img_32FC1=NULL;
	}
	
	//-----------------------------------Function here;
	//---static Functions declaration ---------
	static IplImage*  CreateImg(int h=3,int w=3,int typeOfData=8,int nChannels=1)
	{
		IplImage *retImg=cvCreateImage(cvSize(w,h),typeOfData,nChannels);
		return retImg;
	}//end createImg()
	
	static	void ShowImg(int win_num,IplImage *src){
		
		char windowName[22];
		sprintf_s(windowName,"�ⲿimg_%d\0",win_num);
		cvNamedWindow(windowName,1);
		cvShowImage(windowName,src);
		
	}//end showImg;
	
	void ReSizeToFrame(int Frame_size=128*2 )//���ﲢ������ͼƬ�������֪��img->height ,���ˣ���Ϊ��ĺ����������˽��(����������ǹ����ģ�����IplImage *img;IplImage *img_32FC1
	
	{
		int k=Frame_size;
		
		int h=img->height;
		int w=img->width;
		
		int max=h>w?h:w;
		float rate=(float)(max*1.0/k);//rate����//������﷨����εģ�Ϊ�ο���������������Ϊ���������Σ���Ϊ���õ���img ,������ȴ��Img_32FC1;
		float w1=w/rate;
		float h1=h/rate;
		CvSize sz=cvSize((int)w1,(int)h1);
		
		IplImage *tempimg=cvCreateImage(sz,8,1);
		cvResize(img,tempimg,CV_INTER_LINEAR); //CV_INTER_CUBIC CV_INTER_LINEAR
		img=tempimg;
		
	}//end Resize();
	
	
	
	void LoadToGrayImg(char *FilePath){
		
		IplImage *openImg=cvLoadImage(FilePath,-1);
		if(openImg->nChannels==3)
		{
			IplImage *retGrayImage=cvCreateImage(cvGetSize(openImg),8,1);
			cvCvtColor(openImg,retGrayImage,CV_BGR2GRAY);//����ʹ��cvCvColor
			img=retGrayImage;
			
		}
		else 
			if(openImg->nChannels==1)
				img=openImg;
			else cout<<"Image DataType Not supported!\n";//��ͨ���ĻҶ�ֵת�� always like that 
			
	}//end LoadToGrayImg()
	
	void ShowImg(int win_num=0){
		
		char windowName[22];
		sprintf_s(windowName,"img_%d\0",win_num);
		cvNamedWindow(windowName,1);
		cvShowImage(windowName,img);
		
	}//end showImg;
	void GetImg_32FC1()
	{
		img_32FC1=cvCreateImage(cvGetSize(img),32,1);
		cvConvertScale(img,img_32FC1,1.0);//����ͬ���Ż��������ǣ�
	}//end GetImg_32FC1()
	
	void AdaptiveThreshold(void)
	{
		assert(img->nChannels==1);   //û��include assert Ϊ�ο���ʹ��
		cvAdaptiveThreshold( img, img, 255);//��ֵ�� 
	};
	void Smooth(int Temp_Size=5){//��˹ƽ��
		cvSmooth( img, img, CV_GAUSSIAN, Temp_Size);
	}// End Smooth
	void Threshold(int thresh=119){
		cvThreshold( img, img, thresh, 255,CV_THRESH_BINARY );
	}//End Threshold
	void EqualizeHist(){
		cvEqualizeHist(img,img);
		
	}
	void Canny(int param1=50,int param2=200){//canny ��Ե���
		IplImage *image0=img;
		cvCanny(  image0, image0, param1,  param1, 3 );
	}
	//
	
	static void ShowMatrixData(CvMat *mat){//��debug ����������
#define CV_MAT_PElem(mat__,mytype,I,J) ((mytype*)cvPtr2D(mat__,I,J))//����� �����mytype ���ڿ���ֱ�ӻ���
		int type=CV_MAT_TYPE(mat->type);//Ϊ�β�ֱ��mat->type 
		int i,j;
		int t;
		
		
		int h=mat->height;
		int w=mat->width;
		
		int nchannels=1;
		if(type==CV_64FC3||type==CV_64FC2||type==CV_64FC1)//��Ϊmatû��channels����Ҫ�ر�ָ����  ���ﲻ��switch case 
		{
			if(type==CV_64FC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_64FC2)
			{
				nchannels=2;
			}
			else if(type==CV_64FC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					
					for(t=0;t<nchannels;t++)
					{
						double *dp=CV_MAT_PElem(mat,double,i,j);
						printf(" %4.2f ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end double
		
		else if(type==CV_32FC3||type==CV_32FC2||type==CV_32FC1)
		{
			if(type==CV_32FC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_32FC2)
			{
				nchannels=2;
			}
			else if(type==CV_32FC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						float *dp=CV_MAT_PElem(mat,float,i,j);
						//	cout<<" "<<dp[t]<<" ";
						printf(" %4.2f ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end float
		else if(type==CV_32SC3||type==CV_32SC2||type==CV_32SC1)
		{
			if(type==CV_32SC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_32SC2)
			{
				nchannels=2;
			}
			else if(type==CV_32SC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						int *dp=CV_MAT_PElem(mat,int,i,j);
						printf(" %3d ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end int
		else if(type==CV_8UC3||type==CV_8UC2||type==CV_8UC1)
		{
			if(type==CV_8UC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_8UC2)
			{
				nchannels=2;
			}
			else if(type==CV_8UC1)
			{
				nchannels=1; 
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						byte *dp=CV_MAT_PElem(mat,byte,i,j);
						printf(" %3d ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end uchar//֧����������
		else cout<<"unsupported dataType!!--\n";
		
}// End SHowMatData----------------------

IplImage *GetImg_8UC3_Gray(){
	IplImage *imgC3=	CreateImg(img->height,img->width,8,3);
	cvZero(imgC3);
	IplImage *imgC1=img;
	cvMerge(imgC1,imgC1,imgC1,NULL,imgC3);//�ϲ�����ͨ��
	return imgC3;
}//End Get Img3Channels()
static	void ShowMatrixData(IplImage *src){
	CvMat header;  //��������ͷ�����������ݿռ�
	CvMat *mat	= cvGetMat(src,&header);// share the data Area
	ShowMatrixData(mat);
}//end show imgData
static CvMat* Transpose(CvMat* _mat){
	int h=_mat->width;
	int w=_mat->height;
	int type=CV_MAT_TYPE(_mat->type);
	CvMat *ret_mat=cvCreateMat(h,w,type);
	cvT(_mat,ret_mat);
	return ret_mat;
}//End Transpose() --
static CvMat* Transpose(IplImage* _img){
	CvMat header;  //��������ͷ�����������ݿռ�
	CvMat *mat	= cvGetMat(_img,&header);// share the data Area
	return Transpose(mat);
	
}//End Transpose() --

void LoadToGrayImg(IplImage*src){


	if(src->nChannels==3)
		cvCvtColor(src,img,CV_BGR2GRAY);
	else img=src;
	

}//End LoadToGrayImg(src)


~CImg(){
	if(img!=NULL)cvReleaseImage(&img);
	img_32FC1=NULL;
	cvDestroyAllWindows();
}
};//****-------- End Class CImg ---------------****** 




// *****----  STRUCT_2D_to_3D-----**********
class  C_2D_to_3D{
public:
	CvMat *_2D_pt; //Matrix 2D Point
	
	CvMat *intrinsic_matrix; //Matrix A
	CvMat *rotation_vectors; //Matrix R  -->(1,1,CV_32FC3)
	CvMat *translation_vectors;//Matrix t
	
	CvMat *_3D_pt; //Matrix 3D point
//	int image_count;
	C_2D_to_3D(){
		_2D_pt=NULL; //Matrix 2D Point
		
		intrinsic_matrix=NULL; //Matrix A
		rotation_vectors=NULL; //Matrix R  -->(1,1,CV_32FC3)
		translation_vectors=NULL;//Matrix t
		
		_3D_pt=NULL; //Matrix 3D point
	//	image_count=-1;
	}
	void showAllVals(int valNum=6){
		CImg idcImg;
		int i,j;
	
		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			//---------	show All 6 Vals------------------
			cout<<" image_count-------\n"	;                    cout<<rotation_vectors->width <<endl;
			cout<<"idcImg.ShowMatrixData(_2D_pt)\n";			 idcImg.ShowMatrixData(_2D_pt);cout<<endl;
			cout<<"idcImg.ShowMatrixData(intrinsic_matrix)\n";	 idcImg.ShowMatrixData(intrinsic_matrix);cout<<endl;
			cout<<"idcImg.ShowMatrixData(rotation_vectors)\n";   idcImg.ShowMatrixData(rotation_vectors);cout<<endl;
			cout<<"idcImg.ShowMatrixData(translation_vectors)\n";idcImg.ShowMatrixData(translation_vectors);cout<<endl;
			cout<<"idcImg.ShowMatrixData(_3D_pt)\n";             idcImg.ShowMatrixData(_3D_pt);cout<<endl;
		
			
	}	//end Show(vals= 6);
	void do_2D_to_3D(CvPoint2D32f input_pt2D=cvPoint2D32f(333,24))
	{
		CvPoint2D32f inPt2D=input_pt2D;
		CV_MAT_PElem(_2D_pt,float,0,0)[0]=inPt2D.x;
		CV_MAT_PElem(_2D_pt,float,1,0)[0]=inPt2D.y;
		CV_MAT_PElem(_2D_pt,float,2,0)[0]=1;
		
		int i,j;

		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			
			
			// get the right six vals-------------------
			for(i=0;i<1;i++)   //image_count=tr~.width--
			{
				
				CvMat* rotation_vector_per=cvCreateMat(1,1,CV_32FC3);
				CvMat* rotation_matrix=cvCreateMat(3,3,CV_32FC1); // 3 x 3
				
				
				rotation_vector_per->data.ptr=cvPtr2D(rotation_vectors,0,i);
				cvRodrigues2(rotation_vector_per,rotation_matrix);
				
				
				CvMat *A=intrinsic_matrix;
				
				CvMat *R=rotation_matrix;
				
				CvMat *B=cvCreateMat(3,3,CV_32FC1);
				CvMat *t=cvCreateMat(3,1,CV_32FC1);
				
				B=cvCloneMat(R);
				
				
				// Initial [t]--  //��õ���ͼ���t(3x1)���� �Լ� [R3+t]
				for(j=0;j<3;j++)
				{				
					float t_i_j=(CV_MAT_PElem(t,float,j,0)[0]=CV_MAT_PElem(translation_vectors,float,0,i)[j]);  //Translat_vectors->(T)
					CV_MAT_PElem(B,float,j,2)[0]+=t_i_j;   ////  ( A B=[R1,R,R3+t] )^(-1) [x1,x2,x3](T)
					
				}//end for(j)
				
				cvGEMM(A,B,1,0,1,B);
				cvInvert(B,B,CV_LU);
				cvGEMM(B,_2D_pt,1,0,0,_3D_pt,0);
				
				float _z;
				_z=CV_MAT_PElem(_3D_pt,float,2,0)[0];
				for(j=0;j<3;j++)
				{
					CV_MAT_PElem(_3D_pt,float,j,0)[0]/=_z;		
				}
				
				//	idCImg.ShowMatrixData(_3D_pt);
				
			}  //end for(i:image_count)---
			
	}//end do2D - > 3D 
	void do_3D_to_2D(CvPoint3D32f intputpt_3d=cvPoint3D32f(0,0,1))
	{
		CvPoint3D32f inPt3D=intputpt_3d;
		CV_MAT_PElem(_3D_pt,float,0,0)[0]=inPt3D.x;
		CV_MAT_PElem(_3D_pt,float,1,0)[0]=inPt3D.y;
		CV_MAT_PElem(_3D_pt,float,2,0)[0]=1;
		
		int i,j;

		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			
			
			// get the right six vals
			for(i=0;i<1;i++)   //image_count=1
			{
				// C_2D_to_3D id_2d3d;
				
				
				CvMat* rotation_vector_per=cvCreateMat(1,1,CV_32FC3);
				CvMat* rotation_matrix=cvCreateMat(3,3,CV_32FC1); // 3 x 3
				
				
				rotation_vector_per->data.ptr=cvPtr2D(rotation_vectors,0,i);
				cvRodrigues2(rotation_vector_per,rotation_matrix);
				
				
				CvMat *A=intrinsic_matrix;
				
				CvMat *R=rotation_matrix;
				
				CvMat *B=cvCreateMat(3,3,CV_32FC1);
				CvMat *t=cvCreateMat(3,1,CV_32FC1);
				
				B=cvCloneMat(R);
				
				
				// Initial [t]--  //��õ���ͼ���t(3x1)����
				for(j=0;j<3;j++)
				{
					
					float t_i_j=(CV_MAT_PElem(t,float,j,0)[0]=CV_MAT_PElem(translation_vectors,float,0,i)[j]);  //Translat_vectors->(T)
					CV_MAT_PElem(B,float,j,2)[0]+=t_i_j;   ////  ( A B=[R1,R,R3+t] )^(-1) [x1,x2,x3](T)
					
				}//end for(j)
				

				cvGEMM(R,_3D_pt,1,t,1,_2D_pt);
				cvGEMM(A,_2D_pt,1,0,1,_2D_pt);
				
				// 3D ->2D _z=1 ---
				
				float _z;
				
				_z=CV_MAT_PElem(_2D_pt,float,2,0)[0];
				
				for(j=0;j<3;j++)
				{
					
					CV_MAT_PElem(_2D_pt,float,j,0)[0]/=_z;
				}
				

				
			}  //end for(i:image_count)---
			
	 }	 //end do_3D_to_2D---
	 
};// End struct C_2D_3D-------



void main(){
	CImg idCImg;



//idCImg.ShowImg(1,idCImg.img);







//----------------------------------------------------------CImg test-------------------------------------	
	CvMat *_3D_pt=cvCreateMat(3,1,CV_32FC1);
	CvMat *_2D_pt=cvCreateMat(3,1,CV_32FC1);
	
	int i,j,t;
	ifstream fin("calibdata.txt"); /* ��������ͼ���ļ���·�� */
	ofstream fout("caliberation_result.txt");  /* ���涨�������ļ� */
	
	cout<<"��ʼ��ȡ�ǵ㡭����������\n";
	
	CvSize chess_sz=cvSize(6,4);
	int cw=chess_sz.width;
	int ch=chess_sz.height;
	int chess_WxH=cw*ch;
	CvPoint2D32f* image_pt_perImg = new CvPoint2D32f[chess_WxH];   /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	vector<CvPoint2D32f> seq_img_pt;
	string filename;
	int count=-1;
	int image_count=0;
	while (getline(fin,filename))
	{
		idCImg.LoadToGrayImg((char*)filename.c_str());
		IplImage *img_3C=idCImg.GetImg_8UC3_Gray();
		IplImage *image=idCImg.img;
		CvSize pattern_size=chess_sz;
		CvPoint2D32f* corners=image_pt_perImg;
		int corner_count;
		int find_retcode=cvFindChessboardCorners(image,pattern_size,corners,&corner_count,CV_CALIB_CB_ADAPTIVE_THRESH);
		if(find_retcode==0) continue ;  //Not Find Corners����һ��ͼ
		// Find corner All 
		image_count++;  
		
		cvFindCornerSubPix( image, 
			corners, 
			corner_count, 
			cvSize(11,11),
			cvSize(-1,-1), 
			cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 )
			);//�Դ���ȡ�Ľǵ���о�ȷ��
		// ---- Push to seq----
		for(i=0;i<corner_count;i++)
			seq_img_pt.push_back(corners[i]);
		
		// -----Draw ChessBoard---
		cvDrawChessboardCorners( img_3C, pattern_size, corners, corner_count, find_retcode);//������ͼƬ�б�ǽǵ�
		
		//----  show Image()------
		idCImg.ShowImg(0,img_3C);
		cvWaitKey(100/4);//-----------waitTime-----
	}  // End while(getLine())----
	
	delete [] image_pt_perImg;
	cout<<"Corners finded---\n";
	int corner_all=seq_img_pt.size();
	
	//---- calibration----
	CvSize square_size = cvSize(10,10);  
	
	CvMat *object_points=cvCreateMat(1,image_count*chess_WxH,CV_32FC3);
	CvMat *image_points=cvCreateMat(1,image_count*chess_WxH,CV_32FC2);
	
	CvMat *points_counts=cvCreateMat(1,image_count,CV_32SC1); //ÿ��ͼ�������
	
	CvSize image_size=chess_sz;
	
	CvMat *intrinsic_matrix=cvCreateMat(3,3,CV_32FC1);
	CvMat *distortion_coeffs=cvCreateMat(1,4,CV_32FC1);
	
	CvMat *rotation_vectors=cvCreateMat(1,image_count,CV_32FC3);
	CvMat *translation_vectors=cvCreateMat(1,image_count,CV_32FC3);
	// ---- Initial Object_points and Image_points ----
	//1. object_points--
	for(t=0;t<image_count;t++)
	{
		for(i=0;i<ch;i++)
		{
			for(j=0;j<cw;j++)
			{
				CvPoint3D32f &PtTmp3D32f=*((CvPoint3D32f*)CV_MAT_PElem(object_points,float,0,t*chess_WxH+i*cw+j));
				PtTmp3D32f.x=j*square_size.width;
				PtTmp3D32f.y=i*square_size.height;
				PtTmp3D32f.z=1; //with Z = 1;
			}//end for(j)
		}//end for(i)
	}//end for(t)
	idCImg.ShowMatrixData(object_points);
	
	// 2.image_points----
	
	for(i=0;i<corner_all;i++)
	{
		CvPoint2D32f& Pt2D32f=*(CvPoint2D32f*)CV_MAT_PElem(image_points,float,0,i);
		Pt2D32f.x=seq_img_pt[i].x; //���п���ֱ��Ѱֵ
		Pt2D32f.y=seq_img_pt[i].y; 
	}
	
	//idCImg.ShowMatrixData(image_points);
	
	// 3.point_counts ��ͼ�ĵ�����
	for(i=0;i<=image_count;i++)
	{
		int &cnt_perTemp=*CV_MAT_PElem(points_counts,int,0,i);
		cnt_perTemp=chess_WxH;
	}
	
	i=translation_vectors->rows;
	
	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
  CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

	cvCalibrateCamera2(
		,
		image_pointsobject_points,
		points_counts,
		image_size,
		intrinsic_matrix,
		distortion_coeffs,
		rotation_vectors,
		translation_vectors,
		0);	
	
	// -- END CALIBRATION-----
	
	//---test ---Struct C_2D_to_3D


		intrinsic_matrix,distortion_coeffs,NULL,NULL,0);
	//����õ������������
	cout<<"������ڲ�������Ϊ��\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,0,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,0,1)
		<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,0,2)
		<<"\n\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,1,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,1,1)
		<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,1,2)
		<<"\n\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,2,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,2,1)
		<<"          "<<CV_MAT_ELEM(*intrinsic_matrix,float,2,2)
		<<"\n\n";

	cout<<"����ϵ������Ϊ��\n";
	cout<<CV_MAT_ELEM(*distortion_coeffs,float,0,0)<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,1,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,2,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,3,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,4,0)
		<<"\n\n";
	//���õ����������������Ϊxml�ļ�
	cvSave("Intrinsics.xml",intrinsic_matrix);
	cvSave("Distortion.xml",distortion_coeffs);
	cout<<"��������󡢻���ϵ�������Ѿ��ֱ�洢����ΪIntrinsics.xml��Distortion.xml�ĵ���\n\n";

	CvMat * intrinsic=(CvMat *)cvLoad("Intrinsics.xml");
	CvMat * distortion=(CvMat *)cvLoad("Distortion.xml");

	IplImage * mapx=cvCreateImage(cvGetSize(show_colie),IPL_DEPTH_32F,1);
	IplImage * mapy=cvCreateImage(cvGetSize(show_colie),IPL_DEPTH_32F,1);

	cvInitUndistortMap(intrinsic,distortion,mapx,mapy);

	cvNamedWindow("ԭʼͼ��",1);
	cvNamedWindow("�������ͼ��",1);

	cout<<"������궨�������\n\n";
	//�Զ�����ÿ��ͼƬ���л���У��
	//while(show_colie)
	//{
		IplImage * clone=cvCloneImage(show_colie);
		cvShowImage("ԭʼͼ��",show_colie);
		cvRemap(clone,show_colie,mapx,mapy);
		cvReleaseImage(&clone);
		cvShowImage("�ǻ���ͼ��",show_colie);

		//if(cvWaitKey(10)=='e')
		//{
		//	break;
		//}

		//show_colie=cvQueryFrame(capture1);
	//}


	int board_w = 7;//�ǵ���
	int board_h = 7;
	int board_n  = board_w * board_h;
	CvSize board_sz = cvSize( board_w, board_h );
	//�����������õ��ڲ����ͻ���ϵ��
	CvMat *intrinsic2 = intrinsic_matrix;
    CvMat *distortion2 = distortion_coeffs;
	IplImage *image2 = 0, *gray_image2 = 0;
	//�����ת����ͼƬ
	if((image2 = cvLoadImage("before.jpg"))== 0){
		printf("Error: Couldn't load the picture!\n");
		return -1;
	}
	//����һ�Ÿ�ͼƬ�ĻҶ�ͼƬ
	gray_image2 = cvCreateImage(cvGetSize(image2),8,1);
    cvCvtColor(image2, gray_image2, CV_BGR2GRAY);
	cout<<"�Ҷ�ͼ��ת�����\n";

	

	cvNamedWindow("Checkers");
    CvPoint2D32f* corners2 = new CvPoint2D32f[ board_n ];
    int corner_count2 = 0;
	//Ѱ�ҽǵ�
    int found2 = cvFindChessboardCorners(
        image2,
        board_sz,
        corners2,
        &corner_count2, 
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
      );
	
	//�õ��ǵ�Ĵξ���λ��
	cvFindCornerSubPix(gray_image2, corners2, corner_count2, 
			  cvSize(11,11),cvSize(-1,-1), 
			  cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

	//GET THE IMAGE AND OBJECT POINTS:
	//Object points are at (r,c): (0,0), (board_w-1,0), (0,board_h-1), (board_w-1,board_h-1)
	//That means corners are at: corners[r*board_w + c]
	CvPoint2D32f objPts[4], imgPts[4];//����4������ɵ�����    �ĸ����ϵĵ�
	objPts[0].x = 0;         objPts[0].y = 0; 
	objPts[1].x = board_w-1; objPts[1].y = 0; 
	objPts[2].x = 0;         objPts[2].y = board_h-1;
	objPts[3].x = board_w-1; objPts[3].y = board_h-1; 
	imgPts[0] = corners2[0];
	imgPts[1] = corners2[board_w-1];
	imgPts[2] = corners2[(board_h-1)*board_w];
	imgPts[3] = corners2[(board_h-1)*board_w + board_w-1];

	//����Щ���ò�ͬ��ɫ��ԲȦ��ʾ
	cvCircle(image2,cvPointFrom32f(imgPts[0]),9,CV_RGB(0,0,255),3);
	cvCircle(image2,cvPointFrom32f(imgPts[1]),9,CV_RGB(0,255,0),3);
	cvCircle(image2,cvPointFrom32f(imgPts[2]),9,CV_RGB(255,0,0),3);
	cvCircle(image2,cvPointFrom32f(imgPts[3]),9,CV_RGB(255,255,0),3);
	cout<<"Ѱ�ҽǵ㲢������\n";
	//�������нǵ㲢��չʾ����
	cvDrawChessboardCorners(image2, board_sz, corners2, corner_count2, found2);
    cvShowImage( "Checkers", image2 );
	cvSaveImage("Checkers.jpg",image2);

	//FIND THE HOMOGRAPHY
	CvMat *H = cvCreateMat( 3, 3, CV_32F);
	CvMat *H_invt = cvCreateMat(3,3,CV_32F);
	cvGetPerspectiveTransform(objPts,imgPts,H);//�õ�͸�ӱ任������� HΪ3*3���󣻴�ƽ�浽ƽ���ǵ�Ӧ�Ա任

	float Z = 25;
	int key = 0;
	IplImage *birds_image = cvCloneImage(image2);
	cvNamedWindow("Birds_Eye");
	cout<<"��ʼ�������ͼ�任\n";
    while(key != 27) {
	   CV_MAT_ELEM(*H,float,2,2) = Z;
	   cout<<'1';
	   cvWarpPerspective(image2,birds_image,H,    //cvWarpPerspective����������ͼ���͸�ӱ任
			CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS );//��ͼ�����͸�ӱ任�õ��任���ͼƬbirds_image��
			//CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS��ȷ�����е������ͼת��
	   cout<<'2';
	   cvShowImage("Birds_Eye", birds_image);
	   cout<<"3";
	   cvSaveImage("Birds_Eye.jpg",birds_image);
	   cout<<"���ͼ�任�ɹ�";
	   key = cvWaitKey();
	   if(key == 'u') Z += 0.5;
	   if(key == 'd') Z -= 0.5;
	  
	}

	//SHOW ROTATION AND TRANSLATION VECTORS
	CvMat* image_points3  = cvCreateMat(4,1,CV_32FC2);
	CvMat* object_points3 = cvCreateMat(4,1,CV_32FC3);
	for(int i=0;i<4;++i){
		CV_MAT_ELEM(*image_points,CvPoint2D32f,i,0) = imgPts[i];
		CV_MAT_ELEM(*object_points,CvPoint3D32f,i,0) = cvPoint3D32f(objPts[i].x,objPts[i].y,0);
	}

	CvMat *RotRodrigues   = cvCreateMat(3,1,CV_32F);
	CvMat *Rot   = cvCreateMat(3,3,CV_32F);
	CvMat *Trans = cvCreateMat(3,1,CV_32F);
	cvFindExtrinsicCameraParams2(object_points,image_points,
			intrinsic,distortion,
			RotRodrigues,Trans);
	cvRodrigues2(RotRodrigues,Rot);//��ά��ת

	//��������ļ�
	cvSave("Rot.xml",Rot);
	cvSave("Trans.xml",Trans);
	cvSave("H.xml",H);
	cvInvert(H,H_invt);
    cvSave("H_invt.xml",H_invt); //Bottom row of H invert is horizon line
	return 0;
}
