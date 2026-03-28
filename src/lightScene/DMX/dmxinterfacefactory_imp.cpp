#include "DmxInterfaceFactory.h"

#include "DmxInterface_K8062.h"

DmxInterface * DmxInterfaceFactory::build()
{
   return new DmxInterface_K8062();
}
