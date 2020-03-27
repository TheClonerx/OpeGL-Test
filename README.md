## Building

```bash
git submodule init
git submodule update --recursive
cmake -S . -B build/
cmake --build build/ --target main --parallel
```