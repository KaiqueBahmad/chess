Xadrez independente de plataforma

Como Compilar
Use o Makefile para construir o executável. Por padrão, ele compilará a interface de terminal:
```make```

Se desejar compilar uma interface específica (exemplo: windows ou embedded), utilize a variável UI_TYPE:
```make UI_TYPE=windows```

Execução
Após a compilação, o binário estará disponível em bin/chess.
```./bin/chess```
