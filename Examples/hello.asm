# acd address:       0177566
# acd check address: 0177564

.SECTION .DATA
    HELLO_STR: .STRING "HELLO, WORLD!"


.SECTION .CODE
.GLOBAL MAIN

MAIN:
    MOVW    HELLO_STR, R0
    MOVW    13,        R1
    LOOP:
        TSTB    @(R0)
        BEQ     END_LOOP

        MOVB    (R0)+, (0177566)
        ADC_LOOP:
            BITW    0200, (0177564)
            BEQ     ADC_LOOP

        SOB     R1, LOOP
    END_LOOP:
        HALT
