/*
 * animaly_detection_util.cpp
 *
 * Author: elad baal tzdaka
 * Id: 312531973
 */

#include <math.h>
#include <cmath>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
	float sum=0;
	for (int i = 0; i < size; i++)
	{
		sum=sum+x[i];
	}
	return (sum/size);
	
}

// returns the variance of X and Y
float var(float* x, int size){
	float E1=0;
	float E2=0;
	for (int i = 0; i < size; i++)
	{
		E1=E1+pow(x[i],2);
	}
	E1=E1/size;
	E2=avg(x,size);
	E2=pow(E2,2);
	return (E1-E2);


	
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	float avgX=avg(x,size);
	float avgY=avg(y,size);
	float MultiXY=0;
	for (int i = 0; i < size; i++)
	{
		MultiXY=MultiXY+(x[i]*y[i]);
	}
	MultiXY=MultiXY/size;
	return MultiXY-(avgX*avgY);

	
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	float covXY=cov(x,y,size);
	float varX=var(x,size);
	float varY=var(y,size);
	varX=sqrt(varX);
	varY=sqrt(varY);
	return covXY/(varX*varY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	float arrOfX[size];
	float arrOfY[size];
	float a,b;
	for (int i = 0; i < size; i++)
	{
		arrOfX[i]=points[i]->x;
		arrOfY[i]=points[i]->y;
	}
	a=cov(arrOfX,arrOfY,size)/var(arrOfX,size);
	b=avg(arrOfY,size)-(a*avg(arrOfX,size));
	return Line(a,b);
	
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	Line line=linear_reg(points,size);
	return dev(p,line);
}

// returns the deviation between point p and the line -->|y'-y|
float dev(Point p,Line l){
	return abs(l.f(p.x)-p.y);
}




