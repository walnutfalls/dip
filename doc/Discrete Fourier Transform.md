[[Fourier Transform]] For actual arrays of samples.
$$
\tilde{f}(t) = f(t)s_{\Delta T}(t) = \sum_{n=-\infty}^{\infty} f(t) \delta(t - n\Delta T)\
$$
$$
\tilde{F}(\mu) = \int_{-\infty}^{\infty} \tilde{f}(t) e^{-j 2\pi \mu t} dt = 
$$
$$
\int_{-\infty}^{\infty} \sum_{n=-\infty}^{\infty} f(t)\delta(t-n \Delta T) e^{-j 2\pi \mu t} dt =
$$
$$
\huge {
\sum_{n=-\infty}^{\infty}  \int_{-\infty}^{\infty} f(t)\delta(t - n \Delta T) e^{-j 2\pi \mu t} dt = 
}
$$
$$
\sum_{n=-\infty}^{\infty} f_n e^{-j 2\pi \mu n \Delta T}
$$
because of sifting and the equation for $f_n$, this this: $f(t)\delta(t - n \Delta T)$ turns to $f_n$ and you get the above. 
$$
f_n = \int_{-\infty}^{\infty} f(t)\delta(t - k \Delta T) dt = f(k \Delta T)
$$$$
\huge {
\sum_{n=-\infty}^{\infty}  \int_{-\infty}^{\infty} f(t)\delta(t - n \Delta T) e^{-j 2\pi \mu t} dt = 
}
$$
$$
\huge {
\sum_{n=-\infty}^{\infty}  f_n \int e^{-j 2\pi \mu t} dt =?
}
$$
$$
\sum_{n=-\infty}^{\infty} f_n e^{-j 2\pi \mu n \Delta T}
$$

If you want to get M sample of a [[Fourier Transform]] with interval $\Delta T$ , for $m = 0, 1, 2, 3...$ of $F_m$:
$\mu = \frac{m}{m \Delta T}$
$$
\huge{
	F_m = \sum_{n = 0}^{M - 1} f_n e^{\frac{-j 2\pi m n}{M}}
}
$$
The Inverse for n = 0,1,2... of $f_n$:
$$
\huge {
	f_n = \frac{1}{M} \sum_{m = 0}^{M-1} F_m e^{\frac{j 2\pi m n}{M}}
}
$$
For image and video purposes, we prefer (x,y) image coordinates, and (u,v) for frequency variables:
$$
\huge{
	F(u) = \sum_{x = 0}^{M - 1} f(x) e^{-j 2\pi u x / M}
}
$$
$$
\huge{
	f(x) = \frac{1}{M} \sum_{u = 0}^{M - 1} f(u) e^{j 2\pi u x / M}
}
$$
This is the **1D DFT pair**. 

## 2D DFT
$$
\huge {
F(u,v) = \sum_{x=0}^{M-1} e^{-j 2\pi u x / M} \sum_{y=0}^{N-1} f(x,y) e^{-j 2 \pi v y / N} =
}
$$
$$
\huge {
	\sum_{x=0}^{M-1} F(x,v)e^{-j 2\pi u x / m}
}
$$
$$
F(x,v) = \sum_{y=0}^{N-1} f(x,y) e^{-j 2\pi v y / N
}
$$