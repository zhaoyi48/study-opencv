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
#include <omp.h>

using namespace cv;
using namespace std;

int R[251][251], G[251][251], B[251][251];
int SR[251][251], SG[251][251], SB[251][251];

Mat openfile(string path) {
	Mat data;
	data = imread(path);
	return data;
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
			filepath = filepath + CW2A(dirData.cFileName) + "\\*";
			//cout << filepath << endl;
			wstring temp = wstring(filepath.begin(), filepath.end());
			WIN32_FIND_DATA fileData;
			HANDLE fileFind = FindFirstFile(temp.c_str(), &fileData);
			if (fileFind != INVALID_HANDLE_VALUE) {
				FindNextFile(fileFind, &fileData);
				FindNextFile(fileFind, &fileData);
				do {
					path = path + CW2A(dirData.cFileName) + "/" + CW2A(fileData.cFileName);
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
void BGR(Mat data) {
	//int rate = 0;
	int nl = data.rows;
	int nc = data.cols;
//#pragma omp parallel for 
	for (int i = 0; i < nl; i++) {
		uchar* pixel = data.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			B[i][j] = (int)pixel[j * 3];
			G[i][j] = (int)pixel[j * 3 + 1];
			R[i][j] = (int)pixel[j * 3 + 2];
		}
	}
	//cout << "test:" << B[25]<< endl;
}

void clear() {
//#pragma omp parallel for 
	for (int i = 0; i < 251; i++) {
		for (int j = 0; j < 251; j++) {
			R[i][j] = 0;
			G[i][j] = 0;
			B[i][j] = 0;
		}
	}
}

int tsa(Mat data,int lyer) {
	clear();
	BGR(data);
	int rate = 0;
	int nl = data.rows;
	int nc = data.cols;
	int Rc = 0;
//#pragma omp parallel for 
	for (int i = 0; i < nl; i++) {
		for (int k = 0; k < 5; k++) {
			if (abs((int)B[50 * k] - (int)SB[50 * k] )<(255/(double)(6-lyer))) {
				Rc++;
			}
			if (abs((int)G[50 * k] - (int)SG[50 * k])<(255 / (double)(6 - lyer))) {
				Rc++;
			}
			if (abs((int)R[50 * k] - (int)SR[50 * k])<(255 / (double)(6 - lyer))) {
				Rc++;
			}
		}
	}
	//cout << "v:" << Rc << "--" << lyer << endl;
	return Rc;
}


void SBGR(Mat data) {
	int rate = 0;
	int nl = data.rows;
	int nc = data.cols;
	cout << "test:" << nl << endl;
	for (int i = 0; i < nl; i++) {
		uchar* pixel = data.ptr<uchar>(i);
		for (int j = 0; j < nc; j++)
		{
			SB[i][j] = (int)pixel[j * 3];
			SG[i][j] = (int)pixel[j * 3 + 1];
			SR[i][j] = (int)pixel[j * 3 + 2];

		}
	}
}

/*vector<int> black(vector<int> blacklist,int image) {
	for (int i = 0; i < 251; i++) {
		for (int j = 0; j < 251; j++) {
			if (B[i][j] > SB[i][j] * 0.2 || G[i][j] > SG[i][j] * 0.2 || R[i] > SR[i][j] * 0.2) {
				blacklist.push_back(image);
				cout << "black--list:" << image << endl;
				break;
			}
		}
	}
	return blacklist;
}*/

int verify(Mat data,int lyer) {
	int Cr = 0;
	//BGR(data);
	Cr = tsa(data, lyer);
	int nl = data.rows;
	cout << "v:" << Cr << "--" << lyer << endl;

	if (Cr / (nl * 5.0 * 3) >(1.0/(6-lyer))) {
		return 1;
	}
	return 0;
}

int findblack(vector<int> blacklist,int v) {
	int count = blacklist.size();
	for (int i = 0; i < count; i++) {
		if (v == blacklist.at(i)) {
			return 0;
		}
	}
	return 0;
}

int main() {
	vector<string> imgPaths;
	vector<int> list;
	Mat image1, image2, image3, image4, image5, solve, t1;



	for (int i = 0; i < 250; i++) {
		for (int j = 0; j < 250; j++) {
			R[i][j] = 0;
			SR[i][j] = 0;
			G[i][j] = 0;
			SG[i][j] = 0;
			B[i][j] = 0;
			SB[i][j] = 0;
		}
	}
	solve = imread("0.jpg");


	if (solve.empty()) {
		cout << "file no found" << endl;
	}





	SBGR(solve);

	//namedWindow("solve");
	//imshow("solve", solve);

	imgPaths = test();

	cout << imgPaths.size() << endl;

	int count = imgPaths.size();

	Mat data;

	/*namedWindow("solve");
	imshow("solve", solve);
	cout << imgPaths.size() << endl;
	cout << imgPaths.at(0).c_str() << endl;
	cout << imgPaths.at(2).c_str() << endl;
	cout << imgPaths.at(597).c_str() << endl;
	cout << imgPaths.at(10363).c_str() << endl;
	cout << imgPaths.at(13011).c_str() << endl;
	image1 = openfile(imgPaths.at(0).c_str());
	image2 = openfile(imgPaths.at(2).c_str());
	image3 = openfile(imgPaths.at(597).c_str());
	image4 = openfile(imgPaths.at(10363).c_str());
	image5 = openfile(imgPaths.at(13011).c_str());
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


	/*for (int i = 0; i < count; i++) {
		t1 = openfile(imgPaths.at(i).c_str());
		clear();
		BGR(0.2*t1);
		blacklist = black(blacklist, i);
	}*/
	clear();

	int lyer = 0;
	for (int i = 0; i < count - 4; i++)
	{
		lyer = 1;
		image1 = openfile(imgPaths.at(i).c_str());
		data = 0.2*image1;
		cout << "t--image:" << i << endl;
		if (verify(data, lyer))
		{
			namedWindow("test");
			imshow("test", data);
			waitKey(100);
			//cout << "t--image:" << i << endl;
			for (int j = i + 1; j < count - 3; j++)
			{
				lyer = 2;
				image2 = openfile(imgPaths.at(j).c_str());
				data += 0.2*image2;
				cout << "t--image:" << i << "--" << j  << endl;
				if (verify(data, lyer))
				{
					namedWindow("test");
					imshow("test", data);
					waitKey(100);
					for (int k = j + 1; k < count - 2; k++)
					{
						lyer = 3;
						image3 = openfile(imgPaths.at(k).c_str());
						data += 0.2*image3;
						cout << "t--image:" << i << "--" << j << "--" << k  << endl;
						if (verify(data, lyer))
						{
							namedWindow("test");
							imshow("test", data);
							waitKey(100);
							for (int l = k + 1; l < count - 1; l++) 
							{
								lyer = 4;
								image4 = openfile(imgPaths.at(l).c_str());
								data += 0.2*image4;
								cout << "t--image:" << i << "--" << j << "--" << k << "--" << l << endl;
								if (verify(data, lyer))
								{
									namedWindow("test");
									imshow("test", data);
									waitKey(100);
									for (int m = l + 1; m < count; m++) 
									{
										lyer = 4;
										image5 = openfile(imgPaths.at(m).c_str());
										data += 0.2*image5;
										int Cr = 0;
										//BGR(data);
										Cr = tsa(data, 5);
										int nl = data.rows;
										if ((Cr/ (nl * 5.0*3)) >0.9)
										{
											namedWindow("test");
											imshow("test", data);
											waitKey(100);
											cout << "p--image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << "--" << Cr / (nl * 5.0*3) * 100 << "%" << endl;
											list.push_back(i);
											list.push_back(j);
											list.push_back(k);
											list.push_back(l);
											list.push_back(m);
										}
										cout << "image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << "--" << Cr / (nl * 5.0*3) * 100 << "%" << endl;
										data -= 0.2*image5;
									}
								}
								data -= 0.2*image4;
							}
						}
						data -= 0.2*image3;
					}
				}
				data -= 0.2*image2;
			}
		}
	}

	for (int i = 0; i < list.size(); i++) {
		cout << "image:" << i <<  endl;
	}



	/*while (lyer>1){
		data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4;
		rate = ly(data, solve);
		if (rate - orate > 0) {
			orate = rate;
			b1 = i;
			b2 = j;
			b3 = k;
			b4 = l;
			b5 = m;
			cout << "p--image:" << i << "--" << j << "--" << k << "--" << l << "--" << m << "--" << orate * 100 << "%" << endl;
		}
		else {
			if (lyer == 5) {
				m++;
				if (m >= count) { m = b5; lyer--; }
			}
			if (lyer == 4) {
				l++;
				if (l >= count) { l = b4; lyer--; }
			}
			if (lyer == 3) {
				k++;
				if (k >= count) { k = b3; lyer--; }
			}
			if (lyer == 2) {
				j++;
				if (j >= count) { j = b2; lyer--; }
			}
			if (lyer == 1) {
				i++;
				if (i >= count) { i = b1; lyer--; }
			}
		}
		image1 = openfile(imgPaths.at(i).c_str());
		image2 = openfile(imgPaths.at(j).c_str());
		image3 = openfile(imgPaths.at(k).c_str());
		image4 = openfile(imgPaths.at(l).c_str());
		image5 = openfile(imgPaths.at(m).c_str());
		//cout << "select:" << i << "--" << j << "--" << k << "--" << l << "--" << m << "--" << rate * 100 << "%" << endl;
	}*/

	//11506--13201--453--3--7252
	/*i = b1 = 11506;
	j = b2 = 13201;
	k = b3 = 453;
	l = b4 = 4;
	m = b5 = 7252;
	image1 = openfile(imgPaths.at(i).c_str());
	image2 = openfile(imgPaths.at(j).c_str());
	image3 = openfile(imgPaths.at(k).c_str());
	image4 = openfile(imgPaths.at(l).c_str());
	image5 = openfile(imgPaths.at(m).c_str());
	while (true)
	{
		data = 0.2*image1 + 0.2*image2 + 0.2*image3 + 0.2*image5 + 0.2*image4;
		rate = ly(data, solve);
		if (rate - orate > 0) {
			orate = rate;
			b1 = i;
			b2 = j;
			b3 = k;
			b4 = l;
			b5 = m;
			cout << "update:" << i << "--" << j << "--" << k << "--" << l << "--" << m << "--" << orate * 100 << "%" << endl;
		}
		lyer = rand() % 5 + 3;
		//lyer = 3;
		switch (lyer)
		{
		case 1:i++; if (i >= count) { i = 0; cout << "end1" << endl; }break;
		case 2:j++; if (j >= count) { j = 1; cout << "end2" << endl;}break;
		case 3:k++; if (k >= count) { k = 2; cout << "end3" << endl;}break;
		case 4:l++; if (l >= count) { l = 3; cout << "end4" << endl;}break;
		case 5:m++; if (m >= count) { m = 4; cout << "end5" << endl;}break;
		}
		/*lyer = rand() % 4 + 3;
		switch (lyer)
		{
		case 1: i = b1; break;
		case 2: j = b2; break;
		case 3: k = b3; break;
		case 4: l = b4; break;
		case 5: m = b5; break;
		}
		image1 = openfile(imgPaths.at(i).c_str());
		image2 = openfile(imgPaths.at(j).c_str());
		image3 = openfile(imgPaths.at(k).c_str());
		image4 = openfile(imgPaths.at(l).c_str());
		image5 = openfile(imgPaths.at(m).c_str());
	}*/
	return 0;
}