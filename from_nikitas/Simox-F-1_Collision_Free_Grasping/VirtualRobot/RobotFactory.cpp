
#include "RobotFactory.h"
#include "Robot.h"
#include "Nodes/RobotNode.h"
#include "Nodes/RobotNodeRevolute.h"
#include "Nodes/RobotNodePrismatic.h"
#include "Nodes/RobotNodeFixed.h"
#include "Visualization//VisualizationFactory.h"
#include "VirtualRobotException.h"


#include <algorithm>
#include <deque>

namespace VirtualRobot
{


    RobotFactory::RobotFactory()
    {
    }

    RobotFactory::~RobotFactory()
    {
    }


    RobotPtr RobotFactory::createRobot(const std::string& name, const std::string& type)
    {
        RobotPtr result(new LocalRobot(name, type));
        return result;

    }

    bool RobotFactory::initializeRobot(RobotPtr robot,
                                       std::vector<RobotNodePtr >& robotNodes,
                                       std::map< RobotNodePtr, std::vector<std::string> > childrenMap,
                                       RobotNodePtr rootNode
                                      )
    {
        VR_ASSERT(robot);
        bool result = true;

        // check for root
        std::vector<RobotNodePtr >::iterator iter = robotNodes.begin();
        bool foundRoot = false;

        while (iter != robotNodes.end())
        {
            if ((*iter) == rootNode)
            {
                foundRoot = true;
            }

            if ((*iter)->getRobot() != robot)
            {
                THROW_VR_EXCEPTION("Invalid robot node (robot is not set correctly)");
            }

            iter++;
        }

        THROW_VR_EXCEPTION_IF(!foundRoot, "Invalid robot node (root is not available)");

        // process children
        std::map< RobotNodePtr, std::vector< std::string > >::iterator iterC = childrenMap.begin();

        while (iterC != childrenMap.end())
        {
            std::vector< std::string > childNames = iterC->second;
            RobotNodePtr node = iterC->first;

            for (size_t i = 0; i < childNames.size(); i++)
            {
                std::string childName = childNames[i];

                if (!robot->hasRobotNode(childName))
                {
                    THROW_VR_EXCEPTION("Robot " << robot->getName() << ": corrupted RobotNode <" << node->getName() << " child :" << childName << " does not exist...");
                }

                RobotNodePtr c = robot->getRobotNode(childName);
                node->attachChild(c);
            }

            iterC++;
        }

        // register root (performs an initialization of all robot nodes)
        robot->setRootNode(rootNode);

        for (size_t i = 0; i < robotNodes.size(); i++)
        {
            if (!robotNodes[i]->getParent() && robotNodes[i] != rootNode)
            {
                VR_ERROR << "RobotNode " << robotNodes[i]->getName() << " is not connected to kinematic structure..." << endl;
            }
        }

        return result;
    }



    struct robotNodeDef
    {
        std::string name;
        std::vector<std::string> children;
    };

    struct robotStructureDef
    {
        std::string rootName;
        std::vector<robotNodeDef> parentChildMapping;
    };

    RobotPtr RobotFactory::cloneInversed(RobotPtr robot, const std::string& newRootName)
    {
        VR_ASSERT(robot);

        RobotNodePtr newRoot = robot->getRobotNode(newRootName);

        if (!newRoot)
        {
            VR_ERROR << "No node " << newRootName << endl;
        }

        RobotFactory::robotStructureDef newStructure;
        newStructure.rootName = newRootName;

        typedef std::pair<RobotNodePtr, RobotNodePtr> RobotTreeEdge;

        std::deque<RobotTreeEdge> edges;
        RobotTreeEdge rootEdge;
        rootEdge.second = newRoot;
        edges.push_back(rootEdge);

        while (!edges.empty())
        {
            RobotTreeEdge currentEdge = edges.front();
            edges.pop_front();

            RobotNodePtr parent = boost::dynamic_pointer_cast<RobotNode>(currentEdge.second->getParent());

            std::vector<SceneObjectPtr> children = currentEdge.second->getChildren();
            RobotFactory::robotNodeDef rnDef;
            rnDef.name = currentEdge.second->getName();

            // invert transformation of old parent
            if (parent && parent != currentEdge.first)
            {
                rnDef.invertTransformation.push_back(true);
                rnDef.children.push_back(parent->getName());

                RobotTreeEdge edge;
                edge.first = currentEdge.second;
                edge.second = parent;

                BOOST_ASSERT(edge.second);
                edges.push_back(edge);
            }

            for (unsigned i = 0; i < children.size(); i++)
            {
                if (children[i] != currentEdge.first)
                {
                    RobotNodePtr childNode = boost::dynamic_pointer_cast<RobotNode>(children[i]);

                    // not a robot node
                    if (!childNode)
                    {
                        continue;
                    }

                    rnDef.children.push_back(children[i]->getName());
                    rnDef.invertTransformation.push_back(false);
                    RobotTreeEdge edge;
                    edge.second = childNode;
                    edge.first = currentEdge.second;

                    BOOST_ASSERT(edge.second);
                    edges.push_back(edge);
                }
            }

            newStructure.parentChildMapping.push_back(rnDef);
        }


        return RobotFactory::cloneChangeStructure(robot, newStructure);
    }


    RobotPtr RobotFactory::cloneChangeStructure(RobotPtr robot, const std::string& startNode, const std::string& endNode)
    {
        VR_ASSERT(robot);

        if (!robot->hasRobotNode(startNode))
        {
            VR_ERROR << "No node with name " << startNode << endl;
            return RobotPtr();
        }

        if (!robot->hasRobotNode(endNode))
        {
            VR_ERROR << "No node with name " << endNode << endl;
            return RobotPtr();
        }

        if (!robot->getRobotNode(startNode)->hasChild(endNode, true))
        {
            VR_ERROR << "No node " << endNode << " is not a child of " << startNode << endl;
            return RobotPtr();
        }

        std::vector<std::string> nodes;
        std::string currentNodeName = endNode;
        RobotNodePtr rn = robot->getRobotNode(currentNodeName);

        while (rn && !(rn->getName() == startNode))
        {
            currentNodeName = rn->getName();
            nodes.push_back(currentNodeName);
            rn = boost::dynamic_pointer_cast<RobotNode>(rn->getParent());
        }

        if (!rn)
        {
            VR_ERROR << "No node " << endNode << " is not a child of " << startNode << endl;
            return RobotPtr();
        }

        nodes.push_back(startNode);
        //std::reverse(nodes.begin(), nodes.end());

        RobotFactory::robotStructureDef newStructure;
        newStructure.rootName = endNode;

        for (size_t i = 0; i < nodes.size() - 1; i++)
        {
            RobotFactory::robotNodeDef rnDef;
            rnDef.name = nodes[i];
            rnDef.children.push_back(nodes[i + 1]);
            rnDef.invertTransformation.push_back(true);
            newStructure.parentChildMapping.push_back(rnDef);
        }

        RobotFactory::robotNodeDef rnDef;
        rnDef.name = nodes[nodes.size() - 1];
        rnDef.invertTransformation.push_back(true);
        newStructure.parentChildMapping.push_back(rnDef);


        return RobotFactory::cloneChangeStructure(robot, newStructure);
    }

    RobotPtr RobotFactory::cloneChangeStructure(RobotPtr robot, robotStructureDef& newStructure)
    {
        VR_ASSERT(robot);

        if (!robot->hasRobotNode(newStructure.rootName))
        {
            VR_ERROR << "No root with name " << newStructure.rootName << endl;
            return RobotPtr();
        }

        std::map<std::string, RobotNodePtr> newNodes;
        RobotPtr newRobot = createRobot(robot->getName(), robot->getType() + "_restructured_" + newStructure.rootName);
        RobotNodePtr rn = robot->getRobotNode(newStructure.rootName);
        rn = rn->clone(newRobot, false);
        newNodes[newStructure.rootName] = rn;
        newRobot->setRootNode(newNodes[newStructure.rootName]);

        std::string nodeName;
        typedef std::map<RobotNodePtr,
            Eigen::Matrix4f,
            std::less<RobotNodePtr>,
            Eigen::aligned_allocator<std::pair<const int, Eigen::Matrix4f> > >
            NodeTransformationMapT;

        NodeTransformationMapT localTransformations;
        std::map<RobotNodePtr, VisualizationNodePtr> visuMap;
        std::map<RobotNodePtr, CollisionModelPtr> colMap;
        std::map<RobotNodePtr, bool> directionInversion;

        for (size_t i = 0; i < newStructure.parentChildMapping.size(); i++)
        {
            if (!robot->hasRobotNode(newStructure.parentChildMapping[i].name))
            {
                VR_ERROR << "Error in parentChildMapping, no node with name " << newStructure.parentChildMapping[i].name << endl;
                return RobotPtr();
            }

            nodeName = newStructure.parentChildMapping[i].name;

            if (newNodes.find(nodeName) == newNodes.end())
            {
                rn = robot->getRobotNode(nodeName);
                rn = rn->clone(newRobot, false);
                newNodes[nodeName] = rn;
            }

            RobotNodePtr parent = newNodes[newStructure.parentChildMapping[i].name];

            for (size_t j = 0; j < newStructure.parentChildMapping[i].children.size(); j++)
            {
                nodeName = newStructure.parentChildMapping[i].children[j];

                if (!robot->hasRobotNode(nodeName))
                {
                    VR_ERROR << "Error in parentChildMapping, no child node with name " << nodeName << endl;
                    return RobotPtr();
                }

                if (newNodes.find(nodeName) == newNodes.end())
                {
                    rn = robot->getRobotNode(nodeName);
                    rn = rn->clone(newRobot, false);
                    newNodes[nodeName] = rn;
                }

                //children.push_back(newNodes[nodeName]);
                RobotNodePtr child = newNodes[nodeName];
                parent->attachChild(child);

                if (newStructure.parentChildMapping[i].invertTransformation[j])
                {
                    Eigen::Matrix4f tr = parent->getLocalTransformation().inverse();
                    localTransformations[child] = tr;
                    // we also need to invert the direction
                    directionInversion[child] = true;

                    // check for models
                    if (child->getVisualization())
                    {
                        VisualizationNodePtr v = child->getVisualization();
                        VisualizationFactoryPtr vf = VisualizationFactory::first(NULL);
                        Eigen::Matrix4f tr2 = tr;
                        tr2.block(0, 3, 3, 1) *= 0.001f; // m is needed here?
                        vf->applyDisplacement(v, tr2);
                        visuMap[parent] = v;

                        for (size_t pr = 0; pr < v->primitives.size(); pr++)
                        {
                            v->primitives[pr]->transform = tr * v->primitives[pr]->transform;
                        }
                    }
                    if (child->getCollisionModel())
                    {
                        CollisionModelPtr c = child->getCollisionModel();
                        VisualizationNodePtr v = child->getCollisionModel()->getVisualization();
                        VisualizationFactoryPtr vf = VisualizationFactory::first(NULL);
                        Eigen::Matrix4f tr2 = tr;
                        tr2.block(0, 3, 3, 1) *= 0.001f; // m is needed here?
                        vf->applyDisplacement(v, tr2);
                        v->createTriMeshModel(); // update trimesh model
                        c->setVisualization(v);
                        colMap[parent] = c;

                        for (size_t pr = 0; pr < v->primitives.size(); pr++)
                        {
                            v->primitives[pr]->transform = tr * v->primitives[pr]->transform;
                        }
                    }
                }
                else
                {
                    localTransformations[child] = child->getLocalTransformation();
                    directionInversion[child] = false;
                    if (child->getVisualization())
                        visuMap[child] = child->getVisualization();
                    if (child->getCollisionModel())
                        colMap[child] = child->getCollisionModel();
                }
            }

            // if parent has no parent: reset local transformation
            if (localTransformations.find(parent) == localTransformations.end())
            {
                localTransformations[parent] = Eigen::Matrix4f::Identity();
                directionInversion[parent] = false;
            }
        }

        // apply all transformations
        NodeTransformationMapT::iterator it = localTransformations.begin();

        while (it != localTransformations.end())
        {
            it->first->localTransformation = it->second;
            std::map<RobotNodePtr, bool>::iterator inv_it = directionInversion.find(it->first);
            VR_ASSERT(inv_it != directionInversion.end());
            RobotNodeRevolutePtr rotJoint = boost::dynamic_pointer_cast<RobotNodeRevolute>(it->first);

            if (inv_it->second && rotJoint)
            {
                rotJoint->jointRotationAxis *= -1.0f;
            }

            it++;
        }

        std::vector<RobotNodePtr> nodes = newRobot->getRobotNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (visuMap.find(nodes[i]) != visuMap.end())
            {
                nodes[i]->setVisualization(visuMap[nodes[i]]);
            } else
                nodes[i]->setVisualization(VisualizationNodePtr());

            if (colMap.find(nodes[i]) != colMap.end())
            {
                nodes[i]->setCollisionModel(colMap[nodes[i]]);
            }
            else
                nodes[i]->setCollisionModel(CollisionModelPtr());
        }

        newRobot->getRootNode()->initialize();

        return newRobot;
    }

} // namespace VirtualRobot
