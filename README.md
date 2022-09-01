# top-k商户查询

## 题面

**TL;DR**：给定m个点，对于另外n个点中的每个点，求最近的k个点。

> 随着智能手机的普及，地理信息在诸如高德地图、大众点评、饿了么等App中得到广泛的应用，此次数据结构课程期末大作业将模拟实际生活中的查询需求，完成基于地理信息和文本信息的查找任务。
>
> 系统中每家商户包含以下3项信息：
> 1.位置(x,y)，x>0, y>0；
> 2.商家名称， 12 位 A−Z 字符串，不含小写；
> 3.菜系， 6 位 A−Z 字符串，不含小写；
>
> 你的程序需要提供给用户以下查询的功能：用户输入自己的位置点如 <ux,uy>、感兴趣的菜系和整数 k 值，程序按照由近到远输出商家名称和距离，距离相等时以照商家名称的字典序为准。在此距离精确到小数点后的 3 位（四舍五入）。若满足条件的商户不足 k 个，则输出所有满足条件的商家信息。若存在一次查询中无任何满足条件的商户，则输出空行即可。

## 格式

**TL;DR**：

* 输入m,n
* 输入m个点的`name` `x` `y` `key`
* 输入n个点的`x` `y` `key` `k`
* 输出k个点的`name` `dis`，dis为其到查询点的距离
* 四舍五入到小数点后3位的`dis`为第一关键字，`name`为第二关键字

> 输入格式
>
> 第 1 行：商户的数量 m 和查询的数量 n ， m 和 n 为整数，均不超过 109 ；
> 第 2−(m+1) 行：商户的信息，包括商家名称，位置 x ，位置 y 和菜系；
> 最后的 n 行：每一行表示一次查询，包括用户的位置 ux 和 uy 、菜系名称、 k 值；
>
> 输出格式
>
> 对应于每一次查询，按照顺序输出满足条件的商户信息，每一行对应于一家商户。

## 额外信息

输入数据已知（但太大了所以没存到这里），写个[脚本](#file-test-py)扫一下，得到如下结果：

```
x_min=0
x_max=30000
y_min=0
y_max=30000
```

坐标范围是0到30000，题目描述的x>0, y>0有误，但没什么影响；上限30000，可以用16位整数保存。

另外name只有12位A-Z，64位够存了，这样每个点需要16+16+64=96位，考虑到对齐会用128位的空间。

然而，如果存成array<char, 12>，用到16+16+12*8，还是128位，说明name压成64位并没什么用。

## 数据结构

分析过程略去，总之最终决定用kd树。

具体实现与优化历尽磨难，总之最终重构到还算能看的版本在[这里](#file-eoj_contest289_problemb-cpp)。

## 后记
其实还剩一些优化方向没有尝试，如果以上没能把时间压缩到1s以内，以下是可选内容：

* 将所有浮点数换成定点数，节约算力：
  * 若此，`sqrt`应替换为牛顿法求平方根
  * 定点数优化：性能成倍提升 - 韦易笑的文章 - 知乎 https://zhuanlan.zhihu.com/p/149517485
* 寻找完美哈希函数：
  * 进行了一些搜索但没能找到简单的解决方案
  * GNU的完美哈希函数生成工具：https://www.gnu.org/software/gperf/manual/gperf.html
  * 文档过于反人类，至今未能学会使用
* 这份资料提到了一些其它算法：https://people.csail.mit.edu/indyk/helsinki-1.pdf
  * 权威性未知
* 这里显示了更多相关的数据结构与算法：https://en.wikipedia.org/wiki/Nearest_neighbor_search
  * 太多了以至于丝毫没有意愿去逐个研究