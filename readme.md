Xadrez independente de plataforma
> Para suportar uma nota plataforma (circuito embarcado, SO, etc), somente é nescessário implementar a interface user_interface.h para aquela plataforma


Como Compilar
Use o Makefile para construir o executável. Por padrão, ele compilará a interface de terminal:
```make```

Se desejar compilar uma interface específica (exemplo: windows ou embedded), utilize a variável UI_IMPLEMENTATION, utilizando o nome da pasta da implementação:
```make UI_IMPLEMENTATION=linux_terminal```

Execução
Após a compilação, o binário estará disponível em bin/chess.
```./bin/chess```
