The term "Spatial Filtering" is as opposed to Frequency Domain filtering. So, you can blur an image by running it through a low-pass filter. But a Gaussian Blur is a spatial filter that achieves a similar result. 

Replaces each pixel by a function of the values of the neighbors. Linear function means *linear spatial filter*. 

## Linear Spatial Filtering
*Kernel* is a matrix of coefficients. 

The kernel dimensions are odd, because the kernel has to center on one pixel. So, for an $m \times n$ kernel, $m = 2a+1$ and $n = 2b+1$ (this is a way to describe an odd number of dimensions, as opposed to an even one). 

The formal way to describe kernel filtering is thus:
$$
g(x,y) = \sum_{s=-a}^{a} \sum_{t=-b}^{b}w(s,t)f(x+s,y+t)
$$
## Spatial correlation and [[Convolution]]
*Spatial correlation* is basically kernel filtering as described in the above equation. *Spatial [[Convolution]]* has the kernel rotated 180 degrees. 

A function (image) with all 0s but one 1 is a *discrete unit impulse*. Correlating (the normal kernel filter operation) produces a 180 degree rotated kernel output at the location of the 1.

**Convolving a function with a unit impulse yields a copy of the function**

Correlation is denoted by unfilled star. [[Convolution]] is denoted with a filled star:
$$
(w \star f)(x, y) = \sum_{s=-a}^{a} \sum_{t=-b}^{b} w(s,t)f(x-s,y-t)
$$
The minus here is enough to flip the kernel. *Linear Spatial Filtering* is this sum of products process. 

[[Convolution]] is commutative, so it doesn't matter if we think of the kernel, w, as rotated or the image. It is also associative. Correlation is not.

[[Convolution]] is generally preferred, and *linear spatial filtering* in the book refers to convolution in particular. 

Because of commutative property,  we can represent a pipeline of filters by convolving their kernels first. We have to explicitly apply padding then, to grow the images every time.
$S_v = m + M - 1$
$S_h = n + N - 1$
For Q kernels, all size $m \times n$:
$W_v = Q \times (m - 1) + m$
$W_h = Q \times (n - 1) + n$

## Separable Filter Kernels

A 2d function is separable if it can be written as the product of two 1d functions, $G_1(x)$ and $G_2(x)$. So, $G(x, y)=G_1(x)G_2(y)$
A separable kernel is a matrix that can be expressed as outer product of two vectors.
$w = uv^T$ where w is a matrix and u and v are vectors. 

The product of a column and row vector is the same as the 2d [[Convolution]] of the vectors. 

The separable property is used for computation speed boosts. 

[[Convolution]] is associative, so you can move the parenthesis like $a \star (b \star c) = (a \star b) \star c$. 
If we decompose a kernel into $w_1$ and $w_2$ then we can save $\frac{mn}{m+n}$ operations by convolving the image with 2 1d kernels, instead of with a mxn one. 

To determine if a kernel is separable, we need to determine if the kernel matrix has rank 1. A rank1 matrix is formed by the product of two vectors, which by definition is a separable kernel. 

A Gaussian kernel is separable, and is the only circularly symmetric one that is. 