# [Algorithm] Preshower Detector Alignment

*<div style="text-align: center;" markdown="1">`minimum chi-squared` `gradient decent` `rotation matrix` `statistics`</div>*
Same article : https://hackmd.io/s/B1DKRSJNb

## Introduction
[Preshower detector (ES)](http://cms.web.cern.ch/news/ecal-preshower)  detector is built in the endcaps of the [CMS](http://cms.web.cern.ch/news/what-cms) detector between [tracker](http://cms.web.cern.ch/news/tracker-detector) (before ES, close to collision spot) and [Electromagnetic calorimeter (ECAL)](http://cms.web.cern.ch/news/electromagnetic-calorimeter) (behind ES, far from collision spot) for increasing the precision of position reconstruction of photon and electron. There are four donut-shape planes distributed in each endcap, i.e. two in each endcap which are called the *front* and *rear* planes in the endcap.

[<div style="text-align: center;" markdown="1"><img src="http://hep1.phys.ntu.edu.tw/~alpha78718/cv/esidea.png" height="400"></div>](http://hep1.phys.ntu.edu.tw/~alpha78718/cv/esidea.png)

Each ES plane is made by several silicon modules with the width of 6.3 cm. The module is consist of 32 silicon strips. The silicon strips in the front planes are placed with vertical direction, while they are placed with horizontal direction in the rear planes. The direction gives the particular dimension sensitivity, the front plane is sensitive to $x$ direction, and rear plane is sensitive to $y$ direction. Basing on the $z$ coordinate of the CMS from $-$ to $+$, the four planes are denoted as $+\, Rear$, $-\,Front$, $+\,Front$ and $+\,Rear$. Thus, when the charge particle or photon passes through the silicon sensor, it remains the signal, called ***hit***, and then the position can be recognized.

Since the detector is placed in the large magentic field space, the ES planes' positions are misaligned. This affect the poistion recontruction of the particle in the LHC data. The ES alignment thus plays important role for the data quality. However, it is impossible to fix the ES planes' positions mechanically, we apply the position correction to the particle by transformation matrix. I maintain and update this framework since 2012 to 2017, faced the challenge of long-shotdown, Run II of CERN-LHC and the problem of maganetic field in the CMS. 

## Techniques
The alignment algorithm for each plane are applied independently. The algorithm is supervised learning which compares the ***measured hits*** of *ES* and ***predicted hits*** predicted from particle *tracks* in the *tracker*. As illustrated in the figure of [Introduction](#Introduction), after the collision, the particle tracks (dot lines) from the tracker (light yellow) are extrapolated to ES planes (light green) be the *predicted hits*. However, the tracker is expected to be well aligned, the *measured hits* on ES planes recored in the LHC data has to be corrected with respect to the *predicted hits* (target).

### 1. Defination
By using ***minimum $\chi^2$*** method, the position of *measured hits* are updated iteratively until the ES plane is aligned within the its position resolution. The $\chi^2$ is defined as

$$
\begin{split}
\chi^2&=\sum_{i=1}^{tracks}\epsilon_i^{T}V_i^{-1}\epsilon_i \\
&=\sum_{i=1}^{tracks}\begin{bmatrix} \epsilon_x\,\,\,\epsilon_y\end{bmatrix}_i
\begin{bmatrix} \sigma_x^2 & \sigma_{xy} \\ \sigma_{yx} & \sigma_y^2\end{bmatrix}^{-1}_i
\begin{bmatrix} \epsilon_x \\ \epsilon_y\end{bmatrix}_i\ ,
\end{split}
$$

where the ES plane is expected to only sensitive to 2-dimension, i.e. x and y axis; $\epsilon_x$ ($\epsilon_y$) is the position difference between the closest *measured hit* and *predicted hit* on x-axis (y-axis) in ES-plane coordinate, which is also called ***residual***. $V_i$ is the covariance matrix (error matrix) for tracks extrapolation and intrinsic resolution. If the measurment of x and y is uncorrelated, the errors are $\sigma_{xy}=\sigma_{yx}=0$ in anti-diagonal, the $\chi^2$ is simplied to the rare case as

$$
\chi^2=\sum_{i=1}^{tracks}\left(\frac{\epsilon_x^2}{\sigma_x^2}+\frac{\epsilon_y^2}{\sigma_y^2}\right)_i\ .
$$

The defination of residual is as

$$
\begin{split}
\epsilon &= v'_{predicted} - v'_{measured} \\
&= R_t(v_{predicted}-O_t)-R(v_{measured}-O)\ ,
\end{split}
$$

where $v'$ is the cartesian coordinate in the ES-plane coordinate, while $v$ is in the CMS coordinate. $R_t$ and $O_t$ is the updated $3\times3$ rotation matrix and ES-plane origin with the CMS coordinate in $t$ iteration, i.e. the $\epsilon$ and $\chi^2$ are changed iteratively; the original rotation matrix is indentity, $R=I$, and $O$ is the default origin. The ES-plane coordinate is as the figure:

[<div style="text-align: center;" markdown="1"><img src="https://i.imgur.com/TmgYYff.png" height="150"></div>](https://i.imgur.com/TmgYYff.png)

which includes 3 rotations of the ***Euler angles***, $\alpha$, $\beta$ and $\gamma$, along the 3 transitions, $x$, $y$ and $z$, respectively. The rotation matrix is defined as

$$
R=\begin{bmatrix} \cos{\beta}\cos{\gamma}-\sin{\alpha}\sin{\beta}\sin{\gamma} & \cos{\beta}\sin{\gamma}+\sin{\alpha}\sin{\beta}\cos{\gamma} & -\cos{\alpha}\sin{\beta} \\
 -\cos{\alpha}\sin{\gamma} & \cos{\alpha}\cos{\gamma} & \sin{\alpha} \\
\sin{\beta}\cos{\gamma}+\sin{\alpha}\cos{\beta}\sin{\gamma} & \sin{\beta}\sin{\gamma} - \sin{\alpha}\cos{\beta}\cos{\gamma} & \cos{\alpha}\cos{\beta}
\end{bmatrix}\ ,
$$

which rotates from $\gamma$, $\alpha$ to $\beta$. 

### 2. Iteration and parameter update
According to the residual $\epsilon$ in above, the updating parameter are the rotation matrxi, $R'(\alpha,\,\beta,\,\gamma)$, and the origin of ES-plane, $O'(x,\,y,\,z)$. The coordinate of ES plane can be presented with 6 dimensional vector as 

$$
p = \begin{bmatrix} x\\ y \\ z \\ \alpha \\ \beta \\ \gamma \end{bmatrix}\ ,
$$


and the variance of all parameters is denoted as $\delta p=[\Delta x,\,\Delta y,\,\Delta z,\,\Delta\alpha,\,\Delta\beta,\,\Delta\gamma]^T$. The residual thus is as function of $p$, i.e. $\epsilon(p)$.

The parameter updating is used ***gradient descent*** method, which finds the best parameters to reach the minimum of $\chi^2$. The iteration for the $\chi^2$ is expected to climb down the hill until the valley. Acoording to this concept, the step to reach the minimum can be obtained by the *chain rule of differential* as like:

$$
\begin{split}
\Delta x &= \frac{\Delta f(x)}{\Delta x}\frac{\Delta x}{\Delta f(x)}\Delta x \\
&=\dot{f}\times\ddot{f}^{-1}\\
&=\frac{g}{a}\ ,
\end{split}
$$

where $f(x)$ is a certain function having the minimum point as function of $x$; $g$ is the first-order-differential term $\dot{f}$ w.r.t $x$, called ***gradient (slope)*** ; and $a$ is the second-order-differential term $\ddot{f}$ w.r.t $x$, called ***second derivative (curvature)*** . Since the $f(x)$ has the minimum point, e.g. $f(x)=ax^2+bx+c$, the direction of updating $x$ (movement) is opposite from the sign of $g$, e.g. going down hill by increasing $x$, the $g$ is supposed to be negative. In the meanwhile, the $a$ term can ensure the $x$ is in the valley instead of the summit. Thus, the $x$ updating in $t$ iteration becomes as 

$$
x_t=x_{t-1}-\Delta x_t\ .
$$

By replace the $f(x)$ to $\chi^2$ as function of $\epsilon_i(p)$

$$
\begin{split}
\delta p &= (\nabla_p^2\chi^2)^{-1}\,\nabla_p\chi^2 \\
&=\left[\sum_{i=1}^{tracks}J^T_iV_i^{-1}J_i\right]^{-1}\left[\sum_{i=1}^{tracks}J^T_iV_i^{-1}\epsilon_i\right] \\
&=M^{-1}P\ ,
\end{split}
$$

where $J_i$ is a Jacobian matrix of $\epsilon_i$ as 
$$
J_i=\nabla_p\epsilon_i\ ,
$$ 

where it is $6\times2$ metrix, since $\nabla_p$ is a 6 dimensional operator as $[\frac{\partial}{\partial x},\,\frac{\partial}{\partial y},\,\frac{\partial}{\partial z},\,\frac{\partial}{\partial\alpha},\,\frac{\partial}{\partial\beta},\,\frac{\partial}{\partial\gamma} ]$. The $M$ is the acceration term as a $6\times6$ squared matrix, and $P$ is the gradient term as a $6\times1$ vector. As mentioned in above, since the direction of updating $p$ is opposite from $\delta p$, the updated coordinate of ES-plane with $t$ iterations is

$$
p=p_O-\delta p_1-\dots-\delta p_t\ .
$$

As the following figures from one of ES plane, the alignment algorithm changes each parameters until reaching the stopping point, i.e. the step of each dimension is zero.

<div style="text-align: center;" markdown="1"><img src="http://hep1.phys.ntu.edu.tw/~alpha78718/cv/esiters.png" height="400"></div>

### 3. Resolution
The alignment is achived by the hit measurment of the ES plane, the perfomance is limited by the resolution of the silicon sensor. As metioned in the [Introduction](#Introduction), according to the different structure of the silicon strips, front and rear plane have different sensitivities for the $x$ and $y$ axis, i.e. the resolutions of the measurment are different as well. Taking the example of the front plane, every group of the 32 silicon strips is well placed vertically in the 6.3 $\times$ 6.3 $\text{cm}^2$ module. The resolutions of $x$ and $y$ are caculated as

$$
\begin{split}
\sigma_x&=\frac{width}{\sqrt{12}}=\frac{6.3/32}{\sqrt{12}}&=0.055\,\text{cm}\ ,\\
\sigma_y&=\frac{length}{\sqrt{12}}=\frac{6.3}{\sqrt{12}}&=1.761\,\text{cm}\ ,
\end{split}
$$

where $\frac{1}{\sqrt{12}}$ is obtained by assuming the hit probability of slicon sensor is uniform distribution in every dimension, detail see [here](https://hackmd.io/s/HJ6D65HHG#Uniform-distribution). In contrast, the resolutions of the rear plane are $\sigma_x=1.761$ and $\sigma_y=0.055$, since it sensitive to $y$ direction. These uncertainties are included in the covariance matrix for the minimum $\chi^2$ method.

## Results
The validation is done by checking the residual distribution. Since the sensitivitly difference in front and rear planes, the residual distributions are shown in the particular direction, i.e. the residual of $x$ direction for front plane, and the residual of $y$ direction for rear plane. AS shown in following figures, the residual distribution of the misaligned ES-plane is away from zero (first figure), while the mean of the aligned ES-plane (second figure) is zero within the resolution $\sigma_x=0.055\,\text{cm}$.

<div style="text-align: center;" markdown="1"><img src="http://hep1.phys.ntu.edu.tw/~alpha78718/cv/esbefor.png" height="250"><img src="http://hep1.phys.ntu.edu.tw/~alpha78718/cv/esafter.png" height="250"></div>

## Usage
CMSSW_8_0_8
Description on Wiki for AlignmentTool
https://github.com/chiyi/ESAlignmentWorks/wiki/AlignmentTool

Provide DB files

1. Checkout package:
	```
	cmsrel CMSSW_8_0_8
	cd CMSSW_8_0_8/src
	cmsenv
	git cms-addpkg Geometry/EcalAlgo
	git cms-addpkg Geometry/CaloEventSetup
	git clone https://github.com/alphatsai/AlignmentTool.git
	scram b -j16
	```

2. Produce DB files which contain ES coordinate:
	```
	cd Geometry/CaloEventSetup/test
	vi TestWriteESAlignments.cc
	```

3. Add the aligned coordinate which are with respect to ideal coordinate:

	Example in Run1: https://hypernews.cern.ch/HyperNews/CMS/get/ecal-calibration/560.html
	Example under ESAlignTool/genDBExample/TestWriteESAlignments.cc
	

## References
- Github : <https://github.com/juifa-tsai/AlignmentTool>
- [**Sensor Alignment by Tracks**, *arXiv:physics/0306034*](https://arxiv.org/abs/physics/0306034v2)
- Conference note : <https://doi.org/10.1016/j.nima.2012.04.007> by Dr. K.Y. Kao
- Related Machine learning course : https://www.youtube.com/watch?v=yKKNr-QKz2Q
