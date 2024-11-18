A periodic function with period T can be expressed as the sum of sines and cosines multiplied by coefficients. This sum is the *Fourier Series*:
$$
\huge{
	f(t) = \sum_{n=-\infty}^{\infty} c_n e^{j \frac{2 \pi n}{T}t}
}
$$
The reference video shows this is a sum or rotating vectors. For simplicity, consider T 1, and integrate from 0 to 1.
## Coefficients Cn:
$$
\huge{
	c_n = \frac{1}{T} \int_{-\frac{T}{2}}^{\frac{T}{2}} f(t)e^{-j \frac{2 \pi n}{T}t} dt
}
$$
This is basically cancelling the rotations of every term except the nth term, by making the integral from 0 to T undergo some whole number of rotations, and therefore make a full circle, and so the average of this is 0.

Intuition: [But what is a Fourier series? From heat flow to drawing with circles | DE4](https://www.youtube.com/watch?v=r6sGWTCMz2k)

