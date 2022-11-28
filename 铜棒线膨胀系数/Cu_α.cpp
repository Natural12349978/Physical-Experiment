#include <iostream>
using namespace std;
#include <fstream>
#include <stdlib.h>
#include <cmath>

void out_line_exp(ofstream &fout, double *x, double *y, int cnt, double &k, double &b);

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
        double x, y1, y2;
        int cnt = 0;
        while(fin >> x >> y1 >> y2) cnt++;
        double *num_x = new double[cnt];
        double *num_y = new double[cnt];

        fin.close();
        fin.clear();
        fin.open(path);
        for (int i = 0; i < cnt; i++)
        {
            double tmp;
            fin >> num_x[i] >> num_y[i] >> tmp;
            num_y[i] = (num_y[i] + tmp) / 2;
        }
        double k, b;
        out_line_exp(fout, num_x, num_y, cnt, k, b);

        double len_l, short_l, long_l;
        int t0;
        cout << "请输入测量时铜棒的长度(mm):";
        cin >> len_l;
        cout << "请输入测量铜棒长度时的室温(整数,℃):";
        cin >> t0;
        cout << "请输入光杠杆短臂长(cm):";
        cin >> short_l;
        cout << "请输入光杠杆长臂长(cm):";
        cin >> long_l;
        double l0 = k * t0 * short_l / 2 / long_l;      //l0是原长
        l0 = len_l - l0;
        fout << endl << "铜棒0℃时的原长为:" << l0 << "\tmm" << endl;
        fout << "线膨胀系数:" << k * short_l / 2 / long_l / l0 << endl;

		fin.close();
		fout.close();
	}
	else
		cout << "wrong" << endl;

    cout << "完成，请按任意键退出" << endl;
	system("pause>nul");
	return 0;
}

void out_line_exp(ofstream &fout, double *x, double *y, int cnt, double &k, double &b)
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
		b = avg_y - k * avg_x;
		fout << "根据最小二乘法拟合出的直线为：" << endl;
		fout << "y = " << k << " * x + " << b << endl;
		fout << "相关系数r：" << r << endl;
	}

}

/*
{铜棒温度(℃) 升温时刻度(mm) 降温时刻度()} × n(整数)
30 3.8 0.9
35 4.4 2.0
40 5.2 2.9
45 6.1 3.7
50 6.9 4.1
55 7.5 5.1
60 8.1 7.1
65 8.9 6.9
70 9.3 7.5
75 10.0 8.2
80 10.6 9.0
85 11.3 10.0
90 12.0 11.0
95 12.8 12.3
100 13.2 13.2
*/