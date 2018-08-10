/**
 * IR transmitter (Version 0.0.4)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#include "TransmitterIR.h"

#define LOCK()
#define UNLOCK()

/**
 * Constructor.
 *
 * @param txpin Pin for transmit IR signal.
 */
TransmitterIR::TransmitterIR(PinName txpin) : tx(txpin) {
    tx.write(0.0);
    tx.period_us(26.3);

    work.state = Idle;
    work.bitcount = 0;
    work.leader = 0;
    work.data = 0;
    work.trailer = 0;

    data.format = RemoteIR::UNKNOWN;
    data.bitlength = 0;
}

/**
 * Destructor.
 */
TransmitterIR::~TransmitterIR() {
}

/**
 * Get state.
 *
 * @return Current state.
 */
TransmitterIR::State TransmitterIR::getState(void) {
    LOCK();
    State s = work.state;
    UNLOCK();
    return s;
}

/**
 * Set data.
 *
 * @param format Format.
 * @param buf Buffer of a data.
 * @param bitlength Bit length of the data.
 *
 * @return Data bit length.
 */
int TransmitterIR::setData(RemoteIR::Format format, uint8_t *buf, int bitlength) {
    LOCK();
    if (work.state != Idle) {
        UNLOCK();
        return -1;
    }

    work.state = Leader;
    work.bitcount = 0;
    work.leader = 0;
    work.data = 0;
    work.trailer = 0;

    data.format = format;
    data.bitlength = bitlength;
    const int n = bitlength / 8 + (((bitlength % 8) != 0) ? 1 : 0);
    for (int i = 0; i < n; i++) {
        data.buffer[i] = buf[i];
    }

    switch (format) {
        case RemoteIR::NEC:
            ticker.detach();
            ticker.attach_us(this, &TransmitterIR::tick, RemoteIR::TUS_NEC);
            break;
        case RemoteIR::AEHA:
            ticker.detach();
            ticker.attach_us(this, &TransmitterIR::tick, RemoteIR::TUS_AEHA);
            break;
        case RemoteIR::SONY:
            ticker.detach();
            ticker.attach_us(this, &TransmitterIR::tick, RemoteIR::TUS_SONY);
            break;
    }

    UNLOCK();
    return bitlength;
}

void TransmitterIR::tick(void) {
    LOCK();
    switch (work.state) {
        case Idle:
            work.bitcount = 0;
            work.leader = 0;
            work.data = 0;
            work.trailer = 0;
            break;
        case Leader:
            if (data.format == RemoteIR::NEC) {
                /*
                 * NEC.
                 */
                static const int LEADER_NEC_HEAD = 16;
                static const int LEADER_NEC_TAIL = 8;
                if (work.leader < LEADER_NEC_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.leader++;
                if ((LEADER_NEC_HEAD + LEADER_NEC_TAIL) <= work.leader) {
                    work.state = Data;
                }
            } else if (data.format == RemoteIR::AEHA) {
                /*
                 * AEHA.
                 */
                static const int LEADER_AEHA_HEAD = 8;
                static const int LEADER_AEHA_TAIL = 4;
                if (work.leader < LEADER_AEHA_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.leader++;
                if ((LEADER_AEHA_HEAD + LEADER_AEHA_TAIL) <= work.leader) {
                    work.state = Data;
                }
            } else if (data.format == RemoteIR::SONY) {
                /*
                 * SONY.
                 */
                static const int LEADER_SONY_HEAD = 4;
                static const int LEADER_SONY_TAIL = 0;
                if (work.leader < LEADER_SONY_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.leader++;
                if ((LEADER_SONY_HEAD + LEADER_SONY_TAIL) <= work.leader) {
                    work.state = Data;
                }
            } else {
            }
            break;
        case Data:
            if (data.format == RemoteIR::NEC) {
                /*
                 * NEC.
                 */
                if (work.data == 0) {
                    tx.write(0.5);
                    work.data++;
                } else {
                    tx.write(0.0);
                    if (0 != (data.buffer[work.bitcount / 8] & (1 << work.bitcount % 8))) {
                        if (3 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    } else {
                        if (1 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    }
                }
                if (data.bitlength <= work.bitcount) {
                    work.state = Trailer;
                }
            } else if (data.format == RemoteIR::AEHA) {
                /*
                 * AEHA.
                 */
                if (work.data == 0) {
                    tx.write(0.5);
                    work.data++;
                } else {
                    tx.write(0.0);
                    if (0 != (data.buffer[work.bitcount / 8] & (1 << work.bitcount % 8))) {
                        if (3 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    } else {
                        if (1 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    }
                }
                if (data.bitlength <= work.bitcount) {
                    work.state = Trailer;
                }
            } else if (data.format == RemoteIR::SONY) {
                /*
                 * SONY.
                 */
                if (work.data == 0) {
                    tx.write(0.0);
                    work.data++;
                } else {
                    tx.write(0.5);
                    if (0 != (data.buffer[work.bitcount / 8] & (1 << work.bitcount % 8))) {
                        if (2 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    } else {
                        if (1 <= work.data) {
                            work.bitcount++;
                            work.data = 0;
                        } else {
                            work.data++;
                        }
                    }
                }
                if (data.bitlength <= work.bitcount) {
                    work.state = Trailer;
                }
            } else {
            }
            break;
        case Trailer:
            if (data.format == RemoteIR::NEC) {
                /*
                 * NEC.
                 */
                static const int TRAILER_NEC_HEAD = 1;
                static const int TRAILER_NEC_TAIL = 2;
                if (work.trailer < TRAILER_NEC_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.trailer++;
                if ((TRAILER_NEC_HEAD + TRAILER_NEC_TAIL) <= work.trailer) {
                    work.state = Idle;
                    //ticker.detach();
                }
            } else if (data.format == RemoteIR::AEHA) {
                /*
                 * AEHA.
                 */
                static const int TRAILER_AEHA_HEAD = 1;
                static const int TRAILER_AEHA_TAIL = 8000 / RemoteIR::TUS_AEHA;
                if (work.trailer < TRAILER_AEHA_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.trailer++;
                if ((TRAILER_AEHA_HEAD + TRAILER_AEHA_TAIL) <= work.trailer) {
                    work.state = Idle;
                    //ticker.detach();
                }
            } else if (data.format == RemoteIR::SONY) {
                /*
                 * SONY.
                 */
                static const int TRAILER_SONY_HEAD = 0;
                static const int TRAILER_SONY_TAIL = 0;
                if (work.trailer < TRAILER_SONY_HEAD) {
                    tx.write(0.5);
                } else {
                    tx.write(0.0);
                }
                work.trailer++;
                if ((TRAILER_SONY_HEAD + TRAILER_SONY_TAIL) <= work.trailer) {
                    work.state = Idle;
                    //ticker.detach();
                }
            } else {
            }
            break;
        default:
            break;
    }
    UNLOCK();
}
