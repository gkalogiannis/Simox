#include <VirtualRobot/VirtualRobot.h>
#include <VirtualRobot/RuntimeEnvironment.h>

#include <string>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using namespace VirtualRobot;

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "GraspPlannerEvaluatorWindow.h"
void DisplayRobotSelectionMenu(int &option) 
{
  cout << "-------------------------------------------"<<endl;
  cout << "Working on Ramcip Objects" << endl;
  cout << "-------------------------------------------"<<endl;
  cout << "Please Select Robot...."<<endl;
  cout << " 1.Shadow Hand"<<endl;
  cout << " 2.Barret Hand"<<endl;
  cout << " 3.Exit"<<endl;
  cout << "-------------------------------------------" << endl;
  cout << "Enter Selection...(default is Shadow Hand).." << endl;
  cin >> option;
}
void DisplayObjectSelectionMenu(int &option) 
{
  cout << "-------------------------------------------"<<endl;
  cout << "Please Select Object...."<<endl;
  cout << " 1.Amita Object"<<endl;
  cout << " 2.Depon Object"<<endl;
  cout << " 3.Exit"<<endl;
  cout << "-------------------------------------------" << endl;
  cout << "Enter Selection...(default is Shadow Hand).." << endl;
  cin >> option;
}

int main(int argc, char* argv[])
{
  int robot_selection=0;
    SoDB::init();
    SoQt::init(argc, argv, "showRobot");
    DisplayRobotSelectionMenu(robot_selection);
    switch (robot_selection)
    {
      case 1:
        break;
      case 2:
        break;
      case 3:
        return 0;
        break;
      default:
        break;
    }
    std::string filename("../../data/ramcip_scenes/AmitaScene.xml");
    //std::string filename("../../data/ramcip_scenes/DeponScene.xml");
    std::string robotName("SHADOWHAND");
    std::string eefName("SHADOWHAND");
    
    std::string objectName("ManipulationObject");
    std::string preshapeName("Grasp Preshape");

    VirtualRobot::RuntimeEnvironment::considerKey("scene");
    VirtualRobot::RuntimeEnvironment::considerKey("robot");
    VirtualRobot::RuntimeEnvironment::considerKey("object");
    VirtualRobot::RuntimeEnvironment::considerKey("endeffector");
    VirtualRobot::RuntimeEnvironment::considerKey("preshape");

    VirtualRobot::RuntimeEnvironment::processCommandLine(argc, argv);
    VirtualRobot::RuntimeEnvironment::print();

    filename = VirtualRobot::RuntimeEnvironment::checkValidFileParameter("scene", filename);

    std::string robName = VirtualRobot::RuntimeEnvironment::getValue("robot");

    if (!robName.empty() && VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(robName))
    {
        robotName = robName;
    }

    std::string objName = VirtualRobot::RuntimeEnvironment::getValue("object");

    if (!objName.empty() && VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(objName))
    {
        objectName = objName;
    }

    std::string eefname = VirtualRobot::RuntimeEnvironment::getValue("endeffector");

    if (!eefname.empty())
    {
        eefName = eefname;
    }

    std::string ps = VirtualRobot::RuntimeEnvironment::getValue("preshape");

    if (!ps.empty())
    {
        preshapeName = ps;
    }

    cout << "Robot name: " << robotName << endl;
    cout << "End effector name:" << eefName << ", preshape:" << preshapeName << endl;
    cout << "Object name: " << objectName << endl;
    cout << "Using scene from file:" << filename << endl;

    GraspPlannerEvaluatorWindow rw(filename, robotName, eefName, preshapeName, objectName);


    rw.main();

    return 0;
}
