#ifndef __TMAC_H__
#define __TMAC_H__

typedef unsigned int uint;

/**
 * @brief Actions defined for the header of a given TMac.
 */
enum header_action {
    WRITE = 0,
    LEFT,
    RIGHT
};

/**
 * @brief Input of a transition function
 */
typedef struct tmac_dt_in {
    char symbol;
    uint state;
} tmac_intuple;

/**
 * @brief Output of a transition function
 */
typedef struct tmac_dt_out {
    header_action action;
    uint state;
} tmac_dt_out;

/**
 * @brief Struct for definition of a single transition 
 */

typedef struct tmac_dt {
    tmac_dt_in in;
    tmac_dt_out out;
} tmac_dt;

/**
 * @brief Struct for complete definition of a transition function of a TMac.
 */
typedef struct tmac_transition {
    tmac_dt* transition;
    uint size;
} tmac_transition;

/**
 * @brief Struct to centralize information about some TMac.
 */
typedef struct tmac {
    char* alphabet;
    uint size_alphabet;
    uint* states;
    uint size_states;
    uint initial_state;
    uint halting_state;
} tmac;

/**
 * @brief Creates a new transition. Should be assigned to a transition function.
 * @param s Symbol component of input.
 * @param q State component of input.
 * @param h Header action.
 * @param p State component of output.
 * @returns Pointer to new transition. If used in a tmac, it will be deallocated on tmac_destroy.
 */
tmac_dt* tmac_new_transition(char s, uint q, header_action h, uint p);

/**
 * @brief Creates a new TMac.
 * @param alphabet String of characters for the alphabet
 * @param a_size Size of alphabet array
 * @param states Array of numbers (some enumeration of states)
 * @param s_size Size of states array
 * @param initial Initial state
 * @param halting Halting state
 * @param transitions Array of transitions
 * @returns Pointer to TMac data. Returns NULL if creating failed.
 */
tmac* tmac_create(char* alphabet, uint a_size, uint* states, uint s_size, uint initial, uint halting, tmac_transition* transitions);

#endif