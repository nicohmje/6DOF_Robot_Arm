import os


from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, Command, FindExecutable
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

import xacro


from launch.actions import RegisterEventHandler, IncludeLaunchDescription, ExecuteProcess
from launch.event_handlers import (OnProcessStart, OnProcessExit)

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():


    load_jsb2 = Node(
	    package="controller_manager",
	    executable="spawner",
	    arguments=["joint_state_broadcaster"],
	)

    load_hand2 = Node(
		    package="controller_manager",
		    executable="spawner",
		    arguments=["hand_group_controller"],
		)
    load_arm2 = Node(
		    package="controller_manager",
		    executable="spawner",
		    arguments=["arm_group_controller"],
		)
		
	
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py'])
            )
    
    package_path = os.path.join(
		get_package_share_directory('nick_bot'))
		
    xacro_file = os.path.join(package_path, 
					'urdf',
					'nick_bot.urdf')
					
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)
    params = {'robot_description' : doc.toxml()}

    rsp = Node(
		package='robot_state_publisher',
		executable='robot_state_publisher',
		output='screen',
		parameters = [params],
	    )

            
    
    spawn_entity = Node(
		package='gazebo_ros',
		executable='spawn_entity.py',
		arguments=['-topic', '/robot_description', '-entity', 'nick_bot'],
		output='screen'
	    )

		
		
		
    return LaunchDescription([
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=spawn_entity,
                on_exit=[load_jsb2]
            )
        ),
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=load_jsb2,
                on_exit=[load_arm2]
            )
        ),
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=load_arm2,
                on_exit=[load_hand2]
            )
        ),
        gazebo,
        rsp,
        spawn_entity,
    ])
