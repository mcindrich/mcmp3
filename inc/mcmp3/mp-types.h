#ifndef _MCMP3_TYPES_H
#define _MCMP3_TYPES_H

typedef enum {
    mp_signal_t_unknown,
    mp_signal_t_none,
    mp_signal_t_play,
    mp_signal_t_pause,
    mp_signal_t_stop,
    mp_signal_t_change_song,
    mp_signal_t_end_connection,
    mp_signal_t_close_server
} mp_signal_t;

#endif