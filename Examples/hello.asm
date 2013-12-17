# acd address:       0177566
# acd check address: 0177564
# stack start:       0200

# push: MOVW <data>, -(R6)
# pop:  MOVW (R6)+, <data>

.SECTION .DATA
    HELLO_STR: .STRING "HELLO, WORLD!"

.SECTION .CODE
.GLOBAL MAIN

MAIN:
    # init stack
    MOVW    0200, R6

    MOVW    HELLO_STR, -(R6)
    JSR     R5, (PUTS)
    HALT

PUTC:
    MOVB    2(R6), (0177566)
    WAIT_ADC:
        BITW    0200, (0177564)
        BEQ     WAIT_ADC
    RST     R5

PUTS:
    MOVW    2(R6), R0
    MOVW    R5, -(R6)
    SUB     2, R6
    PUTS_LOOP:
        TSTB    @(R0)
        BEQ     END_PUTS_LOOP
        MOVB    (R0)+, @(R6)
        JSR     R5, (PUTC)
        BR      PUTS_LOOP
    END_PUTS_LOOP:
    ADD     2, R6
    MOVW    (R6)+, R5
    RST     R5
