#include "mavlink_control.h"

int sysid  = 1;  // The unique system id of this MAV, 0-127. Has to be consistent across the system
int compid_all = 0;	//Component id adressing all the components
int WRITE_FLAG = 1;
int
arm_disarm(int value)
{
    mavlink_message_t msg;
    mavlink_command_long_t cmd = {};
    cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
    cmd.target_system = sysid;
    cmd.target_component = compid_all;
    cmd.param1 = value;
    mavlink_msg_command_long_encode(sysid, compid_all, &msg, &cmd);
    int len = write_serial(msg);
}

int set_mode_send()
{
    mavlink_message_t msg;
    mavlink_command_long_t cmd = {};
    cmd.command = MAV_CMD_DO_SET_MODE;
    cmd.target_system = sysid;
    cmd.target_component = compid_all;

    cmd.param1 =0;
    cmd.param2 =393216;// PX4_CUSTOM_MODE_OFFBOARD;
	cmd.param3 = 0;
	cmd.param4 = 0;
	cmd.param5 = 0;
	cmd.param6 = 0;
	cmd.param7 = 0;
    mavlink_msg_command_long_encode(sysid, compid_all, &msg, &cmd);
	int len = write_serial(msg);
}
int
send_mocap()
{
	mavlink_att_pos_mocap_t sp;
	sp.time_usec   = 0;
	sp.q[4]        = (1,0,0,0);
        sp.x = 2.0f;
        sp.y = 3.0f;
        sp.z = 4.0f;

	// --------------------------------------------------------------------------
	//   ENCODE
	// --------------------------------------------------------------------------
	mavlink_message_t message;
	mavlink_msg_att_pos_mocap_encode(sysid, compid_all, &message, &sp);

	// --------------------------------------------------------------------------
	//   WRITE
	// --------------------------------------------------------------------------
	int len = write_serial(message);
	//printf("Sent buffer of length %i\n",len);

	// Done!
	return 0;
}

int
read_message()
{
	bool success;           // receive success flag
	bool received = false;  // receive only one message

	// Blocking wait for new data
	while (!received)
	{
		// ----------------------------------------------------------------------
		//   READ MESSAGE
		// ----------------------------------------------------------------------
		mavlink_message_t message;
		success = read_serial(message);

		// ----------------------------------------------------------------------
		//   HANDLE MESSAGE
		// ----------------------------------------------------------------------
		if( success )
		{
			// Handle Message ID
			if(message.msgid == MAVLINK_MSG_ID_HEARTBEAT){
				mavlink_heartbeat_t heartbeat;
				mavlink_msg_heartbeat_decode(&message, &heartbeat);

				printf("   custom_mode: %d\n", heartbeat.custom_mode);
				printf("   type: %d\n", heartbeat.type);
				printf("   autopilot: %d\n", heartbeat.autopilot);
				printf("   base_mode: %d\n", heartbeat.base_mode);
				printf("   system_status: %d\n", heartbeat.system_status);
				printf("   mavlink_version: %d\n", heartbeat.mavlink_version);
				received = true;
			}
		}
		// end: if read message

	}

	return 0;
}


// ------------------------------------------------------------------------------
//   Write Message
// ------------------------------------------------------------------------------
int
write_setpoint()
{

	// --------------------------------------------------------------------------
	//   PACK PAYLOAD
	// --------------------------------------------------------------------------
	mavlink_set_position_target_local_ned_t sp;
	sp.time_boot_ms     = 0;
	sp.type_mask        = 0;
	sp.target_system    = sysid;
	sp.target_component = compid_all;
	sp.coordinate_frame = MAV_FRAME_LOCAL_NED;
	sp.x = 0.0f;
	sp.y = 0.0f;
	sp.z = 0.0f;
	sp.vx = 9000.0f;
	sp.vy = 0.0f;
	sp.vz = 0.0f;
	sp.afx = 0.0f;
	sp.afy = 9000.0f;
	sp.afz = 0.0f;
	sp.yaw = 0.0f;
	sp.yaw_rate = 0.0f;

	// --------------------------------------------------------------------------
	//   ENCODE
	// --------------------------------------------------------------------------
	mavlink_message_t message;
	mavlink_msg_set_position_target_local_ned_encode(sysid, compid_all, &message, &sp);

	// --------------------------------------------------------------------------
	//   WRITE
	// --------------------------------------------------------------------------
	int len = write_serial(message);
	printf("Sent buffer of length %i\n",len);

	// Done!
	return 0;
}

int
write_toggle_offboard(float sw)
{
	// --------------------------------------------------------------------------
	//   PACK THE COMMAND MESSAGE
	// --------------------------------------------------------------------------
        mavlink_command_long_t com;

        com.param1 = sw;                              //A number > 0.5f sets offboard mode, according to MAV_CMD_NAV_GUIDED_ENABLE docs

        com.target_system = sysid;
        com.target_component = compid_all;

        com.command = MAV_CMD_NAV_GUIDED_ENABLE;

	// --------------------------------------------------------------------------
	//   ENCODE
	// --------------------------------------------------------------------------
        mavlink_message_t message;
	mavlink_msg_command_long_encode(sysid, compid_all, &message, &com);

	// --------------------------------------------------------------------------
	//   WRITE
	// --------------------------------------------------------------------------
        int len = write_serial(message);
	printf("Sent buffer of length %i\n",len);

	// Done!
	return 0;
}


// ------------------------------------------------------------------------------
//   Parse Command Line
// ------------------------------------------------------------------------------
/**
 * throws EXIT_FAILURE if could not open the port
 */
void
parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate)
{

	const char *commandline_usage = "usage: mavlink_serial -d <devicename> -b <baudrate>";
	int i;

	// Read input arguments
	for (i = 1; i < argc; i++) { // argv[0] is "mavlink"

		// Help
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("%s\n",commandline_usage);
			throw EXIT_FAILURE;
		}

		// UART device ID
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
			if (argc > i + 1) {
				uart_name = argv[i + 1];

			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// Baud rate
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0) {
			if (argc > i + 1) {
				baudrate = atoi(argv[i + 1]);

			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

	}
	// end: for each input argument

	// Done!
	return;
}

// ------------------------------------------------------------------------------
// Handle CTRL-C quit signals
// ------------------------------------------------------------------------------
void quit_handler(int sig)
{
        printf("Exiting on user request.\n");
        WRITE_FLAG = 0;
}

