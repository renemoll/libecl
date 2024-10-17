# Embedded components library (ECL)

Just a place to keep components/code I re-use.

# Notes

Using:
* [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html)
* [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)

# Local development environment

You can use the provided devcontainer or setup your own.

```bash
pip install -r requirements.txt
pre-commit install --hook-type commit-msg
```

# Compiling & testing

List presets:
```bash
cmake --list-presets
```

Compiling for host:
```bash
cmake --preset host
cmake --build --preset host-debug
```

```bash
ctest --preset host
```
