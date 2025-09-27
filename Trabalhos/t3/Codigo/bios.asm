; programa contido na ROM
; contém o código a executar (em modo supervisor) quando a CPU é inicializada
; deve ser colocado no endereço CPU_END_RESET (0)

; faz parecido com uma interrupção. O valor de A deve ser 0, que não é usado
;   por nenhuma interrupção; dessa forma o SO consegue distinguir entre reset
;   e interrupção.
; chama o SO e verifica o valor retornado: se for 0, é sinal que tudo está OK e
;   a execução deve continuar com o estado da CPU registrado na memória -- retorna
;   da interrupção;  se não for 0, o SO não tem o que executar e o estado na memória
;   não é válido -- suspende a execução da CPU executando a instrução PARA (o SO
;   vai ser executado novamente na próxima interrupção)

inicio_da_rom
        ; chamac chama a função C do SO passando A como argumento
        ; põe 0 em A, para garantir
        cargi 0
        chamac
        ; o valor de retorno da função chamada é colocado em A
        ; ele representa a vontade do SO de suspender a execução (o que indica que
        ;   o SO não conseguiu inicializar) ou executar o processo cujo estado da
        ;   CPU está no início da memória, onde a CPU irá pegar quando executar
        ;   RETI
        desvnz suspende
        reti
suspende
        para

; Que BIOS fajuta, nem tem código de IO!
