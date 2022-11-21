#include <iostream>
using namespace std;
#include <fstream>
#include <stdlib.h>
#include <cmath>

#define G 9.792				//长沙地区重力加速度
#define DELTA_S 0.5e-3		//Δs
#define DELTA_T 0.1e-3		//Δt
#define ETA 1e-2 			//进制转换，长度由厘米转换成米，最后求得米每二次方秒单位的加速度
#define N 3					//3个时间求平均

bool f(ifstream &fin, ofstream &fout, double &s, double &t);
double delta_g(double v0, double t1, double t2, double g);

int main()
{	//输入输出文件打开##################
	system("chcp 65001");

    string path = "data.txt";
    ifstream fin (path);

	string string1 = getenv("homePath");
	string string2 = "/Desktop/";

    if (!fin)
    {
        path = (string1 + string2 + path).c_str();
        cout << "正在桌面路径\"" << path << "\"搜索数据文件" << endl;
        fin.clear();
        fin.open(path);
    }

	//这些特例将不进行测试。
	string path_out = "outdata.txt";
	path_out = (string1 + string2 + path_out).c_str();
	cout << "将结果导出到" << path_out << endl;
	ofstream fout (path_out);
	if (!fout)
	{
		cout << "输出到桌面失败，将输出到相对路径目录下" << endl;
		fout.clear();
		fout.open("outdata.txt");
		if (!fout)
			cout << "wrong" << endl;
	}
	//输入输出文件打开完毕############

	if (fin && fout)
	{
		double v0;
		fin >> v0;		//实际输入s0
		v0 = sqrt(2 * v0 * ETA * G);		//用来算Δg的
		double s2, t1, t2, t3;		//只有s2、t1和t2真正有用~
		fin >> s2 >> t1 >> t2 >> t3;
		t2 = (t1 + t2 + t3) / N;
		s2 = s2 * ETA;

		double s1, g;
		while (f(fin, fout, s1, t1))			//这里t1被重载
		{
			g = 2 * (s2 / t2 - s1 / t1) / (t2 - t1);
			fout << "计算所得g为：\t" << g << endl;
			fout << "百分误差：\t" << abs(g - G) / G * 100 << endl;
			fout << "Δg = " << delta_g(v0, t1, t2, g) << endl << endl;
		}

		fin.close();
		fout.close();
	}
	else
		cout << "wrong" << endl;

	system("pause");
	return 0;
}

bool f(ifstream &fin, ofstream &fout, double &s, double &t)
{
	double tmp_t1, tmp_t2;
	if (fin >> s >> t >> tmp_t1 >> tmp_t2)
	{
		t = (t + tmp_t1 + tmp_t2) / N;
		fout << s << "cm时的平均周期为：\t" << t << endl;
		s = s * ETA;
		return true;
	}
	else
		return false;
}

double delta_g(double v0, double t1, double t2, double g)
{
	double tmp = (1 / t1 + 1 / t2);
	tmp = 2 * tmp * (DELTA_S + v0 * DELTA_T) / (t2 - t1);
	return tmp + 4 * g * DELTA_T / (t2 - t1);
}

/*
g0不用输入。长沙地区g0取9.792m/s²
s0[OA](cm)
s2[AC](cm) {t2(s)} × 3
{ s1[AB](cm) {t1(s)} × 3 } × n（组数不限，正整数）

12.5
73.51 0.26141 0.26002 0.26197
11.10 0.0617 0.06202 0.06206
17.90 0.09055 0.09137 0.09032
21.82 0.103930.10502 0.10507
26.35 0.12303 0.12373 0.12342
31.89 0.14373 0.14327 0.14339
36.66 0.15812 0.15742 0.15837
41.18 0.17326 0.17329 0.17351
47.42 0.19147 0.19245 0.19255
55.42 0.21459 0.21397 0.21428
62.28 0.23009 0.23241 0.23027
70.72 0.25391 0.25265 0.25251
*/