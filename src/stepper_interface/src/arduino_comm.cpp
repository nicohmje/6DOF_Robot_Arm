#include "stepper_interface/arduino_comm.hpp"
#include <rclcpp/rclcpp.hpp>
#include <sstream>
#include <cstdlib>

// MESSAGE OBJECTS TO SEND:
// p : get motor angles 1, 2, 3
// j : set joint angles 1, 2, 3
// k : set pid values kp,ki,kd,ko

std::string ArduinoComm::sendMsg(const std::string &msg)
{   
    serial_conn_.write(msg);
    std::string response = serial_conn_.readline();

    return response;
}


void ArduinoComm::setup(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms)
{  
    serial_conn_.setPort(serial_device);
    serial_conn_.setBaudrate(baud_rate);
    serial::Timeout tt = serial::Timeout::simpleTimeout(timeout_ms);
    serial_conn_.setTimeout(tt); // This should be inline except setTimeout takes a reference and so needs a variable
    serial_conn_.open();
    // serial_conn_.(serial_device, baud_rate, serial::Timeout::simpleTimeout(timeout_ms));

}

void ArduinoComm::sendEmptyMsg()
{
    std::string response = sendMsg("\r");
}



void ArduinoComm::getMotorPos(int &val_1, int &val_2, int &val_3)
{
    std::string response = sendMsg("p\r");

    std::string delimiter = " ";
    size_t first_del = response.find(delimiter);
    std::string token_1 = response.substr(0, first_del);
    std::string token_2 = response.substr(first_del + delimiter.length());
    size_t second_del = response.find(delimiter, first_del + delimiter.length());
    std::string token_3 = response.substr(second_del + delimiter.length());


    val_1 = std::atoi(token_1.c_str());
    val_2 = std::atoi(token_2.c_str());
    val_3 = std::atoi(token_3.c_str());
}

void ArduinoComm::setJointAngles(int joint_1, int joint_2, int joint_3)
{
    std::stringstream ss;
    ss << "j " << joint_1 << " " << joint_2 << " " << joint_3 << "\r";
    sendMsg(ss.str());

}

void ArduinoComm::setPidValues(float kp, float kd, float ki, float ko)
{
    std::stringstream ss;
    ss << "k " << kp << ":" << kd << ":" << ki << ":" << ko << "\r";
    sendMsg(ss.str());
}












