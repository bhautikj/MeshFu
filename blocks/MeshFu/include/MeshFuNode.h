/*
 Copyright (C) 2013 Bhautik Joshi
 
 This is heavily based on Node.h as found in the cinder-assimp library,
 originally written by Gabor Papp.
 See: https://github.com/gaborpapp/Cinder-Assimp/blob/master/src/Node.h
 
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

#include <string>
#include <vector>

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/Quaternion.h"
#include "cinder/Matrix.h"

#include "MeshFuGeometry.h"
#include "MeshFuContainer.h"
#include "gzstream.h"

namespace MeshFu
{
  
  class Node;
  typedef std::shared_ptr< Node > NodeRef;
  
  class Node
  {
  public:
    Node(const std::string& name="") :
    mName(name),
    mScale( ci::Vec3f::one() ),
    mInheritOrientation( true ),
    mInheritScale( true ),
    mNeedsUpdate( true )
    {
      
    }
    
    ~Node() {}
    void setParent( NodeRef parent );
    NodeRef getParent() const;
    
    void addChild( NodeRef child );
    void addChildName (std::string name) { mChildNodeNames.push_back(name); }
    void addMesh(MeshContainerRef mesh) { mMeshes.push_back(mesh); }
    void addMeshName(std::string name) { mMeshNames.push_back(name); }
    
    void setOrientation( const ci::Quatf &q );
    const ci::Quatf &getOrientation() const;
    
    void setPosition( const ci::Vec3f &pos );
    const ci::Vec3f& getPosition() const;
    
    void setScale( const ci::Vec3f &scale );
    const ci::Vec3f &getScale() const;
    
    void setInheritOrientation( bool inherit );
    bool getInheritOrientation() const;
    
    void setInheritScale( bool inherit );
    bool getInheritScale() const;
    
    void setName( const std::string &name );
    const std::string &getName() const;
    
    void setInitialState();
    void resetToInitialState();
    const ci::Vec3f &getInitialPosition() const;
    const ci::Quatf &getInitialOrientation() const;
    const ci::Vec3f &getInitialScale() const;
    
    const ci::Quatf &getDerivedOrientation() const;
    const ci::Vec3f &getDerivedPosition() const;
    const ci::Vec3f &getDerivedScale() const;
    
    const ci::Matrix44f &getDerivedTransform() const;
    
    void requestUpdate();
    
    
  protected:
    /// Shared pointer to parent node.
    NodeRef mParent;
    std::string mParentName;
    
    /// Shared pointer vector holding the children.
    std::vector< NodeRef > mChildren;
    std::vector< std::string > mChildNodeNames;
    
    std::vector< MeshContainerRef > mMeshes;
    std::vector< std::string > mMeshNames;
    
    /// Name of this node.
    std::string mName;
    
    /// The orientation of the node relative to its parent.
    ci::Quatf mOrientation;
    
    /// The position of the node relative to its parent.
    ci::Vec3f mPosition;
    
    /// Scaling factor applied to this node.
    ci::Vec3f mScale;
    
    /// Stores whether this node inherits orientation from its parent.
    bool mInheritOrientation;
    
    mutable bool mNeedsUpdate;
    
    /// Stores whether this node inherits scale from its parent
    bool mInheritScale;
    
    /** Cached combined orientation.
     This member is the orientation derived by combining the
     local transformations and those of it's parents.
     */
    mutable ci::Quatf mDerivedOrientation;
    
    /** Cached combined position.
     This member is the position derived by combining the
     local transformations and those of it's parents.
     */
    mutable ci::Vec3f mDerivedPosition;
    
    /** Cached combined scale.
     This member is the position derived by combining the
     local transformations and those of it's parents.
     */
    mutable ci::Vec3f mDerivedScale;
    
    /// The position to use as a base for keyframe animation.
    ci::Vec3f mInitialPosition;
    /// The orientation to use as a base for keyframe animation.
    ci::Quatf mInitialOrientation;
    /// The scale to use as a base for keyframe animation.
    ci::Vec3f mInitialScale;
    
    /// Cached derived transform as a 4x4 matrix
    mutable ci::Matrix44f mDerivedTransform;
    
    virtual void          write(ogzstream& stream);
    virtual void          read(igzstream& stream);
    
    void update() const;
    
    friend class ModelWrangler;
  };
  
}//namespace MeshFu
