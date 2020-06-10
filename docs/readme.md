# 编译文档
文档由`Documenter.jl`编译，可在项目的根目录（`fate-grand-rider`）用
```shell
julia --project=docs/ -e 'using Pkg; Pkg.instantiate()'
julia --project=docs/ docs/make.jl
```
进行生成（由于各种原因第二行可能失败，若失败则再重复一次），也可直接访问[![docs](https://img.shields.io/badge/docs-dev-blue.svg)](https://melonedo.github.io/fate-grand-rider/dev)。
