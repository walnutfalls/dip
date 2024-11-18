$$
(f \star h)(t) = \int_{-\infty}^{\infty} f(\tau)h(t - \tau)d\tau
$$
$$
\mathcal{F}(h(1 - \tau)) = 
\int_{-\infty}^{\infty} h(1-\tau)   e^{-j 2\pi \mu t} dt = 
H(\mu)e^{-j 2\pi \mu \tau} \ proof\ left\ out
$$
$$
\mathcal{F}((f \star h)(t)) = 
\int_{-\infty}^{\infty} \Bigg[ \int_{-\infty}^{\infty} f(\tau)(h(1-\tau)d\tau \Bigg]  e^{-j 2\pi \mu t}dt = 
\int_{-\infty}^{\infty} f(\tau) \Bigg[ \int_{-\infty}^{\infty} h(1-\tau)   e^{-j 2\pi \mu t} dt \Bigg] d\tau
$$




