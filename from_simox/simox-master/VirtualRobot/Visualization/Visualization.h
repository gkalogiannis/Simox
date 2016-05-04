/**
* This file is part of Simox.
*
* Simox is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* Simox is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* @package    VirtualRobot
* @author     Manfred Kroehnert, Nikolaus Vahrenkamp
* @copyright  2010,2011 Manfred Kroehnert, Nikolaus Vahrenkamp
*             GNU Lesser General Public License
*
*/
#ifndef _VirtualRobot_Visualization_h_
#define _VirtualRobot_Visualization_h_

#include "../VirtualRobot.h"
#include "VisualizationFactory.h"

#include <vector>
#include <Eigen/Core>

namespace VirtualRobot
{

    class VIRTUAL_ROBOT_IMPORT_EXPORT Visualization
    {
    public:

        Visualization(const VisualizationNodePtr visualizationNode);
        Visualization(const std::vector<VisualizationNodePtr>& visualizationNodes);
        virtual ~Visualization();

        /*!
            Highlight a visualization node.

            @param visualizationNode This node must be part of this visualization, passed to the constructor.
            @param enable Do/Undo highlighting.
        */
        virtual bool highlight(VisualizationNodePtr visualizationNode, bool enable);

        /*!
            Highlight a visualization node.

            @param which The index of the visualionNodes, passed to the constructor.
            @param enable Do/Undo highlighting.
        */
        virtual bool highlight(unsigned int which, bool enable);

        /*!
        Colorize this visualization.
        @param c The color to apply. To disable the colorization apply Color::None()
        */
        virtual void colorize(VisualizationFactory::Color c);

        /*!
            Colorize this visualization, but just set the transparency flag (no additional colorization is performed).
            @param transparency The transparent value in [0..1].
        */
        virtual void setTransparency(float transparency);

        virtual bool isVisualizationNodeRegistered(VisualizationNodePtr visualizationNode);

        virtual VisualizationPtr clone();

        //! get total number of faces (i.e. triangles) of all visualizations that are stored in this object
        virtual int getNumFaces();

        static std::string getFactoryName()
        {
            return "AbstractFactoryMethod";
        }

        std::vector<VisualizationNodePtr> getVisualizationNodes()
        {
            return visualizationNodes;
        }

    protected:
        std::vector<VisualizationNodePtr> visualizationNodes;
    };

} // namespace VirtualRobot

#endif // _VirtualRobot_Visualization_h_
