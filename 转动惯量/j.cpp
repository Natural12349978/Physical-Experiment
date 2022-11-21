#include <iostream>
using namespace std;
#include <fstream>
#include <stdlib.h>
#define _USE_MATH_DEFINES		//引入cmath中的M_PI
#include <cmath>
#include <iomanip>

#define N 5			//这个是指5组转动实验来测定扭转常数的
#define CNT_T 10	//测周期时是几倍周期
#define N_T 3		//测扭转常数的时候每组周期测几次
#define ETA 4		//未知原因的修正？？实验和理论误差稳定高达75％

void out_line_exp(double *x, double *y, int cnt, double &k);
void out_line_exp(ofstream &fout, double *x, double *y, int cnt, double &k);
double calc_j(ifstream &fin, double k, double &t1);		//k是扭转常数

int main()
{
	system("chcp 65001");

    string path = "data.txt";
    ifstream fin (path);

	string s1 = getenv("homePath");
	string s2 = "/Desktop/";

    if (!fin)
    {
        path = (s1 + s2 + path).c_str();
        cout << "正在桌面路径\"" << path << "\"搜索数据文件" << endl;
        fin.clear();
        fin.open(path);
    }

	//这些特例将不进行测试。
	string path_out = "outdata.txt";
	path_out = (s1 + s2 + path_out).c_str();
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

	if (fin && fout)
	{
		double m1;			//标定扭转常数K_N实验时一物块质量
		double m2;			//另一物块质量
		double m_zhu;		//圆柱的质量
		double r_zhu;		//圆柱的半径		几何计算转动惯量
		double m_tong;		//圆筒的质量
		double r1_tong;		//圆筒的外径
		double r2_tong;		//圆筒的内径。		几何计算转动惯量
		fin >> m1 >> m2 >> m_zhu >> r_zhu >> m_tong >> r1_tong >> r2_tong;

		double num_x[N];
		double num_y[N];
		for (int i = 0; i < N; i++)
		{
			int x;
			double t1, t2, t3;
			fin >> x >> t1 >> t2 >> t3;
			num_y[i] = pow(x, 2);
			t1 = (t1 + t2 + t3) / N_T / CNT_T;
			fout << "X=" << setw(3) << x << " cm时平均周期为\t" << t1 << "秒" << endl;
			num_x[i] = pow(t1, 2);
		}
		double k;		//直线斜率
		out_line_exp(fout, num_x, num_y, N, k);
		k = k * 4 * pow(M_PI, 2) * (m1 + m2);		//k重载为本次实验测定的扭转常数

		double avg_t;
		double j = calc_j(fin, k, avg_t);
		fout << endl << "平均周期：" << avg_t << endl << "圆盘的转动惯量为：" << j << endl;

		double j_total = calc_j(fin, k, avg_t);
		double c_j = m_zhu * pow(r_zhu, 2) / 2;
		fout << endl << "平均周期：" << avg_t << endl << "实验所得圆柱的转动惯量为：" << j_total - j << endl;
		fout << "计算所得圆柱的转动惯量为：" << c_j << endl;
		fout << "百分误差：" << abs((j_total - j) - c_j) / c_j * 100 << endl;

		j_total = calc_j(fin, k, avg_t);
		c_j = m_tong * (pow(r1_tong, 2) + pow(r2_tong, 2)) / 2;
		fout << endl << "平均周期：" << avg_t << endl << "实验所得圆筒的转动惯量为：" << j_total - j << endl;
		fout << "计算所得圆筒的转动惯量为：" << c_j << endl;
		fout << "百分误差：" << abs((j_total - j) - c_j) / c_j * 100 << endl;

		j_total = calc_j(fin, k,avg_t);
		fout << endl << "平均周期：" << avg_t << endl << "实验所得弹体的转动惯量为：" << j_total - j << endl;
	}
	else
		cout << "wrong" << endl;

	fin.close();
	fout.close();
	system("pause");
	return 0;
}

void out_line_exp(double *x, double *y, int cnt, double &k)
{
	double avg_x = 0;
	double avg_y = 0;
	double xy = 0;
	double sum_sq_x = 0;
	double sum_sq_y = 0;
	for (int i = 0; i < cnt; i++)
	{
		avg_x += x[i];
		avg_y += y[i];
		xy += x[i] * y[i];
		sum_sq_x += pow(x[i], 2);
		sum_sq_y += pow(y[i], 2);
	}
	avg_x /= cnt;
	avg_y /= cnt;
	double r = (xy - cnt * avg_x * avg_y) / sqrt((sum_sq_x - cnt * pow(avg_x, 2)) * (sum_sq_y - cnt * pow(avg_y, 2)));
	if (abs(r) >= 0.75)
	{
		double k = (xy - cnt * avg_x * avg_y) / (sum_sq_x - cnt * pow(avg_x, 2));
		double b = avg_y - k * avg_x;
		cout << endl << "根据最小二乘法拟合出的直线为：" << endl;
		cout << "y = " << k << " * x + " << b << endl;
		cout << "相关系数r：" << r << endl;
	}

}

void out_line_exp(ofstream &fout, double *x, double *y, int cnt, double &k)
{
	double avg_x = 0;
	double avg_y = 0;
	double xy = 0;
	double sum_sq_x = 0;
	double sum_sq_y = 0;
	for (int i = 0; i < cnt; i++)
	{
		avg_x += x[i];
		avg_y += y[i];
		xy += x[i] * y[i];
		sum_sq_x += pow(x[i], 2);
		sum_sq_y += pow(y[i], 2);
	}
	avg_x /= cnt;
	avg_y /= cnt;
	double r = (xy - cnt * avg_x * avg_y) / sqrt((sum_sq_x - cnt * pow(avg_x, 2)) * (sum_sq_y - cnt * pow(avg_y, 2)));
	if (abs(r) >= 0.75)
	{
		k = (xy - cnt * avg_x * avg_y) / (sum_sq_x - cnt * pow(avg_x, 2));
		double b = avg_y - k * avg_x;
		fout << endl << "根据最小二乘法拟合出的直线为：" << endl;
		fout << "y = " << k << " * x + " << b << endl;
		fout << "相关系数r：" << r << endl;
	}

}

double calc_j(ifstream &fin, double k, double &t1)		//k是扭转常数
{
	double t2, t3;
	fin >> t1 >> t2 >> t3;
	t1 = (t1 + t2 + t3) / N_T / CNT_T;		//平均周期
	// return k * pow(t1 / M_PI, 2);
	return k * pow(t1 / 2 / M_PI, 2) * ETA;
}

/*
m1(g) m2(g) 柱(g) 柱(cm)
筒的质量(g) 筒外(cm) 筒内(cm) 
[五组扭摆半径(cm) 10T(s)] × 3
[圆盘 10T(s)]× 3
[圆盘+圆柱 10T(s)]× 3
[圆盘+圆筒 10T(s)]× 3
[圆盘+弹体 10T(s)]× 3

238.3 237.7 358.6 9.968
623.8 9.978 9.394
5 19.920 19.914 19.914
10 25.844 25.791 25.792
15 33.151 33.164 33.166
20 41.495 41.424 41.358
25 50.215 50.173 50.101
6.009 6.009 6.008
8.283 8.291 8.287
11.894 11.964 11.954
12.308 12.319 12.315
*/