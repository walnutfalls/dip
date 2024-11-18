Input layer is nodes 0...N. Like in other ML algorithms, the input vectors $X = {x_1, x_2, x_3...}$ are augmented with $x_0 = 1$. 

In each hidden layer, there is a Bias node, which is analogous to the $x_0=1$ trick.

Let $f$ be the *activation function*. It introduces non-linearity to the output (otherwise, it is just a linear combination, and we're basically doing linear regression). It can be the sigmoid function, tanh, ReLU or something else.

$\sigma(x) = \frac{1}{1 + e^{-x}}$
$tanh(x) = 2\sigma(2x) - 1$ a "sharper" sigmoid
$ReLU(x) = max(0,x)$

Given a Hidden Layer node, there  are weights $w_0, ... , w_n$. The output of the node is

$f(w_0x_0 + w_1x_1 ... w_nx_n) = f(w \cdot x)$

Suppose we have an input layer of values $a_0 = {a_{01}, a_{02}, a_{03}...}$ . The next layer can be written as $a_1 = a_{11}, a_{12}...$ . Each neuron has incoming weights from all the previous layer neurons, and a bias. This can all be expressed as a matrix multiplication $a_1 = Wa_0$ . We also account for the bias, which is a vector $b$, and we get $a_1 = Wa_0 + b$ . Then wrap it in the activation function: 
$$
\huge{
	a_1 = f(Wa_0 + b)
}
$$

## Back Propagation
When you have an output vector, $\tilde{y}$ from the Output Layer, get the *cost*:
$\sum_{k=1}^{n} (y_k - \tilde{y}_k)^2)$

Then, get the average cost for all of the training set.








