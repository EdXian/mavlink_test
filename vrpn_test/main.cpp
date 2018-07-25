#include <iostream>
#include "vrpn_Connection.h"
#include "vrpn_Tracker.h"
#include "stdio.h"
#include <time.h>
#include <unistd.h>

using namespace std;
char connectionName[128];
int  port = 3883;

void VRPN_CALLBACK handle_pos (void *, const vrpn_TRACKERCB t);


int main()
{
    cout << "vrpn_test" << endl;
    vrpn_Connection *connection;
    sprintf(connectionName,"192.168.2.30:%d", port);
    std::cout<<"Server: "<<   connectionName     <<std::endl;
    connection = vrpn_get_connection_by_name(connectionName);

    vrpn_Tracker_Remote *tracker = new vrpn_Tracker_Remote("RigidBody3", connection);

    if(connection->connected()){
        std::cout<<"connection" <<std::endl;
    }

    tracker->register_change_handler(NULL, handle_pos);
    for(int i=0;i<10;i++){
        std::cout<<" 1 sec"<<std::endl;
        usleep(1000000);
    }

    for(;; ){

        tracker->mainloop();


        connection->mainloop();
       //sleep(1);
        usleep(100000);
        timeval time;
        time = connection->get_time();

        std::cout <<"time : "<< time.tv_sec<<std::endl;
    }


    return 0;
}

void	VRPN_CALLBACK handle_pos (void *, const vrpn_TRACKERCB t)
{
    printf("Tracker Position:(%.3f,%.3f,%.3f) Orientation:(%.4f,%.4f,%.4f,%.4f)\n",
                                            t.pos[0], t.pos[1], t.pos[2],
                                            t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
}

