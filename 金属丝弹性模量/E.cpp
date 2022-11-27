#include <iostream>
#include<iomanip>
using namespace std;
#include <fstream>
#include <stdlib.h>
#define _USE_MATH_DEFINES		//引入cmath中的M_PI
#include <cmath>

#define G 9.80                  //重力加速度
#define ETA 1e4                 //金属丝直径d是以cm为单位。最后杨氏模量单位N每平方米的修正

float f(double x, double &y, int &i);       //要求y小于x，i要为0

int main()
{	//输入输出文件打开##################
	system("chcp 65001");

    double delta_m;             //单个法码的重量
    int n;
    cout << "单次增加的砝码重量(kg):";
    cin >> delta_m;
    cout << "数据的组数(整型):";
    cin >> n;

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
        double short_l, long_l, len_l;
        double d1, d2, d;
        fin >> short_l >> long_l >> len_l;
        fin >> d1 >> d2 >> d;
        d = (d + d1 + d2) / 3;

        double *num = new double[n + 1];
        num[0] = 0;
        for (int i = 1; i <= n; i++)
            fin >> num[i];
        for (int i = n - 1; i >= 0; i--)
        {
            double tmp;
            fin >> tmp;
            num[i] = (num[i] + tmp) / 2; 
        }

        for (int i = 0; i <= n; i++)
            fout << "荷重砝码质量" << i * delta_m << "kg时刻度n的平均值(mm):" << num[i] << endl;

        double delta_f = delta_m;       //现在是单个砝码的重量
        delta_m = n * delta_m / 2;      //重载为逐差法中的中间间距
        fout << endl << "荷重砝码质量差" << delta_m << "kg时的读数差:" << endl;
        double avg_dn = 0;
        for(int i = 1; i <= n / 2; i++)
        {
            avg_dn += (num[i + n / 2] - num[i]);
            fout << 'n' << i + n / 2 << " - n" << i << " = " << (num[i + n / 2] - num[i]) / 10 << "\tcm" << endl;
        }

        // Δn的相关计算#############
        avg_dn = avg_dn / 10 / (n / 2);
        fout.setf(ios::fixed);
        fout << "Δn平均值 = " << setprecision(3) << avg_dn << endl;

        fout << endl << "Δn的绝对误差:" << endl;
        double sum_tmp = 0;
        for(int i = 1; i <= n / 2; i++)
        {
            double tmp = num[i + n / 2] - num[i];
            tmp = abs(tmp / 10 - avg_dn);
            fout << "Δ(Δn)" << i << " = " << tmp << "\tcm" << endl;
            sum_tmp += tmp;
        }
        double nnn = sum_tmp * 2 / n;
        fout << "Δ(Δn) = " << nnn << "\tcm" << endl;
        //一阶段完成################

        double e = 8 * len_l * long_l * delta_m * G * ETA;       //弹性模量
        e /= (M_PI * pow(d / 10, 2) * short_l * avg_dn);
        fout.unsetf(ios::fixed);
        fout << endl << "弹性模量E(N·m²):\t" << setprecision(6) << e << endl;

        // nnn = 0.05;      //此输出示例Δ(Δn)被我从0.044手动修改成0.05，最终才与书上结果更近似。  

        double max_delta_e = pow(0.05 / len_l, 2) + pow(0.05 / long_l, 2) + pow(0.004 / short_l, 2) + pow(2 * 0.005 / d, 2) + pow(0.005 / delta_f, 2) + pow(nnn / avg_dn, 2);
        max_delta_e = sqrt(max_delta_e);
        fout << "E的最大相对误差为:\t" << max_delta_e * 100 << "%" << endl;
        double tmptmp = ceil(max_delta_e * 1000) / 10.0;
        fout << "向上近似为:\t" << tmptmp << "%" << endl;
        tmptmp = e * tmptmp / 100;
        fout << "E的最大绝对误差为:\t" << tmptmp << endl;
        int dec_bit = 0;
        float tmp = f(e, tmptmp, dec_bit);
        if (!tmp)
            fout << "wrong" << endl;
        fout << "E的测量结果为E = (" << tmp << " +- " << tmptmp << ") e+" << dec_bit << " N·m(-2)" << endl;

		fin.close();
		fout.close();
	}
	else
		cout << "wrong" << endl;

	system("pause");
	return 0;
}

float f(double x, double &y, int &i)       //要求y小于x，i要为0
{
    if (x <= 0 || y <= 0)
        return 0;
    while(!(x / pow(10, i) < 10 && x / pow(10, i) >= 1)) i++;
    y = y / pow(10, i);
    int j = 0;
    while(!(y * pow(10, j) < 10 && y * pow(10, j) >= 1)) j++;
    y = ceil(y * pow(10, j)) / pow(10, j);
    return round(x / pow(10, i) * 100) / 100;
}
/*
光杠杆短臂长(cm) 长臂长(cm) 金属丝长度(cm)
金属丝上部直径(mm) 中部直径(mm) 下部直径(mm)
7.246 126.80 80.70
0.503 0.497 0.500
1.0 8.5 16.0 23.0 30.0 37.0
30.5 23.5 16.5 9.0 1.5 0.0
*/