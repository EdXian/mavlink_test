#include "iostream"
#include "mavlink_control.h"

// https://mavlink.io/en/messages/common.html
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
    read_message();

    std::cout<<"SETTING TO OFFBOARD\n"<<std::endl;
    write_toggle_offboard(1.0);
    std::cout<<"OFFBOARD ENABLED\n"<<std::endl;

    int i=0;
    int count =0;
    bool armed = false;
    while(1)
        {
            //read_message();
            //usleep(250000);
                    if(i>30){
                            i=0;
                           read_message();
                            usleep(25000);
                            if(!armed){

                             arm_disarm(1);
                             armed = true;
                            }

                count++;
                    }

            write_setpoint();
            usleep(250000);
            i++;
            if(count >5){

              break;
            }
        }
        write_toggle_offboard(0.0);

    //
        printf(" \n");
        arm_disarm(0);
        read_message();
        usleep(25000);
        std::cout<<"SETTING BACK FROM OFFBOARD\n"<<std::endl;
        std::cout<<"CLOSE PORT\n"<<std::endl;
        close_serial();
return 0;
}

