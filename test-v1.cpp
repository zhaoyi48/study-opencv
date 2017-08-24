#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<iostream>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string>
#include<Windows.h>
#include <tchar.h>
#include<atlstr.h>
#include<math.h>

using namespace cv;
using namespace std;

Mat openfile(string path) {
	Mat data;
	data = imread(path);
	return data;
}

int verify(Mat data,Mat solve) {
	Mat_<Vec3b>::iterator d_it = data.begin<Vec3b>();
	Mat_<Vec3b>::iterator d_itend = data.end<Vec3b>();
	Mat_<Vec3b>::iterator s_it = solve.begin<Vec3b>();
	Mat_<Vec3b>::iterator s_itend = solve.end<Vec3b>();
	for (; d_it != d_itend; ++d_it) {
		if (d_it[0] != s_it[0] || d_it[1] != s_it[1] || d_it[2] != s_it[2]) {
			return 0;
		}
		++s_it;
	}
	return 1;
}

vector<string> test() {
	vector<string> imgPaths;
	string path = "D:/Downloads/lfw/lfw/";
	string filepath = "D:\\Downloads\\lfw\\lfw\\";
	string dirpath = "D:\\Downloads\\lfw\\lfw\\*";
	wstring stemp = wstring(dirpath.begin(), dirpath.end());
	WIN32_FIND_DATA dirData;
	HANDLE dirFind = FindFirstFile(stemp.c_str(), &dirData);
	if (dirFind != INVALID_HANDLE_VALUE) {
		FindNextFile(dirFind, &dirData);
		FindNextFile(dirFind, &dirData);
		do {
			//wcout << dirData.cFileName << endl;
			filepath = filepath+CW2A(dirData.cFileName)+"\\*";
			//cout << filepath << endl;
			wstring temp = wstring(filepath.begin(), filepath.end());
			WIN32_FIND_DATA fileData;
			HANDLE fileFind = FindFirstFile(temp.c_str(), &fileData);
			if (fileFind != INVALID_HANDLE_VALUE) {
				FindNextFile(fileFind, &fileData);
				FindNextFile(fileFind, &fileData);
				do {
					path = path+ CW2A(dirData.cFileName)+"/"+CW2A(fileData.cFileName);
					cout << path << endl;
					imgPaths.push_back(path);
					path = "D:/Downloads/lfw/lfw/";
				} while (FindNextFile(fileFind, &fileData));
			}
			filepath = "D:\\Downloads\\lfw\\lfw\\";
		} while (FindNextFile(dirFind, &dirData));
		FindClose(dirFind);
	}
	return imgPaths;
}
//fail - image:0--1--2--463--3228
int average(Mat image1, Mat image2, Mat image3, Mat image4, Mat image5, Mat solve) {
	Mat data;
	data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4;
	/*namedWindow("test");
	imshow("test", data);
	waitKey(0);*/
	int nl = data.rows/25;
	int nc = data.cols*data.channels()/25;
	for (int i = 0; i < nl; i++)
	{
		uchar* pixel = data.ptr<uchar>(i);
		uchar* pixels = solve.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			if (pixel[j] != pixels[j]) {
				return 0;
			}
		}
	}
	return 1;
}

double ly1(Mat data, Mat solve) {
	int nl = data.rows;
	int nc = data.cols*data.channels();
	int hit = 0;
	int all = nl*nc;
	for (int i = 0; i < nl; i++)
	{
		uchar* pixel = data.ptr<uchar>(i);
		uchar* pixels = solve.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			if (abs(pixel[j] - pixels[j])<8) {
				hit++;
			}
		}
	}
	double mrate = (double)hit / (double)all;
	//cout << mrate*100 << "%" << endl;
	return mrate;
}


int averagef(Mat image1, Mat image2, Mat image3, Mat image4, Mat image5, Mat solve) {
	int i = image1.rows;
	int nc = image1.cols*image1.channels()/50;
		uchar* pixel1 = image1.ptr<uchar>(i);
		uchar* pixel2 = image2.ptr<uchar>(i);
		uchar* pixel3 = image3.ptr<uchar>(i);
		uchar* pixel4 = image4.ptr<uchar>(i);
		uchar* pixel5 = image5.ptr<uchar>(i);
		uchar* pixels = solve.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			if (pixels[j] != (pixel1[j] + pixel2[j] + pixel3[j] + pixel4[j] + pixel5[j]) / 5) {
				return 0;
			}
		}
	return 1;
}

void colorReduce(Mat image) {
	int div = 64;
	int nl = image.rows;
	int nc = image.cols*image.channels();
	//cout << "success-image:" << nl << "--" << nc<< endl;
	for (int  i = 0; i < nl; i++)
	{
		uchar* data = image.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			data[j] = data[j] / div*div + div / 2;
		}
	}
}
int mainb() {
	vector<string> imgPaths;
	vector<int> possable;
	Mat image1, image2, image3, image4, image5, solve;
	solve = imread("0.jpg");
	if (solve.empty()) {
		cout << "file no found" << endl;
	}
	//colorReduce(solve);
	namedWindow("solve");
	imshow("solve", solve);
	imgPaths=test();
	cout << imgPaths.size() << endl;
	int count = imgPaths.size();
	/*image1 = openfile(imgPaths.at(0).c_str());
	namedWindow("test");
	imshow("test", image1);
	cout << imgPaths.at(0).c_str() << endl;
	if (image1.empty()) {
		cout << "error" << endl;
	}*/
	//1--2--22--14--4
	//9--2--22--14--4
	//11506--13201--453--3--7252
	/*cout << imgPaths.at(0).c_str() << endl;
	cout << imgPaths.at(13201).c_str() << endl;
	cout << imgPaths.at(453).c_str() << endl;
	cout << imgPaths.at(3).c_str() << endl;
	cout << imgPaths.at(7252).c_str() << endl;
	image1 = openfile(imgPaths.at(0).c_str());
	image2 = openfile(imgPaths.at(13201).c_str());
	image3 = openfile(imgPaths.at(453).c_str());
	image4 = openfile(imgPaths.at(3).c_str());
	image5 = openfile(imgPaths.at(7252).c_str());
	Mat data;
	data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4;
	namedWindow("test");
	imshow("test", data);
	namedWindow("image1");
	imshow("image1", image1);
	namedWindow("image2");
	imshow("image2", image2);
	namedWindow("image3");
	imshow("image3", image3);
	namedWindow("image4");
	imshow("image4", image4);
	namedWindow("image5");
	imshow("image5", image5);
	waitKey(0);*/
	/*Mat data;
	int i, j, k, l, m;
	i = 0;
	j = i + 1;
	k = j + 1;
	l = k + 1;
	m = l + 1;
	int b1, b2, b3, b4, b5;
	b1 = i;
	b2 = j;
	b3 = k;
	b4 = l;
	b5 = m;
	image1 = openfile(imgPaths.at(i).c_str());
	image2 = openfile(imgPaths.at(j).c_str());
	image3 = openfile(imgPaths.at(k).c_str());
	image4 = openfile(imgPaths.at(l).c_str());
	image5 = openfile(imgPaths.at(m).c_str());
	double rate = 0;
	double rate2 = 0;
	double rate3 = 0;
	double rate4 = 0;
	double orate = 0;
	double orate2 = 0;
	double orate3 = 0;
	double orate4 = 0;
	int lyer = 5;
	while (rate<0.9)
	{
		data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4; data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4;
		rate = ly1(data, solve);
		//cout << "ly"<< lyer <<"--"<<rate*100 << "%" << endl;;
		if (rate-orate>0&& rate2 - orate2>0&& rate3 - orate3>0&& rate4 - orate4>0) {
			orate = rate;
			orate2 = rate2;
			orate3 = rate3;
			orate4 = rate4;
			if (lyer == 5) {
				b5 = m;
			}
			if (lyer == 4) {
				b4 = l;
			}
			if (lyer == 3) {
				b3 = k;
			}
			if (lyer == 2) {
				b2 = j;
			}
			if (lyer == 1) {
				b1 = i;
			}
			cout << "p--image:" << b1 << "--" << b2 << "--" << b3 << "--" << b4 << "--" << b5 << "--" << (orate+ orate2+ orate3+ orate4) * 100 << "%" << endl;
		}
		else {
			if (lyer == 5) {
				m++;
				if (m >= count) { m = b5; lyer--; }
			}
			if (lyer == 4) {
				l++;
				if (l >= count) { l = b4; lyer--;  }
			}
			if (lyer == 3) {
				k++;
				if (k >= count) { k = b3; lyer--;  }
			}
			if (lyer == 2) {
				j++;
				if (j >= count) { j = b2; lyer--;  }
			}
			if (lyer == 1) {
				i++;
				if (i >= count) { i = b1; lyer++;  }
			}
			image1 = openfile(imgPaths.at(i).c_str());
			image2 = openfile(imgPaths.at(j).c_str());
			image3 = openfile(imgPaths.at(k).c_str());
			image4 = openfile(imgPaths.at(l).c_str());
			image4 = openfile(imgPaths.at(m).c_str());
			//cout << "image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << endl;
		}
		//cout << "b--image:" << b1 << "--" << b2 << "--" << b3 << "--" << b4 << "--" << b5 << "--" << orate * 100 << "%" << endl;
		//cout << "image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << endl;
	}
	cout << "b--image:" << b1 << "--" << b2 << "--" << b3 << "--" << b4 << "--" << b5 << "--" << orate << endl;
	/*for (int i = 0; i < count - 4; i++) {
		image1 = openfile(imgPaths.at(i).c_str());
		if (image1.empty()) {
			cout << "file no found" << endl;
			break;
		}
		colorReduce(image1);
		for (int j = i+1 ; j < count - 3; j++) {
			image2 = openfile(imgPaths.at(j).c_str());
			if (image2.empty()) {
				cout << "file no found" << endl;
				break;
			}
			colorReduce(image2);
			for (int k = j+1; j < count - 2; k++) {
				image3 = openfile(imgPaths.at(k).c_str());
				if (image3.empty()) {
					cout << "file no found" << endl;
					break;
				}
				colorReduce(image3);
				for (int l = k+1; l < count; l++) {
					image4 = openfile(imgPaths.at(l).c_str());
					if (image4.empty()) {
						cout << "file no found" << endl;
						break;
					}
					colorReduce(image4);
					for (int m = l+1; m < count; m++) {
						image5 = openfile(imgPaths.at(m).c_str());
						if (image5.empty()) {
							cout << "file no found" << endl;
							break;
						}
						colorReduce(image5);
						if (average(image1, image2, image3, image4, image5,solve)) {
							cout << "success--image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << endl;
						}
						else {
							//cout << "fail-image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << endl;
							cout << "fail"<< m << endl;
						}
					}
				}
			}
		}
	}*/
	waitKey(0);
	return 0;
}