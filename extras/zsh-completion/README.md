# lttng Zsh completion scripts

## Installing

Put the files in any location listed in the `fpath` variable.

Note: you need to run `compinit` after modifying `fpath`.


## Styling

Suggested styles for these completion scripts.

### Colored tracing session statuses

![](http://ss.0x3b.org/pseudoamateurism20.png)

    zstyle ':completion:*:tracing-sessions' list-colors '=(#b)*\[(inactive)\]=0=31' '=(#b)*\[(active)\]=0=32'
