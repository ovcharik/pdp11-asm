# cdr rcu: 0177520
# cdr rdb: 0177522

# stack start: 0600

# push: MOVW <data>, -(R6)
# pop:  MOVW (R6)+, <data>

.SECTION .DATA

.SECTION .CODE
.GLOBAL MAIN

MAIN:
    MOVW    0600, R6

    JSR     R5, (CLEAR_CDR)

    # left red triangle
    MOVW      1, -(R6)
    MOVW     84, -(R6)
    MOVW     85, -(R6)
    MOVW     84, -(R6)
    MOVW      0, -(R6)
    JSR      R5, (DRAW_TRI)
    ADD      10, R6

    # middle blue rect
    MOVW      2, -(R6)
    MOVW     85, -(R6)
    MOVW     85, -(R6)
    MOVW     86, -(R6)
    MOVW     84, -(R6)
    JSR      R5, (DRAW_RECT)
    ADD      10, R6

    # right green triangle
    MOVW      3, -(R6)
    MOVW    171, -(R6)
    MOVW     85, -(R6)
    MOVW     84, -(R6)
    MOVW      1, -(R6)
    JSR      R5, (DRAW_TRI)
    ADD      10, R6

    HALT

CLEAR_CDR:
    MOVW    0140, (0177520)
    MOVW    0001, (0177520)
    RST     R5

# params (word color = {0..3}, word x, word y)
PUT_PX:
    MOVB    2(R6), (0177523)
    MOVB    4(R6), (0177522)
    MOVW    6(R6), (0177520)
    RST     R5

# params (word color = {0..3}, word x, word y, word width, word heihgt)
DRAW_RECT:
    MOVW    R5, -(R6)

    MOVW    12(R6), R4

    MOVW     4(R6), R1
    MOVW     8(R6), R3
    RECT_HEIGHT_LOOP:
        MOVW     6(R6), R0
        MOVW    10(R6), R2
        RECT_WIDTH_LOOP:
            
            MOVW    R4, -(R6)
            MOVW    R2, -(R6)
            MOVW    R3, -(R6)
            JSR     R5, (PUT_PX)
            ADD     6, R6

            INCW    R2
        SOB     R0, RECT_WIDTH_LOOP
        INCW    R3
    SOB     R1, RECT_HEIGHT_LOOP

    MOVW    (R6)+, R5
    RST     R5

# params (word color = {0..3}, word x, word y, word size, word left_or_right = {0..1})
DRAW_TRI:
    MOVW    R5, -(R6)
    
    MOVW    12(R6), R4

    MOVW    6(R6), R1
    MOVW    8(R6), R3
    TRI_HEIGHT_LOOP:
        MOVW    6(R6), R0
        SUB     R1, R0
        INCW    R0

        MOVW    10(R6), R2
        TRI_WIDTH_LOOP:
            
            MOVW    R4, -(R6)
            MOVW    R2, -(R6)
            MOVW    R3, -(R6)
            JSR     R5, (PUT_PX)
            ADD     6, R6

            TSTW    4(R6)
            BEQ     TRI_X_DEC
                INCW    R2
                BR      TRI_X_INC
            TRI_X_DEC:
                DECW    R2
            TRI_X_INC:
        SOB     R0, TRI_WIDTH_LOOP
        INCW    R3
    SOB     R1, TRI_HEIGHT_LOOP

    MOVW    (R6)+, R5
    RST     R5   