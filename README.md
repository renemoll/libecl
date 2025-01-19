# Embedded components library (ECL)

Just a place to keep components/code I re-use.

## Notes

Using:

* [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html)
* [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)

## Local development environment

You can use the provided devcontainer or setup your own.

```bash
pip install -r requirements.txt
pre-commit install --hook-type commit-msg
```

## Compiling & testing

This project provides CMake workflows to easy building from source code.

List available workflows:

```bash
cmake --workflow --list-presets
```

Workflow | Command
--- | ---
Unit tests | ```cmake --workflow --preset unittest```
Test coverage | ```cmake --workflow --preset coverage```
