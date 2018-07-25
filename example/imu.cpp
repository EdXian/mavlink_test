#include "iostream"
#include "mavlink_control.h"

bool read_imu_message(){
    bool success;
    bool received = false;
    while (!received)
    {
        mavlink_message_t message;
        success = read_serial(message);
        if( success )
        {
            // Handle Message ID
            if(message.msgid == MAVLINK_MSG_ID_HIGHRES_IMU){
                mavlink_highres_imu_t imu_data;
                mavlink_msg_highres_imu_decode(&message, &imu_data);

                printf("system_id : %d\n",  message.sysid);
                printf("component_id : %d\n",  message.compid);
                printf("acc :x = %f , y = %f ,z = %f\n",imu_data.xacc , imu_data.yacc , imu_data.zacc);
                printf("gyro :x = %f , y = %f ,z = %f\n",imu_data.xgyro , imu_data.ygyro , imu_data.zgyro);
                printf("mag :x = %f , y = %f ,z = %f\n",imu_data.xmag , imu_data.ymag , imu_data.zmag);
                printf("pressure: %f\n", imu_data.diff_pressure);
                printf("time: %ld\n", imu_data.time_usec);
                printf("temperature: %f\n",imu_data.temperature);
                received = true;
            }
        }
        // end: if read message

    }
    return success;
}



int main(int argc, char **argv){

    char *uart_name = (char*)"/dev/ttyACM0";
    int baudrate = 921600;

    bool failure;
    std::cout<<"mavlink_test"<<std::endl;
    try
    {
        parse_commandline(argc, argv, uart_name, baudrate);
    }

    catch (int failure)
    {
        return failure;
    }
    std::cout<<"open port\n"<<std::endl;


    try {
        open_serial(uart_name, baudrate);
    }
    catch (int failure)
    {
        return failure;
    }

    signal(SIGINT, quit_handler);
    std::cout<<"received data from mavlink: "<<std::endl;
    std::cout<<"=========================== "<<std::endl;
    for(int i =0; i<10; i++){
        printf("=================%d====================\n",i+1);
        read_imu_message();
        sleep(1);
    }

    std::cout<<"SETTING BACK FROM OFFBOARD\n"<<std::endl;
    std::cout<<"CLOSE PORT\n"<<std::endl;
    close_serial();
return 0;
}
