/**
 * IR receiver (Version 0.0.4)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#ifndef _RECEIVER_IR_H_
#define _RECEIVER_IR_H_

#include <mbed.h>

#include "RemoteIR.h"

/**
 * IR receiver class.
 */
class ReceiverIR {
public:

    /**
     * Constructor.
     *
     * @param rxpin Pin for receive IR signal.
     */
    explicit ReceiverIR(PinName rxpin);
    
    /**
     * Destructor.
     */
    ~ReceiverIR();

    /**
     * State.
     */
    typedef enum {
        Idle,
        Receiving,
        Received
    } State;
    
    /**
     * Get state.
     *
     * @return Current state.
     */
    State getState();
    
    /**
     * Get data.
     *
     * @param format Pointer to format.
     * @param buf Buffer of a data.
     * @param bitlength Bit length of the buffer.
     *
     * @return Data bit length.
     */
    int getData(RemoteIR::Format *format, uint8_t *buf, int bitlength);
    
private:
    
    typedef struct {
        RemoteIR::Format format;
        int bitcount;
        uint8_t buffer[64];
    } data_t;
    
    typedef struct {
        State state;
        int c1;
        int c2;
        int c3;
        int d1;
        int d2;
    } work_t;

    InterruptIn evt;    /**< Interrupt based input for input. */
    Timer timer;        /**< Timer for WDT. */
    Ticker ticker;      /**< Tciker for tick. */
    Timeout timeout;    /**< Timeout for tail. */

    data_t data;
    work_t work;
    
    void init_state(void);

    void isr_wdt(void);
    void isr_fall(void);
    void isr_rise(void);
    
    /**
     * ISR timeout for tail detection.
     */
    void isr_timeout(void);

};

#endif
