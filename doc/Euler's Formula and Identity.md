$$
e^{i\theta} = cos(\theta) + isin(\theta)
$$
## Derivation 
1. **Taylor Series of exe^{x}ex:** For any real number xxx, the exponential function exe^xex can be expanded as a Taylor series:
$$
e^x = 1 + \frac{x}{1!} + \frac{x^2}{2!}+ \frac{x^3}{3!} ... = \sum_{n = 0}^{\infty} \frac{x^n}{n!}
$$
Substitute $i\pi$:
$$
e^{i \pi} = 1 + \frac{i \theta}{1!} + \frac{(i \theta)^2}{2!} + \frac{(i \theta)^3}{3!} + ..
$$
Simplify based on $i^2 = 1, i^3 = -1, i^4 = 1...$
We see terms with even powers are real, terms with odd powers are imaginary:
$$
e^{i\theta} = (1 - \frac{\theta^2}{2!} + \frac{\theta^4}{4!} - ... ) + i (\frac{\theta^1}{1!} - \frac{\theta^3}{3!} + \frac{\theta^5}{5!} - ...)
$$
The real part is the Taylor series expansion for $cos(\theta)$.
Imaginary part is Taylor series expansion for $sin(\theta)$ 
So, $e^{i\pi} = cos(\theta) + isin(\theta)$

# Euler's Identity
Just plug $\pi$ into Euler's formula, you get $e^{i\pi} = -1 + 0 -> e^{i\pi} + 1 = 0$
# Talyor Series
This all follows from Taylor Series expansions. 
### Taylor Series Expansion
Taylor Series for a function f(x) around x=a is given by
$$
f(x) = f(a) + f'(a)(x-a) + \frac{f''(a)}{2!}(x-a)^2 + \frac{f'''(a)}{3!}(x-a)^3...
$$
#### Taylor Series Formula For $e^x$
So we apply this to $e^x$, and set $a=0$. Each derivative is 1, so:
$$
1 + x + \frac{x^2}{2!}...
$$
#### Taylor Series for sin, cos
$$
\frac{dsin(\theta)}{d\theta} = -cos(\theta) + C
$$
$$
\frac{dcos(\theta)}{d\theta} = sin(\theta) + C
$$
$$
cos(0) = 1, sin(\theta)=0
$$
If you write out the expansions, each every other term is cancelled, and the sign of the cos terms switches, as the negative sign carries through the sin derivative. 

