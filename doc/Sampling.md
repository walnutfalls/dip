Sampling can be expressed as an impulse train multiplied with a function:
$$
\tilde{f}(t) = f(t)s_{\Delta T}(t) = \sum_{n=-\infty}^{\infty} f(t) \delta(t - n\Delta T)
$$

Retrieval of a discrete sample is given by
$$
f_k = \int_{-\infty}^{\infty} f(t)\delta(t - k \Delta T) dt = f(k \Delta T)
$$

$$
\tilde{F}(\mu) = \mathcal{F}(\tilde{f}(t)) =
$$
$$
\mathcal{F}(f(t)s_{\Delta T}(t)) =
$$
$$
(F \star S)(\mu)
$$
$$
\mathcal{F}\{s_{\Delta T}\} = S(\mu) =\frac{1}{\Delta T} \sum_{n=-\infty}^{\infty} \delta(\mu - \frac{n}{\Delta T})
$$
the Fourier transform F( ) m of the sampled function f t () is an infinite, periodic sequence of copies of the transform of the original, continuous function.

## Sampling Theorem
Let $[-\mu_{max}, \mu_{max}]$ be the band, beyond which $F(\mu)$ of $f(t)$ is 0. $f(t)$ does not have those component frequencies in it. 

In order to extract $f(t)$ from $F(\mu)$, the sampling rate needs to be high enough. It needs to be more than twice $\mu_{max}$:
$$
\frac{1}{\Delta T} > 2\mu_{max}
$$
$$
0.5 f > \mu_{max}
$$
The *Nyquist Rate*  is twice the highest frequency. 

The Fourier Transform of a sampled function is multiplied by $H(\mu)$ - a lowpass filter - in order to isolate one copy of the original transform. Then, in theory, we can fully recover the original function via inverse Fourier Transform. 

Lowpass Filters are imperfect in practice, and there are many clever circuits built for the purpose, as well as software implementations. See the DSP For Engineers book for some examples of circuits. 

We need to sample at *over* the Nyuist rate, not equal to. 

## Recovery From Sampled Data
$$
f(t) = \mathcal{F}^{-1}(F(\mu)) =
\mathcal{F}^{-1}(H(\mu)\tilde{F}(\mu)) =
h(t) \star \tilde{f}(t)
$$
It can be shown that:
$$
f(t) = \sum_{-\infty}^{\infty} f(n \Delta T) sinc[\frac{t - n \Delta T}{\Delta T}]
$$

These are the equations this follow from, but I dunno how to drive:
$$
(f \star h)(t) = \int_{-\infty}^{\infty} f(\tau)h(t - \tau)d\tau
$$
$$
(f \star h)(t) \Leftrightarrow (H \cdot F)(\mu)
$$
$$
\tilde{f}(t) = f(t)s_{\Delta T}(t) = \sum_{n=-\infty}^{\infty} f(t) \delta(t - n\Delta T)\ (4-27)
$$
$$
h(t) \star \tilde{f}(t) = h(t) \star \sum_{n=-\infty}^{\infty} f(t) \delta(t - n\Delta T)
$$
$$
\int h(\tau) \sum f(t - \tau)\delta(t - \tau - n \Delta T) d\tau = 
$$
$$
\sum \int \ f(t)\ h(\tau)\delta(t - \tau - n \Delta T) d\tau =
$$
$$
...
$$
$$
f(t) = \sum_{-\infty}^{\infty} f(n \Delta T) sinc[\frac{t - n \Delta T}{\Delta T}]
$$
