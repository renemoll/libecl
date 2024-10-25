# set_target_properties(${target}
# 	PROPERTIES
# 		DEBUG_POSTFIX "d"
# 		RELWITHDEBINFO_POSTFIX "rd"
# )

-Wdisabled-optimization # warn when the compiler disables an optimization pass
-Wvector-operation-performance # warn if vector operations are not supported in the platform


-fstack-protector-strong
-fstack-clash-protection
-fcf-protection
-fomit-frame-pointer
-Wstack-protector: Warn when not issuing stack smashing protection for some reason

Sanitizers
-fsanitize=undefined
-fno-sanitize-recover=undefined
-fsanitize-minimal-runtime
-fanalyzer

-Wformat-overflow=2: check for *printf overflow
-Wformat-security: check for dangerous format specifiers in *printf (enabled by -Wformat=2)
-Wformat-signedness:

https://airbus-seclab.github.io/c-compiler-security/gcc_compilation.html
https://airbus-seclab.github.io/c-compiler-security/clang_compilation.html
