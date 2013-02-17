#include <iostream>

#include "gzstream.h"

#include "ModelWranglerAssimp.h"

int main(int argc, char **argv)
{
  
  
  std::string fileName(argv[1]);
  std::string target = fileName + ".cmf";

  MeshFu::ModelWranglerAssimp loader(fileName);
  
  std::cout<<"loaded "<<fileName<<" successfully"<<std::endl;
  
  ogzstream out;
  out.open(target.c_str());
  loader.write(out);
  out.close();
  
  std::cout<<"wrote "<<target<<" successfully"<<std::endl;  
}