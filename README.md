# kdl
Kestel Detection Language - A DSL designed for detecting malware and attacks

IMPORTANT: This project is NOT complete. It is still in development.

## Features
This project is a sort of spin-off from YARA. It can compile to a shared library or driver
that can be loaded and run.

## Basic Syntax

```yara

rule rulename{

	content:
		$stringvar = "this is a string"
		$bytevar = | 74 68 69 73 20 69 73 20 61 20 62 79 74 65 20 73 65 71 75 65 6e 63 65 |
		$unsupportedregexvar = /hi! I am unsupported!/

	conditions:
		all of them
}
```

