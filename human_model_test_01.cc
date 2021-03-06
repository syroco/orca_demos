// This file is a part of the ORCA framework.
// Copyright 2017, ISIR / Universite Pierre et Marie Curie (UPMC)
// Copyright 2018, Fuzzy Logic Robotics
// Main contributor(s): Antoine Hoarau, Ryan Lober, and
// Fuzzy Logic Robotics <info@fuzzylogicrobotics.com>
//
// ORCA is a whole-body reactive controller framework for robotics.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability.
//
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or
// data to be ensured and,  more generally, to use and operate it in the
// same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL-C license and that you accept its terms.

/** @file
 @copyright 2018 Fuzzy Logic Robotics <info@fuzzylogicrobotics.com>
 @author Antoine Hoarau
 @author Ryan Lober
*/

#include <orca/orca.h>
#include <orca/gazebo/GazeboServer.h>
#include <orca/gazebo/GazeboModel.h>

using namespace orca::all;
using namespace orca::gazebo;



int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage : " << argv[0] << " /path/to/robot-urdf.urdf (optionally -l debug/info/warning/error)" << "\n";
        return -1;
    }
    std::string urdf_url(argv[1]);

    GazeboServer gz_server(argc,argv);
    auto gz_model = GazeboModel(gz_server.insertModelFromURDFFile(urdf_url));
    // auto gz_model = GazeboModel(gz_server.insertModelFromURDFFile(urdf_url,Eigen::Vector3d(0,0,0.901)));

    gz_model.setModelConfiguration( { "left_shoulder_X", "right_shoulder_X" } , { M_PI/2., -M_PI/2. } );
    // gz_model.setModelConfiguration( { "left_shoulder_abduction", "right_shoulder_abduction" } , { M_PI/2., -M_PI/2. } );

    orca::utils::Logger::parseArgv(argc, argv);

    auto robot_model = std::make_shared<RobotModel>();
    robot_model->loadModelFromFile(urdf_url);
    // robot_model->setBaseFrame("pelvis");
    robot_model->setBaseFrame("root");
    robot_model->setGravity(Eigen::Vector3d(0,0,-9.81));

    const int ndof = robot_model->getNrOfDegreesOfFreedom();

    gz_model.setBrakes(false);

    gz_model.setJointMapping(robot_model->getJointNames());

    gz_model.executeAfterWorldUpdate([&](uint32_t n_iter,double current_time,double dt)
    {
        robot_model->setRobotState(gz_model.getWorldToBaseTransform().matrix()
                            ,gz_model.getJointPositions()
                            ,gz_model.getBaseVelocity()
                            ,gz_model.getJointVelocities()
                            ,gz_model.getGravity()
                        );

        // Compensate the gravity at least
        Eigen::VectorXd trq = Eigen::VectorXd::Zero(ndof);
        // std::cout <<  "JOINTS NAMES" << std::endl;
        // for (int i=0; i<ndof; i++)
        //   // if (robot_model->getJointName(i) == "right_knee_flexion")
        //   //   trq[i] = 100.;

        gz_model.setJointTorqueCommand(robot_model->getJointGravityTorques());
        // gz_model.setJointGravityTorques(robot_model->getJointGravityTorques());
        // gz_model.setJointTorqueCommand(Eigen::VectorXd::Zero(ndof));
        // gz_model.setJointTorqueCommand( controller.getJointTorqueCommand() );

    });

    gazebo::event::Events::pause.Signal(true);

    gz_server.run();
    return 0;
}
