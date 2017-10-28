#include "ros/ros.h"
#include "ros/subscribe_options.h"
#include "geometry_msgs/Twist.h"

#include <gazebo/common/Plugin.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>

using namespace ros;
using namespace std;

namespace gazebo {
class BattlebotsPlugin : public ModelPlugin {
private:
    physics::ModelPtr model;
    physics::JointPtr joint1, joint2, joint3;
    common::PID pid;

    NodeHandle rosNode;
    Subscriber rosSub;

public:
    BattlebotsPlugin() {}

    void SetVelocityToJoints(double vel)
    {
        model->GetJointController()->SetVelocityTarget(joint1->GetScopedName(), vel);
        model->GetJointController()->SetVelocityTarget(joint2->GetScopedName(), vel);
        model->GetJointController()->SetVelocityTarget(joint3->GetScopedName(), vel);
    }

    void OnRosMsg(const geometry_msgs::TwistConstPtr& _msg)
    {
        SetVelocityToJoints(_msg->angular.z);

        double speed = -_msg->linear.x;

        math::Quaternion r = model->GetWorldPose().rot;
        double angle = r.GetYaw();
        model->SetLinearVel(math::Vector3(speed * cos(angle), speed * sin(angle), 0));
        model->SetAngularVel(math::Vector3(0, 0, _msg->angular.z));
    }

    virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
        model = _model;
        joint1 = _model->GetJoints()[0];
        joint2 = _model->GetJoints()[1];
        joint3 = _model->GetJoints()[2];

        pid = common::PID(0.2, 0, 0);

        model->GetJointController()->SetVelocityPID(joint1->GetScopedName(), pid);
        model->GetJointController()->SetVelocityPID(joint2->GetScopedName(), pid);
        model->GetJointController()->SetVelocityPID(joint3->GetScopedName(), pid);

        SetVelocityToJoints(0);

        //=========================

        if (!ros::isInitialized()) {
            int argc = 0;
            char** argv = NULL;
            ros::init(argc, argv, "BattlebotsPlugin");
        }
        rosSub = rosNode.subscribe(("/sim/" + model->GetName() + "/vel_cmd"), 1000, &BattlebotsPlugin::OnRosMsg, this);

        ROS_INFO("%s control plugin loaded!", model->GetName().c_str());
    }
};

GZ_REGISTER_MODEL_PLUGIN(BattlebotsPlugin)
}
