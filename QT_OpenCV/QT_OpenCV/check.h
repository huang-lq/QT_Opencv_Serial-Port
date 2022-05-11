#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat img;
vector <vector<int>> newpoints;
int b=0;
vector < vector <int>> mycolors{ {138,25,93,179,255,255},         
							   {0,136,189,179,255,255} };         
vector<Scalar>mycolorvalues{ { 255,0,255 },                  
								{ 0,255,255 } };                   

Point getContours(Mat imgdil)
{
	
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	findContours(imgdil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>>conPoly(contours.size());
	vector<Rect>boundRect(contours.size());

	Point mypoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 1000)

		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

			mypoint.x = boundRect[i].x + boundRect[i].width / 2;
			mypoint.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}
	}
	return mypoint;
}

vector <vector<int>>  findcolor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < mycolors.size(); i++)
	{
		Scalar lower(mycolors[i][0], mycolors[i][1], mycolors[i][2]);
		Scalar upper(mycolors[i][3], mycolors[i][4], mycolors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point mypoint = getContours(mask);
		if (mypoint.x != 0 && mypoint.y != 0)
		{
			b = 1;
			newpoints.push_back({ mypoint.x,mypoint.y,i });
		}
	}
	return newpoints;
}

void drawcri(vector <vector<int>> newpoints, vector<Scalar>mycolorvalues)
{
	for (int i = 0; i < newpoints.size(); i++)
	{
		circle(img, Point(newpoints[i][0], newpoints[i][1]), 10, mycolorvalues[newpoints[i][2]], FILLED);
	}
}
