#include "StdAfx.h"
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>


//#pragma comment(lib, "ml.lib") //���ӽ�һ����
//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cvaux.lib")
//#pragma comment(lib, "cvcam.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "cxts.lib")
//#pragma comment(lib, "highgui.lib")
//#pragma comment(lib, "cvhaartraining.lib")

using namespace std;

int main()
{
	int cube_length=12;
	//CvCapture* capture;
	//capture=cvCreateCameraCapture(0); //������ͷ������ͼ��
	//if(capture==0)
	//{
	//	printf("�޷���������ͷ�豸��\n\n");
	//	return 0;
	//}
	//else
	//{
	//	printf("��������ͷ�豸�ɹ�����\n\n");
	//}
	//IplImage* frame = NULL;
	//cvNamedWindow("�����֡��ȡ����",1);
	//printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");
	//
	//
	////��ȡ���б궨Ҫʹ�õ�ͼƬ
	//int number_image=1;
	//char *str1=".jpg";
	//char filename[20]="";

	//while(true)
	//{
	//	frame=cvQueryFrame(capture);//�ڴ�����ʾ���񵽵�ÿһ֡
	//	if(!frame)
	//		break;
	//	cvShowImage("�����֡��ȡ����",frame);
	//	//������c��ʱ������������
	//	if(cvWaitKey(10)=='c')
	//	{
	//		sprintf_s (filename,"%d.jpg",number_image);
	//		cvSaveImage(filename,frame);
	//		cout<<"�ɹ���ȡ��ǰ֡�������ļ���"<<filename<<"����...\n\n";
	//		printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");
	//		number_image++;
	//	}
	//	//������q��ʱ�˳������
	//	else if(cvWaitKey(10)=='q')
	//	{
	//		printf("��ȡͼ��֡�������...\n\n");
	//		cout<<"���ɹ���ȡ"<<--number_image<<"֡ͼ�񣡣�\n\n";
	//		break;
	//	}
	//}
	////�ͷ��ڴ�
	//cvReleaseImage(&frame);
	//cvDestroyWindow("�����֡��ȡ����");	
	int number_image=14;
	char filename[14] = "";
	char *str = ".jpg";
	for (int i = 1; i<=number_image;i++)
	{
		sprintf_s (filename,"%d.jpg",i);
	}
	for (int j=0;j<14;j++){
		cvLoadImage(&filename[j]);
	}

	CvSize board_size=cvSize(12,12);// Cvsizes:OpenCV�Ļ�����������֮һ����ʾ������С��������Ϊ���ȡ���CvPoint�ṹ���ƣ������ݳ�Ա��integer���͵�width��height��
	int board_width=board_size.width;
	int board_height=board_size.height;
	int total_per_image=board_width*board_height;
	CvPoint2D32f * image_points_buf = new CvPoint2D32f[total_per_image];//����һ������Ԫ��Ϊ�������
	CvMat * image_points=cvCreateMat(number_image*total_per_image,2,CV_32FC1);
	CvMat * object_points=cvCreateMat(number_image*total_per_image,3,CV_32FC1);
	CvMat * point_counts=cvCreateMat(number_image,1,CV_32SC1);//�洢ÿ��ͼƬ��ĳ����Ϣ
	CvMat * intrinsic_matrix=cvCreateMat(3,3,CV_32FC1);//��������ڲ�������
	CvMat * distortion_coeffs=cvCreateMat(5,1,CV_32FC1);//������Ķ�����ϵ��

	int count;
	int found;
	int step;
	int successes=0;
	int a=1;
	int number_image_copy = number_image;
	IplImage * show;
	cvNamedWindow("RePlay",1);

	while(a<=number_image_copy)//���ÿ��ͼƬ������
	{
		sprintf_s (filename,"%d.jpg",a);
		show=cvLoadImage(filename,-1);

		found=cvFindChessboardCorners(show,board_size,image_points_buf,&count,
			CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);//���ڵ�ǰ��ͼƬ����Ѱ�ҽǵ�
		if(found==0)//û���ҵ����нǵ㣬ʧ��
		{		
			cout<<"��"<<a<<"֡ͼƬ�޷��ҵ����̸����нǵ�!\n\n";
			cvNamedWindow("RePlay",1);
			cvShowImage("RePlay",show);
			cvWaitKey(0);

		}
		else
		{	//�ɹ�
			//�ҵ�ͼƬ�еĽǵ㲢��չʾ����
			cout<<"��"<<a<<"֡ͼ��ɹ����"<<count<<"���ǵ�...\n";
			cvNamedWindow("RePlay",1);
			IplImage * gray_image= cvCreateImage(cvGetSize(show),8,1);
			cvCvtColor(show,gray_image,CV_BGR2GRAY);//����ǰͼƬ��Ϊ�Ҷ�ͼƬ
			cout<<"��ȡԴͼ��Ҷ�ͼ�������...\n";
			cvFindCornerSubPix(gray_image,image_points_buf,count,cvSize(11,11),cvSize(-1,-1),
				cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));//�Եõ��ĻҶ�ͼ����������ؾ��ȵĴ���
			cout<<"�Ҷ�ͼ�����ػ��������...\n";
			cvDrawChessboardCorners(show,board_size,image_points_buf,count,found);
			cout<<"��Դͼ���ϻ��ƽǵ�������...\n\n";
			cvShowImage("RePlay",show);
			cvWaitKey(0);
		}
		//�궨ͼƬ������������������������
		if(total_per_image==count)
		{
			step=successes*total_per_image;
			for(int i=step,j=0;j<total_per_image;++i,++j)
			{
				CV_MAT_ELEM(*image_points,float,i,0)=image_points_buf[j].x;
				CV_MAT_ELEM(*image_points,float,i,1)=image_points_buf[j].y;
				CV_MAT_ELEM(*object_points,float,i,0)=(float)(j/cube_length);
				CV_MAT_ELEM(*object_points,float,i,1)=(float)(j%cube_length);
				CV_MAT_ELEM(*object_points,float,i,2)=0.0f;
			}
			CV_MAT_ELEM(*point_counts,int,successes,0)=total_per_image;
			successes++;
		}
		a++;
	}

	cvReleaseImage(&show);
	cvDestroyWindow("RePlay");


	cout<<"*********************************************\n";
	cout<<number_image<<"֡ͼƬ�У��궨�ɹ���ͼƬΪ"<<successes<<"֡...\n";
	cout<<number_image<<"֡ͼƬ�У��궨ʧ�ܵ�ͼƬΪ"<<number_image-successes<<"֡...\n\n";
	cout<<"*********************************************\n\n";

	cout<<"���������ʼ����������ڲ���...\n\n";

	//CvCapture* capture1;
	//capture1=cvCreateCameraCapture(0);
	IplImage * show_colie;
	show_colie=cvLoadImage("1.jpg");

	CvMat * object_points2=cvCreateMat(successes*total_per_image,3,CV_32FC1);
	CvMat * image_points2=cvCreateMat(successes*total_per_image,2,CV_32FC1);
	CvMat * point_counts2=cvCreateMat(successes,1,CV_32SC1);
	//������궨
	for(int i=0;i<successes*total_per_image;++i)
	{
		CV_MAT_ELEM(*image_points2,float,i,0)=CV_MAT_ELEM(*image_points,float,i,0);
		CV_MAT_ELEM(*image_points2,float,i,1)=CV_MAT_ELEM(*image_points,float,i,1);
		CV_MAT_ELEM(*object_points2,float,i,0)=CV_MAT_ELEM(*object_points,float,i,0);
		CV_MAT_ELEM(*object_points2,float,i,1)=CV_MAT_ELEM(*object_points,float,i,1);
		CV_MAT_ELEM(*object_points2,float,i,2)=CV_MAT_ELEM(*object_points,float,i,2);
	}

	for(int i=0;i<successes;++i)
	{
		CV_MAT_ELEM(*point_counts2,int,i,0)=CV_MAT_ELEM(*point_counts,int,i,0);
	}

	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&point_counts);

	CV_MAT_ELEM(*intrinsic_matrix,float,0,0)=1.0f;
	CV_MAT_ELEM(*intrinsic_matrix,float,1,1)=1.0f;

	cvCalibrateCamera2(object_points2,image_points2,point_counts2,cvGetSize(show_colie),
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


	int board_w = board_width;//�ǵ���
	int board_h = board_height;
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

	//��������
    //IplImage* mapx2 = cvCreateImage( cvGetSize(image2), IPL_DEPTH_32F, 1 );
    //IplImage* mapy2 = cvCreateImage( cvGetSize(image2), IPL_DEPTH_32F, 1 );
    //cvInitUndistortMap(
     // intrinsic2,
    //  distortion2,
     // mapx2,
     // mapy2
    //);
	//IplImage *t = cvCloneImage(image2);
    //cvRemap( t, image2, mapx2, mapy2 );//�õ��������ͼƬimage;t��Ϊ���ͼ��image��Ϊ���ͼ��
	//cout<<"ͼƬ�������\n";

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
	if(!found){
		printf("Couldn't aquire checkerboard on the picture, only found %d of %d corners\n",
				corner_count2,board_n);
		return -1;
	}
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
		CV_MAT_ELEM(*image_points2,CvPoint2D32f,i,0) = imgPts[i];
		CV_MAT_ELEM(*object_points2,CvPoint3D32f,i,0) = cvPoint3D32f(objPts[i].x,objPts[i].y,0);
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
