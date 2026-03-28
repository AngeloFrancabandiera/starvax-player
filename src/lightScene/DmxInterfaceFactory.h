#ifndef DMXINTERFACEFACTORY_H
#define DMXINTERFACEFACTORY_H

class DmxInterface;

/** this header file defines a factory for DMX interface.
 * Implementation file is platform specific */
class DmxInterfaceFactory
{
public:
   DmxInterfaceFactory() {}
   virtual ~DmxInterfaceFactory() {}

   DmxInterface * build();
};

#endif // DMXINTERFACEFACTORY_H
