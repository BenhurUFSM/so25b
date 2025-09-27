; programa tratador de interrupção
; contém o código a executar (em modo supervisor) quando a CPU aceitar uma interrupção
; deve ser colocado no endereço CPU_END_TRATADOR (60)

; chama o SO e verifica o valor retornado: se for 0, é sinal que tudo está OK e
;   a execução deve continuar com o estado da CPU registrado na memória -- retorna
;   da interrupção;  se não for 0, o SO não tem o que executar e o estado na memória
;   não é válido -- suspende a execução da CPU executando a instrução PARA (o SO
;   vai ser executado novamente na próxima interrupção)

trata_int
        ; quando atende uma interrupção, a CPU salva seu estado na memória, coloca
        ;   o código da interrupção (IRQ) em A e desvia para este endereço.
        ; infelizmente, a CPU não salva o valor de X, então colocamos o X no end 59
        ;   (que parece estar livre), para que o SO consiga encontrar

salvax  define 59
        trax         ; A <=> X
        armm salvax  ; mem[59] = X
        trax         ; A <=> X
        ; chamac chama a função C do SO passando A como argumento
        chamac
        ; o valor de retorno da função chamada é colocado em A
        ; ele representa a vontade do SO de suspender a execução ou retornar
        ;   da interrupção e executar o processo cujo estado da CPU está no
        ;   início da memória
        desvnz suspende
        ; o SO deve ter colocado na memória, nos endereços CPU_END_PC etc os
        ;   valores dos registradores que serão recuperados por RETI, e deve
        ;   ter colocado o valor de X (que RETI não recupera) no endereço 59
        cargm salvax
        trax
        reti
suspende
        para
