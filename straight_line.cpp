/*
最小二乘法拟合直线~
这里若用作光电效应根据频率和截止电压求普朗克常数，截止电压为正数。
*/
#include <iostream>
using namespace std;
#include <fstream>
#include <stdlib.h>
#include <cmath>
// #include <sting>

#define Y_E 1.60217662e-19          //现代精确测量的元电荷数值。库伦
#define ZETA 1e14       //光频率单位修正
#define H 6.62607015e-34        //现代科学测算普朗克常量
#define SIZE 100

int main()
{
    system("chcp 65001");
    cout << "最小二乘法进行直线(y = k * x + b)拟合" << endl;
    cout << "数据文件data.txt每行一对x y。中间以空格分开。可以存放在用户桌面，或相对路径同一文件夹下" << endl;
    cout << "如果是普朗克常数的计算，要求数据光频率的数量值是10的14次方Hz。截止电压是正数且单位为V(伏)" << endl;
    cout << "确认没有问题后按任意键继续" << endl;
    system("pause>nul");
    string path = "data.txt";
    ifstream fin (path);

    if (!fin)
    {
        string s1 = getenv("homePath");
        string s2 = "/Desktop/";
        path = (s1 + s2 + path).c_str();
        cout << "正在桌面路径\"" << path << "\"搜索数据文件" << endl;
        fin.clear();
        fin.open(path);
    }

    if (fin)
    {
        double x, y;
        int cnt = 0;
        while(fin >> x >> y) cnt++;
        double *num_x = new double[cnt];
        double *num_y = new double[cnt];

        fin.close();
        fin.clear();
        fin.open(path);
        double avg_x = 0;
        double avg_y = 0;
        double xy = 0;
        double sum_sq_x = 0;
        double sum_sq_y = 0;
        for (int i = 0; i < cnt; i++)
        {
            fin >> num_x[i] >> num_y[i];
            avg_x += num_x[i];
            avg_y += num_y[i];
            xy += num_x[i] * num_y[i];
            sum_sq_x += pow(num_x[i], 2);
            sum_sq_y += pow(num_y[i], 2);
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
            cout << endl << "符合数据要求进行普朗克常数计算并误差分析请键入小写字符h，键入其他字符将退出" << endl;
            char c;
            cin >> c;
            if (c == 'h')
            {
                double h = Y_E * k / ZETA;
                cout << endl << "实验测定的普朗克常数：" << h << endl;
                cout << "误差：" << abs(h - H) / H * 100 << '%' << endl;
                delete [] num_x;
                delete [] num_y;
            }
            else
            {
                delete [] num_x;
                delete [] num_y;
                return 0;
            }
        }
        else
            cout << endl << "相关系数绝对值小于0.75，不近似于直线" << endl;
    }
    else
        cout << "数据data.txt打开失败" << endl;
    
    cout << "任意按键以退出" << endl;
    system("pause>nul");
    return 0;
}