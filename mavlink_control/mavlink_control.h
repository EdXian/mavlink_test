

// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string.h>
#include <inttypes.h>
#include <fstream>
#include <signal.h>

using std::string;
using namespace std;
#include "serial_port.h"

#include <common/mavlink.h>


// ------------------------------------------------------------------------------
//   Prototypes
// ------------------------------------------------------------------------------
void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate);

int read_message();
int write_setpoint();
int write_toggle_offboard(float sw);
void quit_handler(int sig);
int arm_disarm(int i);
int set_mode_send();
int send_mocap();



