# ! Development is currently stopped, because of working on AST-Walker version !

<p align="center">
	<img src="img/Yocto.png" width="250">	
</p>

# Yocto

10⁻²⁴ size programming language written in pure C++.

Yocto implementation with bytecode compilation and VM.

# Architecture
- Everything is an Object, primitives are objects too and all operators implemented as magic functions
- The compiler always inside a function. The top-level code is always in some kinda `main()` function

