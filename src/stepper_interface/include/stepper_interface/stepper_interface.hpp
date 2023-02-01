#ifndef STEPPER_INTERFACE
#define STEPPER_INTERFACE

#include "hardware_interface/system_interface.hpp"
#include "arduino_comm.hpp"

namespace stepper_interface 
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class StepperInterface : public hardware_interface::SystemInterface
{

public: 

    StepperInterface();

    CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

    std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

    std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

    CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

    CallbackReturn read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

    CallbackReturn write(const rclcpp::Time & time, const rclcpp::Duration & period) override;









private:





} 

} //namespace stepper_interface

#endif //STEPPER_INTERFACE