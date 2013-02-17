#include <iostream>

#include "gzstream.h"

#include "MeshFuModelWrangler.h"

int main(int argc, char **argv)
{
  std::string fileName(argv[1]);
  
  igzstream testIn;
  testIn.open(fileName.c_str());
  MeshFu::ModelWrangler testLoader(fileName);
  testLoader.read(testIn);
  testIn.close();
  
  std::cout<<fileName<<" re-read successfully"<<std::endl;  
}