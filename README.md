# 自用物理实验数据分析的小脚本 #
**我是<mark>夏离</mark>**
<!-- 我永远喜欢牛杂师傅！！！！！ -->
### <font color = blue>作者写在前面：</font> ###
* 瞎搞的
* 如有大佬路过，请手下留情  
* 登录GitHub并不是很流畅，所以。。。

事情的起因是大学物理实验开课了。好家伙第一个实验是密立根油滴实验。虽然要记录的实验数据并不多。这里采用的是<mark>  静态法测量油滴电量  </mark>，数据分析采用的是<mark>  反向验证法  </mark>。需要测量的是两个数据。一个是<mark>  平衡电压  </mark>。一个是<mark>  匀速下落时间  </mark>。然后计算式在我看来非常麻烦。然后就有小伙伴撺掇我用C++写个脚本出来处理这些数据。恰巧有点时间。就把它搞出来了。还有小伙伴让我做分享，就有了这些。。。

## 使用方法 ##
---
我这里实验要求是每个油滴测<mark>3</mark>次。一共测量<mark>5</mark>个油滴  
>ps:如果每个油滴的测量次数与3不同，请修改源代码中定义的常量  
`#define N 3`  
并将N后面的数值修改为你需要的数字并手动编译或者在自己的IDE环境中编译运行

1.直接使用Millian.exe。
* 在同一个路径下放置数据文本文件`data.txt`  
格式如下：  
平衡电压<空格>下落时间  
以一个油滴的三组数据为例
```
170 9.10
168 8.99
168 8.94
```
* 正常应该有3 * 5 = 15 行数据。（ps：全怼一行里空格分开也不是不可以）  
然后一组数据出来的结果大概是这样：  
![一张图片](suoyin/Snipaste_2022-11-14_23-53-40.png)  
* 看，是不是很规整，很好看23333  

2.手动编译  
`g++ Millikan.cpp -o 密立根油滴实验.exe`  
`clang++ --target=x86_64-pc-mingw64 Millikan.cpp -o 密立根油滴实验.exe`  

3.利用cpp源码使用自己的IDE集成环境

</br>
</br>
</br>

感谢<mark>@monny </mark>提供的示例实验数据  
[^_^](抽到纳西妲了耶，来和我的牛杂师傅贴贴~~！)