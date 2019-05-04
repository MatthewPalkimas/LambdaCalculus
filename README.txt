~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ README ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Work of Matthew Palkimas for ECE511 Computability, Complexity, and Algorithms

This language is defined as:
variable    ::= letter{number}
abstraction ::= #variable.lambda
application ::= (lambda lambda)
lambda 	    ::= variable
	   	|abstraction
	   	|application

Examples to try:

example from rabbit
(#x.(x (x y)) #a.(a a))

example from rabbit but to show numbers work
(#x1.(x1 (x1 y337)) #a50.(a50 a50))

first example we did in class
#y2.(#x.(y2 x) z)

increment where in this case it is incrementing N3 to N4
(#n.#f.#x.(f((n f)x)) #f.#x.(f(f(f x))))

//SIDE NOTE
//N0 being #f.#x.x
//N1 being #f.#x.(f x)
//N2 being #f.#x.(f(f x)) etc...

Y the infinite function (have it stop after number of iterations)
(#f.(#x.(f (x x)) #x.(f(x x))) a)

Case for True
(((#x.x #x.#y.x) a) b)

Case for False
(((#x.x #x.#y.y) a) b)

Test for Zero with input being N0
(#n.((n #x.#x.#y.y) #x.#y.x) #f.#x.x)

Test for Zero with input being N3
(#n.((n #x.#x.#y.y) #x.#y.x) #f.#x.(f(f(f x))))

WORKS FOR ALL THESE CASES! ANY OTHERS YOU CAN COME UP WITH TOO I ASSUME!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ README ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
