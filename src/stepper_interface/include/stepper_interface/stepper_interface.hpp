#ifndef STEPPER_INTERFACE_HPP
#define STEPPER_INTERFACE_HPP


#include <memory>
#include <string>
#include <vector>
#include "arduino_comm.hpp"

#include <serial/serial.h>

#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/logger.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "stepper_interface/visibility_control.h"



struct Parameters
{
  std::string device = "/dev/ttyUSB0"; //sometimes ttyUSB1
  int baud_rate = 1000000; //1 MHz
  int timeout = 1000; //ms
  std::vector<double> s_p_a;

};




namespace stepper_interface 
{

// using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class StepperInterface : public hardware_interface::SystemInterface
{

public: 
    RCLCPP_SHARED_PTR_DEFINITIONS(StepperInterface)

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;

    STEPPER_INTERFACE_PUBLIC
    std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

    STEPPER_INTERFACE_PUBLIC
    std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

    STEPPER_INTERFACE_PUBLIC
    hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

    STEPPER_INTERFACE_PUBLIC
    double calcSteps(double , int );

    STEPPER_INTERFACE_PUBLIC
    double calcAngles(int , int );






private:

    Parameters param_; 
    ArduinoComm arduino_; 
    double hw_start_sec_;

    std::vector<double> hw_commands_;
    std::vector<double> hw_states_;

    std::vector<int> steps_;
    std::vector<double> angles_;
    std::vector<double> pos_;

}; 

}; //namespace stepper_interface

#endif //STEPPER_INTERFACE_HPP
