/**
 * IR remote common class (Version 0.0.4)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#ifndef _REMOTE_IR_H_
#define _REMOTE_IR_H_

class RemoteIR {
public:

    typedef enum {
        UNKNOWN,
        NEC,
        NEC_REPEAT,
        AEHA,
        AEHA_REPEAT,
        SONY
    } Format;

    static const int TUS_NEC = 562;
    static const int TUS_AEHA = 425;
    static const int TUS_SONY = 600;

private:
    RemoteIR();
};

#endif
