# 无刷电机FOC控制器

***

## 无刷电机数学模型

**1.FOC数学基础**
对于无刷电机来说，我们通过控制无刷电机A，B，C三相的电流来控制磁场，从而驱动无刷电机。
无刷电机的定子上有三个绕组，它们彼此相隔120°电角度。假设这三相绕组是对称的，分别为A,B,C
，它们所产生的电动势也是对称的，且相位差为120°。因此，每一相电流的表达式可以写成正弦波的形式：

$$I_a=I\sin(\omega_rt)$$

$$I_b=I\sin(\omega_rt-\frac{2\pi}{3})$$

$$I_c=I\sin(\omega_rt-\frac{2\pi}{3})$$


$\omega_r为转子的速度（注意这个速度是电角度的速度）$
***电流关系***
由基尔霍夫定律有：

$$I_a+I_b+I_c=0$$

***Clark变换***
Clarke变换用于将三相电流$I_a,I_b,I_c转换为α-β静止参考系下的两相电流I_\alpha,I_\beta$:

$$\left[\begin{matrix}I_\alpha\\
I_\beta\\
\end{matrix}\right]=\left[\begin{matrix}
    1&-\frac{1}{2}&-\frac{1}{2}\\
    0&\frac{\sqrt{3}}{2}&-\frac{\sqrt{3}}{2}
  \end{matrix}\right] 
  \left[\begin{matrix}
    I_a\\
    I_b\\
    I_c
  \end{matrix}\right]
$$

***Park变换***
利用转子的位置角$\theta_r$，将$\alpha-\beta$静止坐标系下的两相电流转换为与转子磁场同步旋转坐标系$d-p$坐标系下的电流$I_d,I_q$

$$
\left[
    \begin{matrix}
        I_q\\
        I_d\\
    \end{matrix}
\right]
=\left[
    \begin{matrix}
        \cos(\theta_r)&\sin(\theta_r)\\
        -\sin(\theta_r)&\cos(\theta_r)\\
    \end{matrix}
\right]
\left[
\begin{matrix}
    I_\alpha\\
    I_\beta
\end{matrix}
\right]\tag{4}
$$
***电压方程***

对于无刷电机三相电压，满足以下方程：
$$ V_s=I_s*R_s+L_s*\frac{dI_s}{dt}+e_s $$
其中
$V_s$为定子绕组的电压矢量
$I_s$为定子绕组的电流矢量
$R_s$为定子绕组的电阻
$L_s$为定子电感（包括自感和互感）
$e_s$为反电动势
反电动势$e_s$是定子磁链随时间变化产生的，反映了电机运行中的电动势，并且与转子的位置相关。通常表示为：$ e_s=\lambda_s*\frac{d\theta_r}{dt} $

***转矩方程***
$$ T_e=\frac{3}{2}p(\lambda_fI_q+(L_d-L_q)I_dI_q) $$
其中
$T_e$为电磁转矩
$p$为电机极对数
$\lambda_d ,\lambda_q$为$d$轴和$q$轴的磁链分量
$I_d$和$I_q$为d轴和q轴的电流矢量

***FOC实现流程图***
![本地路径](pmsm_foc.png) <!-- 此路径表示图片和MD文件，处于同一目录 -->
图片来自matlab官方教程
