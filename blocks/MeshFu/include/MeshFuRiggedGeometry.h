/*
 Copyright (C) 2013 Bhautik Joshi
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 
 */

#pragma once

#include "MeshFuGeometry.h"
#include "MeshFuNode.h"

#include <string>
#include <vector>
#include <map>

namespace MeshFu
{
  
  class RiggedGeometry;
  typedef std::shared_ptr< RiggedGeometry > RiggedGeometryRef;
  
  class RiggedGeometry
  {
    
  public:
    RiggedGeometry() {}
    RiggedGeometry(const std::string &file) : mFilePath(file) { };
    ~RiggedGeometry() {}
    virtual void                          write(ogzstream& stream);
    virtual void                          read(igzstream& stream);
    void                                  buildAll();
    
    std::vector< std::string >            getMeshNames();
    std::vector< std::string >            getNodeNames();
    GeometryRef                           getMeshByName(std::string name);
    NodeRef                               getNodeByName(std::string name);
    
    NodeRef                               getRootNode();
    
    bool                                  getMeshVisibility(const std::string& name);    
    bool                                  setMeshVisibility(const std::string& name, const bool& visible);
    
    void forceUpdate();
    void update();
    void draw();
    
  protected:
    void                                  updateMeshes();
    void                                  updateSkinning();
    ci::fs::path                          mFilePath;
    
    NodeRef                               mRootNode;
    std::string                           mRootNodeName;
    
    // all the meshes
    std::vector< GeometryRef >       mModelMeshes;
    
    // only the nodes with meshes
    std::vector< NodeRef >                mMeshNodes;
    std::map<GeometryRef, std::vector<NodeRef> >
                                          mMeshToNodeMap;
    // mesh names
    std::vector< std::string >            mMeshNames;
    std::map< std::string, GeometryRef >
                                          mMeshMap;
    std::map < std::string, bool >        mMeshVisibilityMap;
    
    // node names
    std::vector< std::string >            mNodeNames;
    std::map< std::string, NodeRef >      mNodeMap;

  };
  
} // namespace MeshFu

