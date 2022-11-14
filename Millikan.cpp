#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <cmath>
#include<iomanip>
#include <fstream>

#define RHO 981         //油密度。千克每立方米
#define G 9.7915        //引力常数。米每二次方秒
#define L 1.5e-3        //实验装置运动6格的长度。米
#define ETA 1.83e-5     //空气黏滞系数（23摄氏度）。千克每米秒
#define P 76.0          //大气压强。厘米汞柱
#define B 6.17e-6       //空气非连续流体修正系数。米·厘米汞柱
#define D 5e-3          //极板间距离。毫米
#define Y_E 1.60217662e-19          //现代精确测量的元电荷数值。库伦

#define N 3             //单个油滴的试验次数
void f(double t, int u, double &q, int &n, float &x);       //t是下落时间，u是极板电压，q是计算所得油滴总电荷量，n是计算所得元电荷个数，x是计算所得误差分析百分比

int main()
{
    double t;       //下落时间
    int u;          //极板电压
    ifstream fin ("data.txt");
    ofstream fout ("outdata.txt");
    if (fin && fout)
    {
        int cnt = 0;
        float avg_u = 0;
        float avg_t = 0;
        while(fin >> u >> t)
        {
            double q;
            int n;
            float x;
            f(t, u, q, n, x);
            if (cnt == 0)
                fout << "带电量:\t" << "电荷数:\t" << "误差分析:\t" << endl; 
            fout << setprecision(5) << q << "\t";
            fout << n << "\t\t";
            fout.setf(ios::fixed);
            fout << setprecision(2) << x << endl;
            fout.unsetf(ios::fixed);

            avg_u += u;
            avg_t += t;
            if (cnt++ == 2)
            {
                avg_t /= cnt;
                avg_u /= cnt;
                f(avg_t, avg_u, q, n, x);
                fout.setf(ios::fixed);
                fout << "-----------------------------" << endl;
                fout << "平均平衡电压：\t" << avg_u << endl;
                fout << "平均下落时间：\t" << avg_t << endl;
                fout.unsetf(ios::fixed);
                fout << "计算得电量：\t" << setprecision(5) << q << endl;
                fout << "计算得电子数：\t" << n << endl;
                fout << "单位电荷百分误差：\t" << setprecision(2) << x << endl;
                fout << endl << endl;
                //还原数据
                avg_t = 0;
                avg_u = 0;
                cnt = 0;
            }
        }
        fin.close();
        fout.close();
    }
    else
        cout << "wrong" << endl;

    // cout << q << "\t" << n << "\t";
    // cout << fixed << setprecision(2) << x << endl;
    return 0;
}
void f(double t, int u, double &q, int &n, float &x)
{
    double a = sqrt(9 * ETA * L / (2 * RHO * G * t));
    double b = sqrt(pow(L / t * ETA / (1 + B / (P * a)), 3) / (2 * RHO * G));
    q = 18 * M_PI * b * D / u;
    n = round(q / Y_E);         //n是电子数
    double e = q / n;               //e是实验所求元电荷量
    x = (abs(e - Y_E) / Y_E) * 100;       //x是误差分析
}
