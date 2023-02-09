from setuptools import Extension, setup

setup(name='kawpow',
    version='0.0.1',
    ext_modules=[
        Extension(
            'kawpow', [
                "src/ethash/ethash.cpp",
                "src/ethash/primes.cpp",
                "src/ethash/progpow.cpp",
                "src/keccak/keccak.cpp",
                "src/keccak/keccakf800.cpp",
                "src/keccak/keccakf1600.cpp",
                "src/uint256.cpp",
                "src/utilstrencodings.cpp",
                'hasherkawpow_py.cc'
                ],
            include_dirs=["src", "src/include"],
            language="c++",
            extra_compile_args=['-std=c++0x']
            )],
    )
