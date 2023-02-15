// Copyright 2023 Nicolas Hammje
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#include "stepper_interface/stepper_interface.hpp"

#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include <typeinfo>

#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"


namespace stepper_interface
{
hardware_interface::CallbackReturn StepperInterface::on_init(
    const hardware_interface::HardwareInfo & info)
{
    if (
        hardware_interface::SystemInterface::on_init(info) !=
        hardware_interface::CallbackReturn::SUCCESS)
    {
        return hardware_interface::CallbackReturn::ERROR;
    }

    steps_.resize(info_.joints.size());
    pos_.resize(info_.joints.size());
    hw_states_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());

  for (const hardware_interface::ComponentInfo & joint : info_.joints)
  {
    if (joint.command_interfaces.size() != 1)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("StepperInterface"),
        "Joint '%s' has %zu command interfaces found. 1 expected.", joint.name.c_str(),
        joint.command_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.command_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("StepperInterface"),
        "Joint '%s' have %s command interfaces found. '%s' expected.", joint.name.c_str(),
        joint.command_interfaces[0].name.c_str(), hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces.size() != 1)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("StepperInterface"),
        "Joint '%s' has %zu state interface. 1 expected.", joint.name.c_str(),
        joint.state_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("StepperInterface"),
        "Joint '%s' have %s state interface. '%s' expected.", joint.name.c_str(),
        joint.state_interfaces[0].name.c_str(), hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }
  }

    return hardware_interface::CallbackReturn::SUCCESS;

}



hardware_interface::CallbackReturn StepperInterface::on_configure(
    const rclcpp_lifecycle::State & /* previous_state */ )
{
    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Configuring...");

    param_.device = info_.hardware_parameters.at("device_port");

    param_.baud_rate = std::stoi(info_.hardware_parameters["baud_rate"]);
    param_.timeout = std::stoi(info_.hardware_parameters["timeout"]);

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Set device parameters... %s", param_.device.c_str());


    // info_.joints[1].name = "base";
    // info_.joints[2].name = "shoulder";
    // info_.joints[3].name = "elbow";

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Configured joint info...");

    param_.s_p_a = {int(13200/(2*3.14159)), int(8000/(2*3.14159)), int(13200/(2*3.14159))};

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Configured steps per angle for 3 joints...");


    arduino_.setup(param_.device, param_.baud_rate, param_.timeout);

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Finished Configuration");

    return hardware_interface::CallbackReturn::SUCCESS;

}




std::vector<hardware_interface::StateInterface>
StepperInterface::export_state_interfaces() 
{
  std::vector<hardware_interface::StateInterface> state_interfaces;
  for (uint i = 0; i < 3; i++)
  {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
      info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_states_[i]));
  }

    // NB: NO VELOCITY INTERFACE ATM FOR THE STEPPERS
    // WIP 



  return state_interfaces;
}




std::vector<hardware_interface::CommandInterface>
StepperInterface::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  for (uint i = 0; i < 3; i++)
  {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
      info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_commands_[i]));
  }

  return command_interfaces;
}



hardware_interface::CallbackReturn StepperInterface::on_activate(
    const rclcpp_lifecycle::State & /*previous_state*/
)
{
    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Activating ...please wait...");

    arduino_.sendEmptyMsg();

    //Check PID values
    arduino_.setPidValues(30, 20, 0, 100);

    // Commands and states should be the same size
    for (uint i = 0; i < hw_states_.size(); i++)
    {
    hw_commands_[i] = hw_states_[i];
    }

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Activation succesful");

    // status_ = hardware_interface::status::STARTED;
    return hardware_interface::CallbackReturn::SUCCESS;
}




hardware_interface::CallbackReturn StepperInterface::on_deactivate(
    const rclcpp_lifecycle::State & /*previous_state*/
)
{
    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Deactivating ...please wait...");

    RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Deactivation succesful");

    // status_ = hardware_interface::status::STOPPED;
    return hardware_interface::CallbackReturn::SUCCESS;
}


double StepperInterface::calcSteps(double angle, int number) //DONE
{
    //Converts angles into steps 
    //1 : base, 2:shoulder, 3:elbow
    int val = int(angle * param_.s_p_a[number]);
    if (angle != angle)
    { 
      val = 0;
    }
    return val;
}

double StepperInterface::calcAngles(int steps, int number) //DONE
{
    //Converts steps into angles 
    //1: base, 2: shoulder, 3: elbow

    return int(steps / param_.s_p_a[number]);
}



hardware_interface::return_type StepperInterface::read(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/
)
{
    if (!arduino_.connected())
    {
      RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Arduino es no connectado");
      return hardware_interface::return_type::ERROR;
    }

    // RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Arduino es connectado");


    arduino_.getMotorPos(steps_[1], steps_[2], steps_[3]);
    //RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "steps1  %i.", steps_[1]);
    for (uint i = 0; i < 3; i++)
    {
        hw_states_[i] = calcAngles(steps_[i], i);
        RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "steps1  %i.", steps_[i]);
        RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "steps1  %f.", calcAngles(steps_[i], i));
    }

    return hardware_interface::return_type::OK;

}



hardware_interface::return_type StepperInterface::write(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/
)
{
    if (!arduino_.connected())
    {
      RCLCPP_INFO(rclcpp::get_logger("StepperInterface"), "Arduino es no connectado");
      return hardware_interface::return_type::ERROR;
    }

    for (uint i = 0; i < 3; i++)
    {
        pos_[i] = calcSteps(hw_commands_[i], i);
    }
    
    //IMPLEMENT LIMITS 

    arduino_.setJointAngles(pos_[1], pos_[2], pos_[3]);

    return hardware_interface::return_type::OK;

}

} //namespace stepper_interface

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  stepper_interface::StepperInterface, hardware_interface::SystemInterface)












