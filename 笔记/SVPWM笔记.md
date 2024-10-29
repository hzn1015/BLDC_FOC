 
 # SVPWM空间矢量调制

空间矢量脉宽调制（SVPWM）是一种高效的调制技术，用于控制三相逆变器以驱动电动机。SVPWM通过生成空间矢量，将直流电源的电压转换为等效的三相交流电压，从而实现对电动机的精确控制。其主要优点包括提高电压利用率、减少谐波产生以及优化电机的运行效率。

SVPWM的基本原理是将电压矢量映射到一个坐标平面上，通过调制不同的电压矢量组合来实现目标输出电压。通过计算每个矢量的作用时间，SVPWM能够在每个开关周期内生成连续的正弦波形（这里正弦波形指的是三相电流波形而不是电压），从而实现平滑的电机驱动。

## 空间矢量


假设有三个相位互差120°的向量$x_a,x_b,x_c$,并且满足$x_a+x_b+x_c=0$,可以引入变换
$$
X_{out}=x_a+a*x_b+a^2*x_c;
$$
其中$a=e^{j\frac{2\pi}{3}}$,$a^2=e^{j\frac{2\pi}{3}}$,注意：$a^2$不是平方，a和a^2是复平面上的基向量
由于任意不平行的矢量可以合成所在复平面上的所有矢量，因此复平面上的 ***空间矢量***可以由$x_a,x_b,x_c$合成
因此我们用$x_a,x_b,x_c$三个基向量表示复数$x_out$如图所示
![空间矢量合成图](SVPWM\img\Example1_img1.png)
可以看出由$x_a,x_b,x_c$合成空间矢量$x_{out}$，其中$x_a=0.5,x_b=0.5*a,x_c=-0.5*a^2$,$x_{out}$满足复数运算定理。

为了计算其中的关系，我们将$x_out$的实部和虚部分开求解可以得到

$$Re X_{out}=x_a-\frac{1}{2}x_b-\frac{1}{2}x_c \\
Im X_{out}=\sqrt{3}/2x_b-\sqrt{3}/2x_c
$$
给$X_out$插入一个零矢量，将其写成矩阵形式有
$$
\left[
    \begin{matrix}
        Re X_{out}\\
        \\
        Im X_{out}\\
        \\
        0\\
    \end{matrix}
\right]=\left[
    \begin{matrix}
        1&-\frac{1}{2}&-\frac{1}{2}\\
\\
        0&\frac{\sqrt{3}}{2}&-\frac{\sqrt{3}}{2}\\
        \\
        \frac{1}{2}&\frac{1}{2}&\frac{1}{2}\\
    \end{matrix}
\right]\left[
    \begin{matrix}
        x_a\\
        \\
        x_b\\
        \\
        x_c\\
    \end{matrix}
\right]
$$
可得变换矩阵$T=
\left[
    \begin{matrix}
        1&-\frac{1}{2}&-\frac{1}{2}\\
\\
        0&\frac{\sqrt{3}}{2}&-\frac{\sqrt{3}}{2}\\
        \\
        \frac{1}{2}&\frac{1}{2}&\frac{1}{2}\\
    \end{matrix}
\right]$

因为T矩阵的行列式：
$
\left|
    \begin{matrix}
        1&-\frac{1}{2}&-\frac{1}{2}\\
\\
        0&\frac{\sqrt{3}}{2}&-\frac{\sqrt{3}}{2}\\
        \\
        \frac{1}{2}&\frac{1}{2}&\frac{1}{2}\\
    \end{matrix}
\right|=1.2990$不等于0，因此变换矩阵T可逆，因此只要知道 $x_{out}$就可以反过来得到$x_a,x_b,x_c$(很重要！这就是为什么我们可以通过空间矢量得到三相正弦波形的原因)

由于空间静止的电动机定子三相绕组轴线相位差120°，因此可以使用上面的基向量来表示三相电压的相位差。同时由于三相电压的关系满足
三相定子绕组电压方程如下：
$$
U_a=U_m\sin(\omega_1t)\\
U_b=U_m\sin(\omega_1t+\frac{2\pi}{3})\\
U_c=U_m\sin(\omega_1t+\frac{4\pi}{3})\\
$$
因此可以定义三相定子电压空间矢量为：
$$
U_{A}=kU_a\\
U_{B}=kU_be^{j\frac{2\pi}{3}}\\
U_{C}=kU_ce^{j\frac{4\pi}{3}}\\
$$
三相合成矢量
$u_s=U_A+U_B+U_C=kU_a+ku_ae^{j\frac{2\pi}{3}}+ku_ae^{j\frac{4\pi}{3}}$
计算得到 $u_s=kU_me^{j\omega_1t}$,因此$u_s$是一个以电源角频率$\omega_1$为角速度匀速旋转的空间矢量，k为调节系数，恒功率情况下为 $\sqrt(\frac{2}{3})$ ,恒幅值情况下为 $\frac{2}{3}$
合成矢量如图所示，可以看出合成的空间矢量在复平面上的轨迹是个圆
![合成矢量动图](SVPWM\img\dynamic_vector_animation.gif)

根据前面我们知道的空间矢量变换的可逆性，可以想象若空间矢量$U_{out}$的顶点运动轨迹为一个圆，则三相电压越趋近于三相对称的正弦波。因此通过空间矢量变换，可以将逆变器三相输出的三个标量控制问题转化为矢量控制问题。


## 逆变电路

![逆变电路示意图](SVPWM\img\nibian.png)
定义开关量$s_a,s_b,s_c$,因为上下桥臂不能同时开通，否则会导致逆变器短路。因此定义$s_a$为1时上桥臂开通，下桥臂关断。$x_a$为0时上桥臂关断，下桥臂开通。则上面的逆变器开关组态$s_{abc}$一共有8种。对于不同开关状态，我们可以得到不同8个空间矢量。（这里的开关是指逆变器桥臂完全打开）
$$
U_{out}=\frac{2U_dc}{3}(s_a+s_be^{j\frac{2\pi}{3}}+s_ce^{j\frac{4\pi}{3}})
$$
这里要乘 $\frac{2}{3}$ 是为了恒幅值变换。
当开关组态为$s_a=1,s_b=0,s_c=0$时，电机内部等效电路如图所示：
![A相通电等效电路](SVPWM\img\dengxiaodianluy.png)
根据串联分压定律可以得到 $u_{AN}=\frac{2}{3}*U_{dc},u_{BN}=-\frac{1}{3}U_{dc},u_{CN}=-\frac{1}{3}U_{dc}$。根据打开开关的不同，可以得到不同的组合，因此对于交流侧相电压$u_{AN},u_{BN},u_{CN}$满足关系：
$$
\left\{
    \begin{matrix}
        u_{AN}=\frac{U_{dc}}{3}(2s_a-s_b-s_c)\\
        \\
        u_{BN}=\frac{U_{dc}}{3}(2s_b-s_a-s_c)\\
        \\
        u_{CN}=\frac{U_{dc}}{3}(2s_c-s_b-s_a)\\
    \end{matrix}
    \right.
$$
将8种开关状态组合带入式子中，可以得到交流测相电压$u_{AN},u_{BN},u_{CN}$，线电压$u_{ab},u_{bc},u_{ac}$,以及$u_{out}$的值对于的表格如下：
$s_a$|$s_b$|$s_c$|$u_{AN}$|$u_{BN}$|$u_{CN}$|$u_{ab}$|$u_{bc}$|$u_{ac}$|$u_{out}$|
-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
0|0|0|0|0|0|0|0|0|0|
1|0|0|$2U_{dc}/3$|$-u_{dc}/3$|$-u_{dc}/3$|$u_{dc}$|0|$-u_{dc}$|$\frac{2}{3}u_{dc}$|
0|1|0|$-u_{dc}/3$|$2u_{dc}/3$|$-u_{dc}/3$|$-u_{dc}$|$u_{dc}$|0|$\frac{2}{3}u_{dc}e^{j\frac{2\pi}{3}}$|
1|1|0|$u_{dc}/3$|$u_{dc}/3$|$-2u_{dc}/3$|0|$u_{dc}$|-$u_{dc}$|$\frac{2}{3}u_{dc}e^{j\frac{\pi}{3}}$|
0|0|1|$-u_{dc}/3$|$-u_{dc}/3$|$u_{dc}/3$|0|$-u_{dc}$|$u_{dc}$|$\frac{2}{3}u_{dc}e^{j\frac{4\pi}{3}}$|
1|0|1|$u_{dc}/3$|$-2u_{dc}/3$|$u_{dc}/3$|$u_{dc}$|$-u_{dc}$|0|$\frac{2}{3}u_{dc}e^{j\frac{5\pi}{3}}$|
0|1|1|$-2u_{dc}/3$|$u_{dc}/3$|$u_{dc}/3$|$-u_{dc}$|0|$u_{dc}$|$\frac{2}{3}u_{dc}e^{j\pi}$|
1|1|1|0|0|0|0|0|0|0|

根据$u_{out}$可以绘制出空间矢量分布图：
![SVPWM空间矢量图](SVPWM\img\kongjianshiliangtu.png)

因此可以由8个基础空间矢量合成复平面内的任意矢量。

## SVPWM算法合成原理

SVPWM算法的理论基础是平均等效原理，即在一个开关周期$T_s$内通过对基本电压矢量加以组合，使平均值与给定电压矢量相等。想要获取某个区域中的电压空间矢量$U_{out}$,可以用该区域两个相邻的非零矢量和零矢量在时间上的不同组合得到。以扇区1为例，空间矢量合成示意图如下：

![示意图](SVPWM\img\空间矢量合成示意图.png)

根据平衡等效原则可以得到下式：

$$
T_sU_{out}=T_4U_4+T_6U_6+T_0\\
\\
T_4+T_6+T_0=T_s\\
$$

$$
U_1=\frac{T_4}{T_s}U_4\\
\\
U_2=\frac{T_6}{T_s}U_6\\
$$
由上图根据正弦定理有：
$$
\frac{|U_{out}|}{\sin\frac{2\pi}{3}}=\frac{|U_1|}{\sin(\frac{\pi}{3}-\theta)}=\frac{|U_{2}|}{\sin\theta}
$$
其中：$\theta$为合成矢量与主矢量（在合成矢量右边的矢量）的夹角，将 $U_4=\frac{2}{3}U_{dc},U_6=\frac{2}{3}U_{dc},U_{out}=U_m$ 代入可以得到：
$$
\left\{
    \begin{matrix}
        T_4=\sqrt{3}\frac{U_m}{U_{dc}}T_s\sin(\frac{\pi}{3}-\theta)\\
\\
        T_6=\sqrt{3}\frac{U_m}{U_{dc}}T_s\sin\theta\\
\\
        T_0=T_7=\frac{1}{2}(T_s-T_4-T_6)\\
    \end{matrix}
    \right.
$$
$U_m$为三相相电压，$U_{dc}$为直流逆变器的直流母线电压

定义SVPWM的调制比
$$
M=\frac{\sqrt{3}U_m}{U_{dc}}
$$
在SVPWM调制中，为了让合成矢量能够在线性区域内调制，则要满足以下关系$U_m<=\frac{2}{3}U_{dc}$,因此最大调制比$M_{max}=\frac{2}{\sqrt{3}}=1.1547$。因此在SVPWM中调制深度最大值可以达到1.1547，比SPWM调制比高0.1547，所以其对直流母线电压的利用率更高。(不过一般为了让空间矢量能够以圆形旋转磁场矢量的形式生产，所以一般在内切圆的范围内进行调制，此时的调制比就是1)

## SVPWM算法实现

流程：扇区判断->计算非零矢量和零矢量作用时间->计算扇区时间切换点->三角波比较输出开关组态->控制逆变器

### 扇区判断

判断电压空间矢量 $U_{out}$ 所在扇区的目的是确定本开关周期所使用的基本电压空
间矢量。用$u_{\alpha}$和$u_{\beta}$表示参考电压矢量$U_{out}$在$\alpha,\beta$轴上的分量
定义变量$U_{ref1},U_{ref2},U_{ref3}$,
$$
\left\{
    \begin{matrix}
        U_{ref1}=u_{\beta}\\
        \\
        U_{ref2}=\frac{\sqrt{3}}{2}u_{\alpha}-\frac{1}{2}u_{\beta}\\
        \\
        u_{ref3}=-\frac{\sqrt{3}}{2}u_{\alpha}-\frac{1}{2}u_{\beta}\\
    \end{matrix}
\right.
$$
再定义变量A,B,C
若$U_{ref1}>0$,A=1,否则A=0;
若$U_{ref2}>0$,B=1,否则B=0;
若$U_{ref3}>0$,C=1,否则C=0;
令$N=4C+2B+A$,则可以得到与扇区的关系如下
![扇区判断](SVPWM\img\ShanquPanduan.png)
通过这个表就可以得到空间矢量的扇区位置，选择对应的相邻基向量。

### 非零矢量和零矢量作用时间计算

首先我们定义相邻两个矢量的作用时间为$T_{first},T_{second}$,再看回刚刚的矢量示意图
![示意图](SVPWM\img\空间矢量合成示意图.png)
可以得到
$$
\left\{
    \begin{matrix}
        u_{\alpha}=\frac{T_{4}}{T_s}|U_4|+\frac{T_6}{T_s}|U_6|\cos\frac{\pi}{3}\\
\\
        u_{\beta}=\frac{T_6}{T_s}|U_6|\sin\frac{\pi}{3}\\
    \end{matrix}
\right.
$$
$$
\left\{
    \begin{matrix}
        T_4=\frac{\sqrt{3}T_s}{2U_{dc}}(\sqrt{3}u_{\alpha}-u_{\beta})\\
\\
        T_6=\frac{\sqrt{3}T_s}{2U_{dc}}u_{\beta}
    \end{matrix}
\right.
$$
在这里面作用时间 $T_{first}=T_4$,$T_{second}=T_6$
同理可以得到其他扇区的作用时间。令
$$
\left\{
    \begin{matrix}
        X=\frac{\sqrt{3}T_su_{\beta}}{u_{dc}}\\
        \\
        Y=\frac{\sqrt{3}T_s}{u_{dc}}(\frac{\sqrt{3}}{2}u_{\beta}+\frac{1}{2}u_{\beta})\\
        \\
        Z=\frac{\sqrt{3}T_s}{u_{dc}}(-\frac{\sqrt{3}T_s}{2}u_a+\frac{1}{2}u_{\beta})\\
    \end{matrix}
\right.
$$
可以得到各个扇区$T_0(T_7),T_{first},T_{second}$的作用时间如表所示：

![矢量作用时间图](SVPWM\img\作用时间图.png)

这里的$T_4,T_6$实际就是上面的$T_{first},T_{second}$,不是单独指$U_4$和$U_6$的作用时间

如果$T_{first}+T_{second}>T_s$,则需要进行过调制处理
$$
\left\{\begin{matrix}
    T_{first}=\frac{T_{first}}{T_{first}+T_{second}}\\
    \\
    T_{second}=\frac{T_{second}}{T_{first}+T_{second}}\\
\end{matrix}
\right.
$$

### 扇区矢量切换点的确定

定义
$$
\left\{
    \begin{matrix}
        T_a=(T_s-T_{first}-T_{second})\\
        \\
        T_b=T_a+T_{first}/2\\
        \\
        T_c=T_b=T_{second}/2\\
    \end{matrix}
\right.
$$
定义a相切换时间为$T_{cm1}$,b相切换时间为$T_{cm2}$,c相切换时间为$T_{cm3}$
可以得到各个扇区切换时间如下:
![扇区切换时间](SVPWM\img\扇区切换时间.png)
具体怎么切换以下面扇区一为例
