#include <stddef.h>
#include "controller.h"

static float _ctrl_state[CTRL_N_STATE];
static float _ctrl_output[CTRL_N_OUTPUT];

static const float _ctrl_A[CTRL_N_STATE][CTRL_N_STATE] =
// #include "ctrlA.def"
{
    {2.1560, 15.3699, -2.9093, 80.6137},
    {-0.1825, -1.4251, 0.7728, -12.9618},
    {0.0158, 0.0768, 0.7720, 0.4937},
	{-0.0009, -0.0021, 0.0884, 0.8490},
};

static const float _ctrl_B[CTRL_N_STATE][CTRL_N_INPUT] =
{
// #include "ctrlB.def"
    {-25.6898, 2.9093, -2.2387},
    {4.0555, -0.7728, 0.8505},
    {-0.1284, 0.2280, -0.1020},
	{0.0203, -0.0884, 0.0904}, 
};

static const float _ctrl_C[CTRL_N_OUTPUT][CTRL_N_STATE] =
{
// #include "ctrlC.def"
    {0.1397, 1.8591, 0, 9.8571},
};

static const float _ctrl_D[CTRL_N_OUTPUT][CTRL_N_INPUT] =
{
// #include "ctrlD.def"
    {-3.1042, 0, 0},
};

void ctrl_init(void)
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = 0.0f;
}
    
void ctrl_set_state(const float state[CTRL_N_STATE])
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = state[i];
}

float * ctrl_get_state(void)
{
    return _ctrl_state;
}

float * ctrl_run(const float input[CTRL_N_INPUT])
{
    // y = C*x + D*u
    for (size_t i = 0; i < CTRL_N_OUTPUT; ++i)
    {
        _ctrl_output[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            _ctrl_output[i] += _ctrl_C[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            _ctrl_output[i] += _ctrl_D[i][j]*input[j];
    }

    // x_next = A*x + B*u
    float state[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
    {
        state[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            state[i] += _ctrl_A[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            state[i] += _ctrl_B[i][j]*input[j];
    }

    // x = x_next
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = state[i];

    return _ctrl_output;
}