*"Any periodic function can be expressed as the sum of sines and cosines of different frequencies, each multiplied by a different coefficient"*

It takes $MNmn$ operations to filter an $M \times N$ image with an $m \times n$ kernel. With a separable kernel, it takes $MN(m+n)$ operations. In the frequency domain, it takes $2MNlog_2(MN)$ operations. Coefficient of 2 because we have to compute forward and reverse FFT.

The advantage is $\frac{M^2 m^2}{4log_2(M)}$. If kernel is separable, it's $\frac{m}{2log_2(M)}$ .

Spatial filtering may be better. If $C(m) > 1$, FFT is better. FFT has advantage for kernels 7x7 and larger. For separable kernels, FFT is better for 27x27.

## Prerequisite Math
### Trig Stuff
Euler's formula is:
$e^{j\theta} = cos\theta + jsin\theta$ 
$e^{-j \theta} = e^{j (-\theta)} = cos(-\theta) + jsin(-\theta) = cos\theta - j sin\theta$
$sin\theta = \frac {e^{j \theta} - e^{-j \theta}}{2j}$
$sinc(m) = \frac{sin(\pi m)}{\pi m}$

See [[Euler's Formula and Identity]]
This whole thing is built on the rotating Euler identity complex number. It's mathematically convenient to deal with complex numbers, and reason about them in terms of $e^{i\theta}$ here. 

### Complex Numbers
$C = R+jI$ where $R,I \in \mathbb{R}$ and $j=\sqrt{-1}$
*Conjugate* of a complex number C, denoted as $C^*$ is defined as $C^* = R - jI$
You can plot them on a coordinate plane, like points (R,I)
In polar coordinates, $C=|C|(cos\theta + jsin\theta)$ where $|C| = \sqrt(R^2 + I^2)$
$tan(\theta)$ returns angles in range $[-0.5\pi, 0.5\pi]$, so need to keep track of the sign of I and R when actually computing $\theta$. This is what the $atan2$ function is usually about - you can get theta by doing $atan2(y,x)$ or $atan2(I, R)$ 

Euler's formula is:
$e^{j\theta} = cos\theta + jsin\theta$ . So, $C = |C|e^{j\theta}$ 

A complex function, $F(u)$ can be expressed as $F(u) = R(u) + jI(u)$ - it consists of real and imaginary component functions. 
We also have complex conjugate $F(u) = R(u) - jI(u)$ 
The magnitude is $|F(u)| = \sqrt{ R(u)^2 + I(u)^2 }$
Angle is $\theta(u) = arctan[I(u)/R(u)]$

### [[Fourier Series]]
See [[Fourier Series]]
### Impulses
A unit impulse is $\infty$ at t=0, and 0 elsewhere:
$$

\delta(x) = \begin{cases} 
  \infty & t = 0 \\  
  0 & t\neq 0
\end{cases}

$$
It's a spike of infinite amplitude but 0 duration, having unit area. It kind of "samples" a function:
$$
\int_{-\infty}^{\infty} f(t) \delta(t) dt = f(0)
$$

An impulse located at an arbitrary point $t_0$ denoted $\sigma(t-t_0)$ lets us sift a function at that point:
$$
\int_{-\infty}^{\infty} f(t) \delta(t - t_0) dt = f(t_0)
$$
Note that this is shifting the input space, kind of like [Fourier Series] shifts the series using a integrand factor, to make every part of the sum average to 0 by undergoing a whole number of rotations, except $C_n$ which now integrates to some constant. 

An impulse train is the sum of infinitely many impulses $\Delta T$ units apart:
$$
s_{\Delta T}(t) = \sum_{k=-\infty}^{\infty} \delta(t - k \Delta T)
$$
In the discrete case, the impulse is equal to 1 instead of infinity, and the sifting works the same way - it zeroes out the function everywhere except for the location of the impulse - $t_0$ . At $t_0$, it yields $f(t_0)$. We refer to $x$ instead of $t$ for discrete case though, so $x_0$ . 
$$
\sum_{x=-\infty}^{\infty} f(x) \delta(x - x_0) = f(x_0)
$$
### Fourier Transform of f(t)
Denoted $\mathcal{F}(f(t))$:
$$
\huge{
\mathcal{F}(f(t)) = \int_{-\infty}^{\infty} f(t) e^{-j 2 \pi \mu t}dt
}
$$
$\mu$ is a continuous variable, and as t is integrated out, we can say $\mathcal{F}(f(t)) = F(\mu)$
Note that the exponent is **negative** (as it also is in [Fourier Series])
$$
\huge{
	\mathcal{F}(\mu) = \int_{-\infty}^{\infty} f(t)e^{-j2 \pi \mu t} dt
}
$$
$$
Euler's\ Formula \rightarrow \huge{
	\mathcal{F}(\mu) = \int_{-\infty}^{\infty} f(t) [cos(2\pi \mu t) - jsin(2 \pi \mu t)] dt
}
$$
$\mu$ is in cycles per the unit that goes into $f$. If t is in seconds, $\mu$ it's cycles per second, or Hz. If we're dealing with distance, it's cycles per meter. Basically, **$\mu$ is the frequency** and **$F(\mu)$ is the amplitude,**. Note that it corresponds to $n$ as in the Fourier Series $C_n$. 

The Fourier Transform contains complex terms, so it's customary for display purposes to use the magnitude. 

#### Inverse Fourier Transform
$$
\huge{
	f(t) = \int_{-\infty}^{\infty} F(\mu) e^{\mu j 2\pi t}d\mu
}
$$
This is kind of like a sum of "how much of each frequency" times "that frequency" or "that Euler e rotation number".
Note that the exponent is **positive**

The relationship often shown as $f(t) \Leftrightarrow F(\mu)$
### Fourier Transform of Impulse Train
We use sifting properties of the impulse function $\delta(t)$
$$
\mathcal{F}(\delta(t)) = 
F(\mu) = 
\int_{-\infty}^{\infty} \delta(t) e^{-j 2\pi \mu t}dt =
\int_{\infty}^{\infty} e^{-j 2\pi \mu t}\ \delta(t)\ dt
$$
$$
Sifting: \int_{-\infty}^{\infty} f(t) \delta(t) dt = f(0)
$$ 
$$
Sifting\ (shift):\int_{-\infty}^{\infty} f(t) \delta(t - t_0) dt = f(t_0)
$$
$$
f(t) = e^{-j 2\pi \mu t} \rightarrow f(0) = \int_{-\infty}^{\infty} f(t) \delta(t) dt = e^{-j 2\pi \mu 0} = 1
$$
$$
\mathcal{F}(\delta(t - t_0)) = 
F(\mu) = 
\int_{-\infty}^{\infty} \delta(t-t_0) e^{-j 2\pi \mu t}dt = 
\int_{-\infty}^{\infty} f(t) \delta(t - t_0) dt = 
f(t_0) = 
\huge{
	e^{-j 2\pi \mu t_0}
}
$$
#### Periodic Impulse Train Derivation
Here, $\Delta T$ because important, because we're looking at a spiked ever $\Delta T$ units along the x axis...
$$
\huge{
\mathcal{F}(f(t)) = F(\mu) = \int_{-\infty}^{\infty} f(t) e^{-j 2 \pi \mu t}dt
}
$$
$$
\huge{
	f(t) = \int_{-\infty}^{\infty} F(\mu) e^{j 2\pi \mu t}d\mu
}
$$
$$
s_{\Delta T} = 
\sum_{-\infty}^{\infty} c_n e^{\frac{j 2\pi n}{\Delta T}t}
$$
$$
\huge{
	c_n = 
	\frac{1}{\Delta T} \int_{-0.5T}^{0.5T} 
		s_{\Delta T}(t) e^{\frac{2 \pi n}{\Delta T} t} dt
}
$$
Notice the integral range is only wide enough to capture the impulse at 0, which is just $\delta(t)$.
$$
Sifting: \int_{-\infty}^{\infty} f(t) \delta(t) dt = f(0)
$$
$$
\huge{
	c_n = 
	\frac{1}{\Delta T} \int_{-0.5T}^{0.5T} 
		s_{\Delta T}(t) e^{\frac{2 \pi n}{\Delta T} t} dt = 
	\frac{1}{\Delta T}  e^{\frac{2 \pi n}{\Delta T} 0} = 
	\frac{1}{\Delta T}  e^0 = 
	\frac{1}{\Delta T} 
}
$$
and so
$$
\huge{
	c_n = 
	\frac{1}{\Delta T}
}
$$
$$
\huge{
	s_{\Delta T} = 
	\frac{1}{\Delta T} \sum_{-\infty}^{\infty} e^{\frac{j 2\pi n}{\Delta T}t}
}
$$
Obtaining the Fourier Transform of the $s_{\Delta T}$ sum means obtaining sum Fourier Transforms of each term. 
$$
\mathcal{F}(e^{\frac{j 2\pi n}{\Delta T}t}) = \delta(\mu - \frac{n}{\Delta T})
$$
This part I really don't get. 


$$
\mathcal{F}\{s_{\Delta T}\} = \frac{1}{\Delta T} \sum_{n=-\infty}^{\infty} \delta(\mu - \frac{n}{\Delta T})
$$
The take-away is that this is also an impulse train, with period $\frac{1}{\Delta T}$ . 
## Of a [[Convolution]]
The convolution of two functions:

$$
(f \star h)(t) = \int_{-\infty}^{\infty} f(\tau)h(t - \tau)dt
$$
$$
\mathcal{F}((f \star h)(t)) = 
\int_{-\infty}^{\infty} \Bigg[ \int_{-\infty}^{\infty} f(\tau)(h(1-\tau)d\tau \Bigg]  e^{-j 2\pi \mu t}dt = 
\int_{-\infty}^{\infty} f(\tau) \Bigg[ \int_{-\infty}^{\infty} h(1-\tau)   e^{-j 2\pi \mu t} dt \Bigg] d\tau
$$
$\tau$ is the $f \star h$ iterator basically
### [[Convolution]] Theorem
$$
(f \star h)(t) \Leftrightarrow (H \cdot F)(\mu)
$$
$$
(f \cdot h)(t) \Leftrightarrow (H \star F)(\mu)
$$
Where $\Leftrightarrow$ indicates forward and reverse Fourier Transform. 

#### Explanation
The product of the Fourier Transforms of f(t) and h(t) is the Fourier Transform of the convolution of f(t) and h(t)
$$
\mathcal{F}(f(t)) = F(\mu)
$$
$$
\mathcal{F}(h(t)) = H(\mu)
$$
$$
\mathcal{F}(h(1 - \tau)) = 
\int_{-\infty}^{\infty} h(1-\tau)   e^{-j 2\pi \mu t} dt = 
H(\mu)e^{-j 2\pi \mu \tau} \ proof\ left\ out
$$
$$
\mathcal{F}((f \star h)(t)) = 
\int_{-\infty}^{\infty} f(\tau) \Bigg[ H(\mu)e^{-j 2\pi \mu \tau} \Bigg] d\tau =
$$
$$
H(\mu) \int_{-\infty}^{\infty} f(\tau)  e^{-j 2\pi \mu \tau}  d\tau = 
$$
$$
H(\mu)F(\mu) =
$$
$$
(H \cdot F)(\mu)
$$
where $\cdot$ is multiplication

We can get the convolution by getting $F(\mu)$, $H(\mu)$, and $(F \cdot H)(\mu)$ then get $F^{-1}((F \cdot H)(\mu))$


## Fast Fourier Transform [[FFT]]
There's a fast way to perform Fourier Transform that's been developed around the 60s. 