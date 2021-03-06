
#include "GraspPlannerEvaluatorWindow.h"
#include "GraspPlanning/Visualization/CoinVisualization/CoinConvexHullVisualization.h"
#include "GraspPlanning/ContactConeGenerator.h"
#include "VirtualRobot/EndEffector/EndEffector.h"
#include "VirtualRobot/Workspace/Reachability.h"
#include "VirtualRobot/ManipulationObject.h"
#include "VirtualRobot/Grasping/Grasp.h"
#include "VirtualRobot/IK/GenericIKSolver.h"
#include "VirtualRobot/Grasping/GraspSet.h"
#include "VirtualRobot/CollisionDetection/CDManager.h"
#include "VirtualRobot/XML/ObjectIO.h"
#include "VirtualRobot/XML/RobotIO.h"
#include "VirtualRobot/Visualization/CoinVisualization/CoinVisualizationFactory.h"
#include "VirtualRobot/Visualization/TriMeshModel.h"
#include <QFileDialog>
#include <Eigen/Geometry>

#include <time.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <list>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "Inventor/actions/SoLineHighlightRenderAction.h"
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoScale.h>

#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;
using namespace std;
using namespace VirtualRobot;
using namespace GraspStudio;

float TIMER_MS = 30.0f;

GraspPlannerEvaluatorWindow::GraspPlannerEvaluatorWindow(std::string& sceneFile, std::string& robotName, std::string& eefName, std::string& preshape, std::string& objectName, Qt::WFlags flags) 
{
	VR_INFO << " start " << endl;

	// init the random number generator
	srand(time(NULL));
	graspNumber = 0;

	this->sceneFile = sceneFile;
	this->robotName = robotName;
	this->eefName = eefName;
	this->preshape = preshape;
	this->objectName = objectName;
	eefVisu = NULL;

	sceneSep = new SoSeparator;
	sceneSep->ref();
	sceneVisuSep = new SoSeparator;
	sceneSep->addChild(sceneVisuSep);
	frictionConeSep = new SoSeparator;
	robotSep = new SoSeparator;
	objectSep = new SoSeparator;
	graspsSep = new SoSeparator;
	graspsSep->ref();

#if 0
	SoSeparator* s = CoinVisualizationFactory::CreateCoordSystemVisualization();
	sceneSep->addChild(s);
#endif
	sceneSep->addChild(robotSep);
	sceneSep->addChild(objectSep);
	sceneSep->addChild(frictionConeSep);
	sceneSep->addChild(graspsSep);

	setupUI();

	loadScene();

	buildVisu();
	viewer->viewAll();
}


GraspPlannerEvaluatorWindow::~GraspPlannerEvaluatorWindow()
{
	sceneSep->unref();
	graspsSep->unref();

	if (eefVisu)
	{
		eefVisu->unref();
	}
}

void GraspPlannerEvaluatorWindow::setupUI()
{
	UI.setupUi(this);
	viewer = new SoQtExaminerViewer(UI.frameViewer, "", TRUE, SoQtExaminerViewer::BUILD_POPUP);

	// setup
	viewer->setBackgroundColor(SbColor(1.0f, 1.0f, 1.0f));
	viewer->setAccumulationBuffer(true);
#ifdef WIN32
	viewer->setAntialiasing(true, 8);
#endif
	viewer->setGLRenderAction(new SoLineHighlightRenderAction);
	viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
	viewer->setFeedbackVisibility(true);
	viewer->setSceneGraph(sceneSep);
	viewer->viewAll();

	connect(UI.pushButtonReset, SIGNAL(clicked()), this, SLOT(resetSceneryAll()));
	connect(UI.pushButtonPlan, SIGNAL(clicked()), this, SLOT(plan()));
	connect(UI.pushButtonSave, SIGNAL(clicked()), this, SLOT(save()));
	connect(UI.pushButtonOpen, SIGNAL(clicked()), this, SLOT(openEEF()));
	connect(UI.pushButtonClose, SIGNAL(clicked()), this, SLOT(closeEEF()));
	connect(UI.pushButtonNormalGrasp, SIGNAL(clicked()), this, SLOT(normalGrasp()));
	connect(UI.pushButtonResetPose, SIGNAL(clicked()), this, SLOT(resetPose()));
        connect(UI.pushButtonParsePerdurbationFile, SIGNAL(clicked()), this, SLOT(ParseThePerdurbationFile()));
        connect(UI.pushButtonSetToGroundTruthPose, SIGNAL(clicked()), this, SLOT(SetToGroundTruthPose()));
        connect(UI.pushButtonSetToDetectedPose, SIGNAL(clicked()), this, SLOT(SetToDetectedPose()));
        connect(UI.pushButtonPerturbatedGrasp, SIGNAL(clicked()), this, SLOT(PerturbatedGrasp()));

	connect(UI.checkBoxColModel, SIGNAL(clicked()), this, SLOT(colModel()));
	connect(UI.checkBoxCones, SIGNAL(clicked()), this, SLOT(frictionConeVisu()));
	connect(UI.checkBoxGrasps, SIGNAL(clicked()), this, SLOT(showGrasps()));
        connect(UI.pushButtonAllExperiments, SIGNAL(clicked()), this, SLOT(AutomateExperiments()));
        set_to_detected_pose = false;
        set_to_groundtr_pose = false;
}

Eigen::Matrix<float,3,3,1> GraspPlannerEvaluatorWindow:: PerdurbationGetTheRotationMatrix(Eigen::Matrix<float,3,3,1> rotation_matrix ,std::string line, int i)
{
                      int temp =0;
                      char * cstr = new char [line.length()+1];
                      std::strcpy (cstr, line.c_str());
                      char * p = std::strtok (cstr," ");
                      while (p!=0 && temp<3)
                      {
                          std::string temp_value(p);
                          float value = lexical_cast<float>(temp_value);
                          //std::cout<<"("<<i<<","<<temp<<")="<<value<<'\n';
                          rotation_matrix(i,temp) = value;
                          p = std::strtok(NULL," ");
                          temp++;
                      }
                      
                      delete[] cstr;
                      return rotation_matrix;
}
Eigen::Vector3f GraspPlannerEvaluatorWindow:: PerdurbationGetTheTranslationVector(Eigen::Vector3f translation_vector, std::string line, int i )
  {
                      int temp =0;
                      char * cstr = new char [line.length()+1];
                      std::strcpy (cstr, line.c_str());
                      char * p = std::strtok (cstr," ");
                      while (p!=0)
                      {
                          std::string temp_value(p);
                          float value = lexical_cast<float>(temp_value);
                          if (temp ==3)
                          {
                          //std::cout<<"("<<i<<","<<temp<<")="<<value<<'\n';
                          translation_vector(i) = value;
                          }
                          p = std::strtok(NULL," ");
                          temp++;
                      }
                      delete[] cstr;
                      return translation_vector;
  }
void GraspPlannerEvaluatorWindow::CreatePerdurbationLists()
{
  string line,temp;
    ifstream myfile ("../../data/perdurbation/softkings_evaluation");
      if (myfile.is_open())
          {
              while ( getline (myfile,line) )
              {
                      Eigen::Matrix<float, 3,3,1> rotation_matrix;
                      rotation_matrix = Eigen::Matrix<float,3,3,1>::Zero(3,3);
                      Eigen::Vector3f translation_vector;
                      translation_vector = Eigen::Vector3f::Zero(); 
                    if (line == "detected_pose: ")
                    {

                      getline(myfile,line);
                      int n_line =0;
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      getline(myfile,line);
                      n_line++;
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      n_line++; 
                      getline(myfile,line);
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      getline(myfile,line);
                      //Push back the data
                      PerdurbationRotationDetectedPoseList.push_back(rotation_matrix);
                      PerdurbationTranslationDetectedPoseList.push_back(translation_vector);
                    }
                      if (line == "ground truth pose: ")
                    {
                      getline(myfile,line);
                      int n_line =0;
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      getline(myfile,line);
                      n_line++;
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      n_line++; 
                      getline(myfile,line);
                      rotation_matrix = PerdurbationGetTheRotationMatrix(rotation_matrix, line,n_line);
                      translation_vector = PerdurbationGetTheTranslationVector(translation_vector, line, n_line);
                      getline(myfile,line);
                      //Push back the data
                      PerdurbationRotationGroundTrPoseList.push_back(rotation_matrix);
                      PerdurbationTranslationGroundTrPoseList.push_back(translation_vector);

                    }
                    temp=line;temp+=" \n";
                  }
              myfile.close();
            }

        else cout << "Unable to open file"; 

}
void GraspPlannerEvaluatorWindow::PrintThePerdurbationLists()
{
              std::cout<<"----------Detected Poses-----------"<<endl; 
              std::cout<<"-----------------------------------"<<endl;
              std::cout<<"---------Rotation Matrix-----------"<<endl;
              for (int i=0; i<PerdurbationRotationDetectedPoseList.size(); i++)
              {
                std::cout<<i<<endl;
                std::cout << PerdurbationRotationDetectedPoseList[i] << '\n'<<std::endl;
              }
              std::cout<<"---------Translation Vector-----------"<<endl;
              for (int i=0; i<PerdurbationTranslationDetectedPoseList.size(); i++)
              {
                std::cout<<i<<endl;
                std::cout << PerdurbationTranslationDetectedPoseList[i] << '\n'<<std::endl;
              }

              std::cout<<"--------Ground Truth Poses---------"<<endl;
              std::cout<<"-----------------------------------"<<endl;
              std::cout<<"---------Rotation Matrix-----------"<<endl;
              for (int i=0; i<PerdurbationRotationGroundTrPoseList.size(); i++)
              {
                std::cout<<i<<endl;
                std::cout << PerdurbationRotationGroundTrPoseList[i] << '\n'<<std::endl;
              } 
              std::cout<<"---------Translation Vector-----------"<<endl;
              for (int i=0; i<PerdurbationTranslationGroundTrPoseList.size(); i++)
              {
                std::cout<<i<<endl;
                std::cout << PerdurbationTranslationGroundTrPoseList[i] << '\n'<<std::endl;
              }

}
int GraspPlannerEvaluatorWindow::PerdurbationGetNumberOfEvaluation()
{
    string line;
    int number_of_eval;
    number_of_eval = 0;
    ifstream myfile ("../../data/perdurbation/softkings_evaluation");
      if (myfile.is_open())
          {
              while ( getline (myfile,line) )
              {
                if (line=="detected_pose: "){
                number_of_eval=number_of_eval+1;}
              }
              myfile.close();
              return number_of_eval;
          }

        else {
          cout << "Unable to open file"; 
          return 0;
        }
}

void GraspPlannerEvaluatorWindow::ParseThePerdurbationFile()
{
  std::stringstream ss;
  ss << PerdurbationGetNumberOfEvaluation()-1;
  UI.labelEvaluationNo->setText(QString(ss.str().c_str()));
  CreatePerdurbationLists(); 
  //PrintThePerdurbationLists();
}
void GraspPlannerEvaluatorWindow::SetToDetectedPose()
{
  std::cout<<"Setting to Detected Pose"<<endl;
  int no_of_eval = UI.spinBoxPerdurbation->value();
 
  resetPose();
  if (no_of_eval >= 0)
  {
      	Eigen::Matrix4f deltaPose;
	deltaPose.setIdentity();
     std::cout<<PerdurbationRotationDetectedPoseList[no_of_eval]<<endl;
     std::cout<<PerdurbationTranslationDetectedPoseList[no_of_eval]<<endl;
     if (UI.checkBoxPerdurbationRotation->isChecked() && UI.checkBoxPerdurbationTranslation->isChecked()) 
     {

        deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];
	deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationRotation->isChecked())
     {
    deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationTranslation->isChecked())
     {
	deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];
     }
         std::cout<<"Set to Detected Pose"<<endl;
         // std::cout << "Pose:\n" << object->getGlobalPose() << std::endl;
	// std::cout << "DeltaPose:\n" << deltaPose << std::endl;
        std::cout << "FinalPose:\n" << (deltaPose*object->getGlobalPose()) << std::endl;
	object->setGlobalPose(deltaPose*object->getGlobalPose());
        set_to_detected_pose =true;
        UI.StatusLabel->setText("Detected Pose");

  }
  else{
    std::cout<<"Select Number of Evaluation First"<<endl;
  set_to_detected_pose =false;}
}
void GraspPlannerEvaluatorWindow::SetToGroundTruthPose()
{
   std::cout<<"Setting to Ground Truth Pose"<<endl;
  int no_of_eval = UI.spinBoxPerdurbation->value();
  resetPose();
  if (no_of_eval >= 0)
  {
      	Eigen::Matrix4f deltaPose;
	deltaPose.setIdentity();
     std::cout<<PerdurbationRotationGroundTrPoseList[no_of_eval]<<endl;
     std::cout<<PerdurbationTranslationGroundTrPoseList[no_of_eval]<<endl;
     if (UI.checkBoxPerdurbationRotation->isChecked() && UI.checkBoxPerdurbationTranslation->isChecked()) 
     {

        deltaPose.block(0,0,3,3) = PerdurbationRotationGroundTrPoseList[no_of_eval];
	deltaPose.block(0,3,3,1) = PerdurbationTranslationGroundTrPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationRotation->isChecked())
     {
    deltaPose.block(0,0,3,3) = PerdurbationRotationGroundTrPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationTranslation->isChecked())
     {
	deltaPose.block(0,3,3,1) = PerdurbationTranslationGroundTrPoseList[no_of_eval];
     }
     std::cout<<"Set to Ground Truth Pose"<<endl;
        // std::cout << "Pose:\n" << object->getGlobalPose() << std::endl;
	// std::cout << "DeltaPose:\n" << deltaPose << std::endl;
        std::cout << "FinalPose:\n" << (deltaPose*object->getGlobalPose()) << std::endl;
	object->setGlobalPose(deltaPose*object->getGlobalPose());
        set_to_groundtr_pose =true;
        UI.StatusLabel->setText("Ground Truth Pose"); 

  }
  else{
    std::cout<<"Select Number of Evaluation First"<<endl;
  set_to_groundtr_pose=false;
  }
}

void GraspPlannerEvaluatorWindow::resetSceneryAll()
{
	grasps->removeAllGrasps();
	graspsSep->removeAllChildren();
}


void GraspPlannerEvaluatorWindow::closeEvent(QCloseEvent* event)
{
	quit();
	QMainWindow::closeEvent(event);
}


void GraspPlannerEvaluatorWindow::buildVisu()
{
	sceneVisuSep->removeAllChildren();
	SceneObject::VisualizationType visuType = (UI.checkBoxColModel->isChecked()) ? SceneObject::Collision : SceneObject::Full;

	visualization = scene->getVisualization<CoinVisualization>(visuType);
	SoNode* visualisationNode = visualization->getCoinVisualization();

	if (visualisationNode)
	{
		sceneVisuSep->addChild(visualisationNode);
	}

	robotSep->removeAllChildren();
	SceneObject::VisualizationType colModel = (UI.checkBoxColModel->isChecked()) ? SceneObject::Collision : SceneObject::Full;

	if (eefCloned)
	{
		visualizationRobot = eefCloned->getVisualization<CoinVisualization>(colModel);
		SoNode* visualisationNode = visualizationRobot->getCoinVisualization();

		if (visualisationNode)
		{
			robotSep->addChild(visualisationNode);
			visualizationRobot->highlight(UI.checkBoxHighlight->isChecked());
		}
	}
	objectSep->removeAllChildren();

	if (object)
	{
		SceneObject::VisualizationType colModel2 = (UI.checkBoxColModel->isChecked()) ? SceneObject::CollisionData : SceneObject::Full;
		SoNode* visualisationNode = CoinVisualizationFactory::getCoinVisualization(object, colModel2);

		if (visualisationNode)
		{
			objectSep->addChild(visualisationNode);
		}

	}

	frictionConeSep->removeAllChildren();
	bool fc = (UI.checkBoxCones->isChecked());

	if (fc && contacts.size() > 0 && qualityMeasure)
	{
		ContactConeGeneratorPtr cg = qualityMeasure->getConeGenerator();
		float radius = cg->getConeRadius();
		float height = cg->getConeHeight();
		float scaling = 30.0f;
		SoNode* visualisationNode = CoinVisualizationFactory::getCoinVisualization(contacts, height * scaling, radius * scaling, true);

		if (visualisationNode)
		{
			frictionConeSep->addChild(visualisationNode);
		}

		// add approach dir visu
		for (size_t i = 0; i < contacts.size(); i++)
		{
			SoSeparator* s = new SoSeparator;
			Eigen::Matrix4f ma;
			ma.setIdentity();
			ma.block(0, 3, 3, 1) = contacts[i].contactPointFingerGlobal;
			SoMatrixTransform* m = CoinVisualizationFactory::getMatrixTransformScaleMM2M(ma);
			s->addChild(m);
			s->addChild(CoinVisualizationFactory::CreateArrow(contacts[i].approachDirectionGlobal, 10.0f, 1.0f));
			frictionConeSep->addChild(s);
		}
	}

	if (UI.checkBoxGrasps->isChecked() && sceneSep->findChild(graspsSep) < 0)
	{
		sceneSep->addChild(graspsSep);
	}

	if (!UI.checkBoxGrasps->isChecked() && sceneSep->findChild(graspsSep) >= 0)
	{
		sceneSep->removeChild(graspsSep);
	}

	viewer->scheduleRedraw();
}

int GraspPlannerEvaluatorWindow::main()
{
	SoQt::show(this);
	SoQt::mainLoop();
	return 0;
}


void GraspPlannerEvaluatorWindow::quit()
{
	std::cout << "GraspPlannerEvaluatorWindow: Closing" << std::endl;
	this->close();
	SoQt::exitMainLoop();
}

void GraspPlannerEvaluatorWindow::loadScene()
{
	sceneVisuSep->removeAllChildren();
	cout << "Loading Scene from " << sceneFile << endl;

	scene.reset();

	try
	{
		scene = SceneIO::loadScene(sceneFile);

		robot = scene->getRobot(robotName);
		if (!robot) {
			VR_ERROR << " no robot named as: " << robotName << endl;
			return;
		}
		else {
			eefInitialPose = robot->getGlobalPose();
			scene->deRegisterRobot(robotName);
		}

		eef = robot->getEndEffector(eefName);

		if (!preshape.empty())
		{
			eef->setPreshape(preshape);
		}

		eefVisu = CoinVisualizationFactory::CreateEndEffectorVisualization(eef);
		eefVisu->ref();

		object = scene->getManipulationObject(objectName);
		if (!object)
		{
			object = Obstacle::createBox(50.0f, 50.0f, 10.0f);
		}
		else {
			scene->deRegisterManipulationObject(objectName);
		}

		qualityMeasure.reset(new GraspStudio::GraspQualityMeasureWrenchSpace(object));
		//qualityMeasure->setVerbose(true);
		qualityMeasure->calculateObjectProperties();
		approach.reset(new GraspStudio::ApproachMovementSurfaceNormal(object, eef));
		eefCloned = approach->getEEFRobotClone();
		eefCloned->setGlobalPose(eefInitialPose);

		if (robot && eef)
		{
			std::string name = "Grasp Planner - ";
			name += eef->getName();
			grasps.reset(new GraspSet(name, robot->getType(), eefName));
		}
		planner.reset(new GraspStudio::GenericGraspPlanner(grasps, qualityMeasure, approach));
		planner->setVerbose(true);

		obstacles = scene->getObstacles();
		obstacleSet = SceneObjectSetPtr(new VirtualRobot::SceneObjectSet());
		for (unsigned i=0; i<obstacles.size(); i++) {
			obstacleSet->addSceneObject(obstacles[i]);
		}
	}
	catch (VirtualRobotException& e)
	{
		cout << "Could not find valid scene in file " << sceneFile << endl;
	}

	if (!scene)
	{
		cout << " ERROR while creating scene" << endl;
		return;
	}
}

void GraspPlannerEvaluatorWindow::plan()
{
	openEEF();
//	resetPose();

	float timeout = UI.spinBoxTimeOut->value() * 1000.0f;
	bool forceClosure = UI.checkBoxFoceClosure->isChecked();
	float quality = (float)UI.doubleSpinBoxQuality->value();
	int nrGrasps = UI.spinBoxGraspNumber->value();
	planner.reset(new GraspStudio::GenericGraspPlanner(grasps, qualityMeasure, approach, quality, forceClosure));
	VR_INFO << "EEF robot type:" << approach->getEEF()->getRobotType() << endl;
	/*if (!(approach->getEEF()->getRobot()->getCollisionModel()))
	  VR_INFO << "Null collision model" << endl;*/

	VR_ASSERT(obstacleSet);

	int nr = planner->plan(nrGrasps, timeout, obstacleSet);
	VR_INFO << " Grasp planned:" << nr << endl;
	std::stringstream ss;
	ss << grasps->getSize();
	UI.labelGraspNum->setText(QString(ss.str().c_str()));

	int start = (int)grasps->getSize() - nrGrasps - 1;

	if (start < 0)
	{
		start = 0;
	}

	grasps->setPreshape(preshape);

	for (int i = start; i < (int)grasps->getSize() - 1; i++)
	{
		std::cout << "Adding grasp..." << std::endl;
		Eigen::Matrix4f m = grasps->getGrasp(i)->getTcpPoseGlobal(object->getGlobalPose());
		SoSeparator* sep1 = new SoSeparator();
		SoMatrixTransform* mt = CoinVisualizationFactory::getMatrixTransformScaleMM2M(m);
		sep1->addChild(mt);
		sep1->addChild(eefVisu);
		graspsSep->addChild(sep1);
              
		QualityMeasureListBeforePerdurbation.push_back(  grasps->getGrasp(i)->getQuality());
	}

	// set to last valid grasp
	graspNumber = grasps->getSize();
	if (graspNumber > 0 && eefCloned && eefCloned->getEndEffector(eefName))
	{
		std::cout << "Setting grasp..." << std::endl;
		Eigen::Matrix4f mGrasp = grasps->getGrasp(graspNumber - 1)->getTcpPoseGlobal(object->getGlobalPose());
		eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eefName)->getTcp(), mGrasp);
                QualityMeasureListBeforePerdurbation.push_back(  grasps->getGrasp(graspNumber -1)->getQuality());
	}

	if (nrGrasps > 0 && graspNumber > 0)
	{
		std::cout << "Open/close fingers" << std::endl;
		openEEF();
		closeEEF();
	}
	else {
		resetPose();
	}
}

void GraspPlannerEvaluatorWindow::normalGrasp() {
       // resetPose(); 
        openEEF();

        graspNumber = UI.spinBoxGraspNum->value();
        if (graspNumber <= 0 || grasps->getSize() < graspNumber) {
                graspNumber = grasps->getSize();
                        UI.spinBoxGraspNum->setValue(graspNumber);
                std::cout << "Invalid grasp number selection! Settting to last planned grasp." << std::endl;
        }

        if (graspNumber > 0 && eefCloned && eefCloned->getEndEffector(eefName))
        {
                Eigen::Matrix4f mGrasp = grasps->getGrasp(graspNumber - 1)->getTcpPoseGlobal(object->getGlobalPose());
                eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eefName)->getTcp(), mGrasp);

                closeEEF();
        } 
}

void GraspPlannerEvaluatorWindow::closeEEF()
{
	contacts.clear();

	if (eefCloned && eefCloned->getEndEffector(eefName))
	{
		contacts = eefCloned->getEndEffector(eefName)->closeActors(object);
		qualityMeasure->setContactPoints(contacts);
		float qual = qualityMeasure->getGraspQuality();
		bool isFC = qualityMeasure->isGraspForceClosure();
                
		std::stringstream ss;
		ss << std::setprecision(3);
		ss << "Grasp Nr " << graspNumber << "\nQuality (wrench space): " << qual << "\nForce closure: ";

		if (isFC)
		{
			ss << "yes";
		}
		else
		{
			ss << "no";
		}

		UI.labelInfo->setText(QString(ss.str().c_str()));
	}

	buildVisu();
}

void GraspPlannerEvaluatorWindow::openEEF()
{
	contacts.clear();

	if (eefCloned && eefCloned->getEndEffector(eefName))
	{
		if (!preshape.empty())
			eefCloned->getEndEffector(eefName)->setPreshape(preshape);
		else
			eefCloned->getEndEffector(eefName)->openActors();
	}

	buildVisu();
}
Eigen::Matrix3f GraspPlannerEvaluatorWindow::rodriguesFormula(Eigen::Vector3f vector, float theta) {
	//Rotation vector must be of unit length
	//Angle theta must be in rad
	Eigen::Matrix3f mat;
	mat.setIdentity();

	Eigen::Matrix3f skewSym = getSkewSymmetricMatrix(vector);

	mat = mat + skewSym*sin(theta) + skewSym*skewSym*(1 - cos(theta));

	return mat;
}

Eigen::Matrix3f GraspPlannerEvaluatorWindow::getSkewSymmetricMatrix(Eigen::Vector3f vector) {
	Eigen::Matrix3f mat;
	mat(0,0) = 0;
	mat(1,1) = 0;
	mat(2,2) = 0;

	mat(0,1) = -vector(2);
	mat(1,0) = vector(2);

	mat(0,2) = vector(1);
	mat(2,0) = -vector(1);

	mat(1,2) = -vector(0);
	mat(2,1) = vector(0);

	return mat;
}

float GraspPlannerEvaluatorWindow::PerturbatedGrasp()
{
  openEEF();
  Eigen::Matrix4f mGrasp = grasps->getGrasp(graspNumber - 1)->getTcpPoseGlobal(object->getGlobalPose());
  eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eefName)->getTcp(), mGrasp);
        int no_of_eval = UI.spinBoxPerdurbation->value();
  Eigen::Matrix4f deltaPose;

  deltaPose.setIdentity();
  object->setGlobalPose(deltaPose);
  
  std::cout<<PerdurbationRotationDetectedPoseList[no_of_eval]<<endl;
  std::cout<<PerdurbationTranslationDetectedPoseList[no_of_eval]<<endl;
     if (UI.checkBoxPerdurbationRotation->isChecked() && UI.checkBoxPerdurbationTranslation->isChecked()) 
     {

        deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];
        deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationRotation->isChecked())
     {
    deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];
    deltaPose.block(0,3,3,1) = PerdurbationTranslationGroundTrPoseList[no_of_eval];

     }
     else if (UI.checkBoxPerdurbationTranslation->isChecked())
     {
        deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];
        deltaPose.block(0,0,3,3)= PerdurbationRotationGroundTrPoseList[no_of_eval];
     }
  object->setGlobalPose(deltaPose*object->getGlobalPose());
        //Grasp Center Point Robot Node
                VirtualRobot::RobotNodePtr graspNode = eefCloned->getEndEffector(eefName)->getGCP();
                //Grasp Pose
                Eigen::Matrix4f pose = graspNode->getGlobalPose();
                //Approach Direction (-z vector)
                Eigen::Vector3f approachDir = -pose.block(0,0,3,1);
                //Move eff away	
                moveEEFAway(approachDir, 3.0f);

                closeEEF();
                qualityMeasure->setContactPoints(contacts);
                float qual = qualityMeasure->getGraspQuality();
                std::cout<< "Perdubation"<<endl;
                // std::cout << "Pose:\n" << object->getGlobalPose() << std::endl;
	        std::cout << "Pose:\n" << object->getGlobalPose()<<endl;
        // std::cout << "FinalPose:\n" << (deltaPose*object->getGlobalPose()) << std::endl;
                return qual;

/*   std::cout<< " Perdurbation Grasp"<<endl; */
  // if (set_to_groundtr_pose)
  // {
    // //must check if there are valid grasps for ground truth
    // std::cout<<"Moving Object to the detected pose"<<endl;
    // openEEF();
    // int no_of_eval = UI.spinBoxPerdurbation->value();
 
  // //resetPose();
  // if (no_of_eval != 0)
  // {
              // Eigen::Matrix4f deltaPose;
	// deltaPose.setIdentity();
     // std::cout<<PerdurbationRotationDetectedPoseList[no_of_eval]<<endl;
     // std::cout<<PerdurbationTranslationDetectedPoseList[no_of_eval]<<endl;
     // if (UI.checkBoxPerdurbationRotation->isChecked() && UI.checkBoxPerdurbationTranslation->isChecked()) 
     // {

        // deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];
	// deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];

     // }
     // else if (UI.checkBoxPerdurbationRotation->isChecked())
     // {
    // deltaPose.block(0,0,3,3) = PerdurbationRotationDetectedPoseList[no_of_eval];

     // }
     // else if (UI.checkBoxPerdurbationTranslation->isChecked())
     // {
	// deltaPose.block(0,3,3,1) = PerdurbationTranslationDetectedPoseList[no_of_eval];
     // }
        // std::cout << "Pose:\n" << object->getGlobalPose() << std::endl;
	// std::cout << "DeltaPose:\n" << deltaPose << std::endl;
        // std::cout << "FinalPose:\n" << (deltaPose*object->getGlobalPose()) << std::endl;
	// object->setGlobalPose(deltaPose*object->getGlobalPose());
        // set_to_detected_pose =true;
        // UI.StatusLabel->setText("Detected Pose");
  // }

   // // SetToDetectedPose();
    // std::cout<<"Perform new grasp"<<endl;
    // //normalGrasp();
    
    // //closeEEF();
  // }
  // else
    // std::cout<<"Must set object to ground truth object first"<<endl;
       // [>  openEEF(); <]
	// // resetPose();

	// // graspNumber = UI.spinBoxGraspNum->value();
	// // if (graspNumber <= 0 || grasps->getSize() < graspNumber) {
		// // graspNumber = grasps->getSize();
		// // UI.spinBoxGraspNum->setValue(graspNumber);
		// // std::cout << "Invalid grasp number selection! Settting to last planned grasp." << std::endl;
	// // }

	// // // set to last valid grasp
	// // if (graspNumber > 0 && eefCloned && eefCloned->getEndEffector(eefName))
	// // {
		// // Eigen::Matrix4f mGrasp = grasps->getGrasp(graspNumber - 1)->getTcpPoseGlobal(object->getGlobalPose());
		// // eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eefName)->getTcp(), mGrasp);
		// // perturbateObject();

		// // //Grasp Center Point Robot Node
		// // VirtualRobot::RobotNodePtr graspNode = eefCloned->getEndEffector(eefName)->getGCP();
		// // //Grasp Pose
		// // Eigen::Matrix4f pose = graspNode->getGlobalPose();
		// // //Approach Direction (-z vector)
		// // Eigen::Vector3f approachDir = -pose.block(0,0,3,1);

		// // //Move eff away	
		// // moveEEFAway(approachDir, 3.0f);

		// // closeEEF();
	/* [> } <] */
}

void GraspPlannerEvaluatorWindow::AutomateExperiments()
{
  std::cout<<"Execute all experiments"<<endl;
  std::cout<<"-----------------------"<<endl;
   
   //Create folders that will hold the experimental results
  chdir("../../data");
  mkdir("perdurbation_results", 0700);
  chdir("perdurbation_results");


  //Experimental Variables
  int number_of_grasps =10;
  float timeout = (float)0 ;
  float min_quality = (float)0.4;
  bool force_closure =true;

  UI.spinBoxGraspNumber->setValue(number_of_grasps);
  UI.spinBoxTimeOut->setValue(timeout);
  UI.doubleSpinBoxQuality->setValue(min_quality);
  if (force_closure) {UI.checkBoxFoceClosure->setChecked(1);}

  ParseThePerdurbationFile();

  int evaluations = PerdurbationGetNumberOfEvaluation();
  
  //Set everything 
  UI.checkBoxPerdurbationRotation->setChecked(1); 
  UI.checkBoxPerdurbationTranslation->setChecked(1);
  ofstream result;
 
  for (int i = 0; i <17; i++)
  {
    resetPose();
    resetSceneryAll();
    std::cout<<"Using object evaluation "<<i<<endl;
    UI.spinBoxPerdurbation->setValue(i);
    SetToGroundTruthPose();
    //update the values to the ui
    QualityMeasureListBeforePerdurbation.clear();
    plan();
   

    //Save the currents grasps
    stringstream foldername,filename;
    foldername << i;
    string temp_str = foldername.str();
    mkdir((char*)temp_str.c_str(), 0700);
    chdir((char*)temp_str.c_str());
    std::cout<<"Saving the grasps xml file for evaluation "<<i<<endl;
    filename << number_of_grasps<<".xml";
    temp_str = filename.str();
    saveAs((char*)temp_str.c_str());

    //Find the best grasp
    std::vector<float>::iterator it;
    it =  max_element(QualityMeasureListBeforePerdurbation.begin(), QualityMeasureListBeforePerdurbation.end());
    int pos = distance(QualityMeasureListBeforePerdurbation.begin(), it); //The position in QualityMeasureList
    std::cout<<pos<<endl; 
    float best_grasp_before_perd = (float)*max_element(QualityMeasureListBeforePerdurbation.begin(), QualityMeasureListBeforePerdurbation.end());
    std::cout<<best_grasp_before_perd<<endl;

    //Set to the accordingly grasp
    UI.spinBoxGraspNum->setValue(pos+1);
    normalGrasp();
    
    //Perdurbate the objectt and get the perdurbated metric
     UI.checkBoxPerdurbationRotation->setChecked(1); 
    UI.checkBoxPerdurbationTranslation->setChecked(0);     

    float grasp_after_perd_only_rotation = PerturbatedGrasp();
    
    
    //Get back to ground truth rotation+translation
    resetPose();
    UI.checkBoxPerdurbationRotation->setChecked(1); 
    UI.checkBoxPerdurbationTranslation->setChecked(1);     
    SetToGroundTruthPose();
    
    //Set it to best grasp
    UI.spinBoxGraspNum->setValue(pos+1);
    normalGrasp();  
    
    //Perdurbate the objectt and get the perdurbated metric
    UI.checkBoxPerdurbationRotation->setChecked(0); 
   UI.checkBoxPerdurbationTranslation->setChecked(1);   

  float grasp_after_perd_only_translation = PerturbatedGrasp();

     //Get back to ground truth rotation+translation
    resetPose();
    UI.checkBoxPerdurbationRotation->setChecked(1); 
    UI.checkBoxPerdurbationTranslation->setChecked(1);     
    SetToGroundTruthPose();
    
    //Set it to best grasp
    UI.spinBoxGraspNum->setValue(pos+1);
    normalGrasp();  
    
    //Perdurbate the objectt and get the perdurbated metric
    UI.checkBoxPerdurbationRotation->setChecked(1); 
   UI.checkBoxPerdurbationTranslation->setChecked(1);   

  float grasp_after_perd_both = PerturbatedGrasp();

   


    //print the info
     result.open("result.txt");
    result <<i<<"       "<<best_grasp_before_perd<<"   "<<grasp_after_perd_only_rotation<<" "<<grasp_after_perd_only_translation<<" "<<grasp_after_perd_both<<"\n";
    
    result.close();
    chdir("../");
  }
  

}
void GraspPlannerEvaluatorWindow::resetPose()
{
	Eigen::Matrix4f pose;
	pose.setIdentity();
	object->setGlobalPose(pose);

	eefCloned->setGlobalPose(eefInitialPose);
        openEEF();
  UI.StatusLabel->setText("Normal"); 
}

void GraspPlannerEvaluatorWindow::moveEEFAway(const Eigen::Vector3f& approachDir, float step, int maxLoops)
{
	VirtualRobot::SceneObjectSetPtr sos = eefCloned->getEndEffector(eefName)->createSceneObjectSet();

	if (!sos)
	{
		return;
	}

	int loop = 0;
	Eigen::Vector3f delta = approachDir * step;

	while (loop < maxLoops && eefCloned->getEndEffector(eefName)->getCollisionChecker()->checkCollision(object->getCollisionModel(), sos))
	{
		updateEEFPose(delta);
		loop++;
	}
}
bool GraspPlannerEvaluatorWindow::updateEEFPose(const Eigen::Vector3f& deltaPosition)
{
	Eigen::Matrix4f deltaPose;
	deltaPose.setIdentity();
	deltaPose.block(0, 3, 3, 1) = deltaPosition;
	return updateEEFPose(deltaPose);
}

bool GraspPlannerEvaluatorWindow::updateEEFPose(const Eigen::Matrix4f& deltaPose)
{
	Eigen::Matrix4f pose = eefCloned->getEndEffector(eefName)->getGCP()->getGlobalPose();
	pose = deltaPose * pose;
	return setEEFPose(pose);
}

bool GraspPlannerEvaluatorWindow::setEEFPose(const Eigen::Matrix4f& pose)
{
	eefCloned->setGlobalPoseForRobotNode(eefCloned->getEndEffector(eefName)->getGCP(), pose);
	return true;
}


void GraspPlannerEvaluatorWindow::frictionConeVisu()
{
	buildVisu();
}

void GraspPlannerEvaluatorWindow::colModel()
{
	buildVisu();
}

void GraspPlannerEvaluatorWindow::showGrasps()
{
	buildVisu();
}
void GraspPlannerEvaluatorWindow::saveAs(QString filename)
{
	if (!object)
	{
		return;
	}

	ManipulationObjectPtr objectM(new ManipulationObject(object->getName(), object->getVisualization()->clone(), object->getCollisionModel()->clone()));
	objectM->addGraspSet(grasps);
	objectName = std::string(filename.toAscii());
	bool ok = false;

	try
	{
		ok = ObjectIO::saveManipulationObject(objectM, objectName);
	}
	catch (VirtualRobotException& e)
	{
		cout << " ERROR while saving object" << endl;
		cout << e.what();
		return;
	}

	if (!ok)
	{
		cout << " ERROR while saving object" << endl;
		return;
	}

}
void GraspPlannerEvaluatorWindow::save()
{
	if (!object)
	{
		return;
	}

	ManipulationObjectPtr objectM(new ManipulationObject(object->getName(), object->getVisualization()->clone(), object->getCollisionModel()->clone()));
	objectM->addGraspSet(grasps);
	QString fi = QFileDialog::getSaveFileName(this, tr("Save ManipulationObject"), QString(), tr("XML Files (*.xml)"));
	objectName = std::string(fi.toAscii());
	bool ok = false;

	try
	{
		ok = ObjectIO::saveManipulationObject(objectM, objectName);
	}
	catch (VirtualRobotException& e)
	{
		cout << " ERROR while saving object" << endl;
		cout << e.what();
		return;
	}

	if (!ok)
	{
		cout << " ERROR while saving object" << endl;
		return;
	}
}
