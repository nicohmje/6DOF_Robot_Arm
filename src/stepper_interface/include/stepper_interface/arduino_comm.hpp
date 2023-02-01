#ifndef STEPPER_ARDUINO_COMMUNICATIONS
#define STEPPER_ARDUINO_COMMUNICATIONS

#include <serial/serial.h>

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class ArduinoComm
{


public:

    ArduinoComm()
    {}


    ArduinoComm(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms)
      : serial_conn_(serial_device, baud_rate, serial::Timeout::simpleTimeout(timeout_ms))
    {  }

    void ArduinoComm::setup(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms);
    void ArduinoComm::sendEmptyMsg();
    void ArduinoComm::getMotorPos(int &val_1, int &val_2, int &val_3);
    void ArduinoComm::setJointAngles(int joint_1, int joint_2, int joint_3);
    void ArduinoComm::setPidValues(float kp, float kd, float ki, float ko);

    bool connected() const { return serial_conn_.isOpen(); }

    std::string sendMsg(const std::string &msg_to_send, bool print_output = false);



private: 

  serial::Serial serial_conn_;  ///< Underlying serial connection 


}


#endif // STEPPER_ARDUINO_COMMUNICATIONS