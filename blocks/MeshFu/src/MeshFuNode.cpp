/*
 Copyright (C) 2013 Bhautik Joshi
 
 This is derived from Node.cpp as found in the cinder-assimp library,
 originally written by Gabor Papp.
 See: https://github.com/gaborpapp/Cinder-Assimp/blob/master/src/Node.cpp
 
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


#include "MeshFuNode.h"
#include "MeshFuUtil.h"

namespace MeshFu
{
  
  void Node::setParent( NodeRef parent )
  {
    mParent = parent;
    if (mParent != NodeRef())
      mParentName = parent->getName();
    requestUpdate();
  }
  
  NodeRef Node::getParent() const
  {
    return mParent;
  }
  
  void Node::addChild( NodeRef child )
  {
    mChildren.push_back( child );
  }
  
  void Node::setOrientation( const ci::Quatf &q )
  {
    mOrientation = q;
    mOrientation.normalize();
    requestUpdate();
  }
  
  const ci::Quatf &Node::getOrientation() const
  {
    return mOrientation;
  }
  
  void Node::setPosition( const ci::Vec3f &pos )
  {
    mPosition = pos;
    requestUpdate();
  }
  
  const ci::Vec3f& Node::getPosition() const
  {
    return mPosition;
  }
  
  void Node::setScale( const ci::Vec3f &scale )
  {
    mScale = scale;
    requestUpdate();
  }
  
  const ci::Vec3f &Node::getScale() const
  {
    return mScale;
  }
  
  void Node::setInheritOrientation( bool inherit )
  {
    mInheritOrientation = inherit;
    requestUpdate();
  }
  
  bool Node::getInheritOrientation() const
  {
    return mInheritOrientation;
  }
  
  void Node::setInheritScale( bool inherit )
  {
    mInheritScale = inherit;
    requestUpdate();
  }
  
  bool Node::getInheritScale() const
  {
    return mInheritScale;
  }
  
  void Node::setName( const std::string &name )
  {
    mName = name;
  }
  
  const std::string &Node::getName() const
  {
    return mName;
  }
  
  void Node::setInitialState()
  {
    mInitialPosition = mPosition;
    mInitialOrientation = mOrientation;
    mInitialScale = mScale;
  }
  
  void Node::resetToInitialState()
  {
    mPosition = mInitialPosition;
    mOrientation = mInitialOrientation;
    mScale = mInitialScale;
    requestUpdate();
  }
  
  const ci::Vec3f &Node::getInitialPosition() const
  {
    return mInitialPosition;
  }
  
  const ci::Quatf &Node::getInitialOrientation() const
  {
    return mInitialOrientation;
  }
  
  const ci::Vec3f &Node::getInitialScale() const
  {
    return mInitialScale;
  }
  
  const ci::Quatf &Node::getDerivedOrientation() const
  {
    if ( mNeedsUpdate )
      update();
    return mDerivedOrientation;
  }
  
  const ci::Vec3f &Node::getDerivedPosition() const
  {
    if ( mNeedsUpdate )
      update();
    return mDerivedPosition;
  }
  
  const ci::Vec3f &Node::getDerivedScale() const
  {
    if ( mNeedsUpdate )
      update();
    return mDerivedScale;
  }
  
  const ci::Matrix44f &Node::getDerivedTransform() const
  {
    if ( mNeedsUpdate )
      update();
    
    mDerivedTransform = ci::Matrix44f::createScale( mDerivedScale );
    mDerivedTransform *= mDerivedOrientation.toMatrix44();
    mDerivedTransform.setTranslate( mDerivedPosition );
    
    return mDerivedTransform;
  }
  
  void Node::update() const
  {
    if ( mParent )
    {
      // update orientation
      const ci::Quatf &parentOrientation = mParent->getDerivedOrientation();
      if ( mInheritOrientation )
      {
        // Combine orientation with that of parent
        mDerivedOrientation = getOrientation() * parentOrientation;
      }
      else
      {
        mDerivedOrientation = getOrientation();
      }
      
      // update scale
      const ci::Vec3f &parentScale = mParent->getDerivedScale();
      if ( mInheritScale )
      {
        mDerivedScale = parentScale * getScale();
      }
      else
      {
        mDerivedScale = getScale();
      }
      
      // change position vector based on parent's orientation & scale
      mDerivedPosition = ( parentScale * getPosition() ) * parentOrientation;
      
      // add altered position vector to parent's
      mDerivedPosition += mParent->getDerivedPosition();
    }
    else
    {
      // root node, no parent
      mDerivedOrientation = getOrientation();
      mDerivedPosition = getPosition();
      mDerivedScale = getScale();
    }
    
    mNeedsUpdate = false;
  }
  
  void Node::requestUpdate()
  {
    mNeedsUpdate = true;
    
    for ( std::vector< NodeRef >::iterator it = mChildren.begin();
         it != mChildren.end(); ++it )
    {
      (*it)->requestUpdate();
    }
  }
  
  void Node::write(ogzstream& stream)
  {
    Util::writeString(stream, mName);
    Util::writeString(stream, mParentName);
    int numChildNodes = mChildNodeNames.size();
    stream<<numChildNodes<<std::endl;
    for (int i=0; i< numChildNodes;i++)
    {
      Util::writeString(stream, mChildNodeNames[i]);
    }
    int numMeshes = mMeshNames.size();
    stream<<numMeshes<<std::endl;
    for (int i=0; i< numMeshes; i++)
    {
      Util::writeString(stream, mMeshNames[i]);
    }
    Util::writeQuatf(stream, mOrientation);
    Util::writeVec3f(stream, mPosition);
    Util::writeVec3f(stream, mScale);
    stream<<mInheritOrientation<<std::endl;
    stream<<mNeedsUpdate<<std::endl;
    stream<<mInheritScale<<std::endl;
    Util::writeQuatf(stream, mDerivedOrientation);
    Util::writeVec3f(stream, mDerivedPosition);
    Util::writeVec3f(stream, mDerivedScale);
    Util::writeQuatf(stream, mInitialOrientation);
    Util::writeVec3f(stream, mInitialPosition);
    Util::writeVec3f(stream, mInitialScale);
    Util::writeMatrix44f(stream, mDerivedTransform);
  }
  
  void Node::read(igzstream& stream)
  {
    Util::readString(stream, mName);
    Util::readString(stream, mParentName);
    
    int numChildNodes;
    stream>>numChildNodes;
    for (int i=0; i< numChildNodes;i++)
    {
      std::string data;
      Util::readString(stream, data);
      mChildNodeNames.push_back(data);
    }
    int numMeshes;
    stream>>numMeshes;
    
    for (int i=0; i< numMeshes;i++)
    {
      std::string data;
      Util::readString(stream, data);
      mMeshNames.push_back(data);
    }
    Util::readQuatf(stream, mOrientation);
    Util::readVec3f(stream, mPosition);
    Util::readVec3f(stream, mScale);
    stream>>mInheritOrientation;
    stream>>mNeedsUpdate;
    stream>>mInheritScale;
    Util::readQuatf(stream, mDerivedOrientation);
    Util::readVec3f(stream, mDerivedPosition);
    Util::readVec3f(stream, mDerivedScale);
    Util::readQuatf(stream, mInitialOrientation);
    Util::readVec3f(stream, mInitialPosition);
    Util::readVec3f(stream, mInitialScale);
    Util::readMatrix44f(stream, mDerivedTransform);
  }
  
  
  void Node::setEulerAngles(const float& x, const float& y, const float& z)
  {
    ci::Matrix33<float> X(1, 0, 0, 0, cos(x), sin(x), 0, -1.*sin(x), cos(x));
    ci::Matrix33<float> Y(cos(y), 0, -1.*sin(y), 0, 1, 0, sin(y), 0, cos(y));
    ci::Matrix33<float> Z(cos(z), sin(z), 0, -1.*sin(z), cos(z), 0, 0, 0, 1);
    ci::Matrix33<float> R = Z * Y * X;
    ci::Matrix33<float> qMat(R);
    setOrientation(qMat);
  }
  
  void Node::getEulerAngles(float& x, float& y, float& z)
  {
    ci::Matrix33<float> qMat = getOrientation().toMatrix33();
    x = atan2(qMat.at(2,1), qMat.at(2,2));
    y = atan2(-1.0*qMat.at(2,0), sqrt(qMat.at(2,1)*qMat.at(2,1) + qMat.at(2,2)*qMat.at(2,2)));
    z = atan2(qMat.at(1,0), qMat.at(0,0));
  }

}//namespace MeshFu