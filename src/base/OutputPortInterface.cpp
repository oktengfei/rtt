#include "PortInterface.hpp"
#include "OutputPortInterface.hpp"
#include "InputPortInterface.hpp"
#include "../internal/TaskObject.hpp"
#include "../Method.hpp"

using namespace RTT;
using namespace detail;
using namespace std;


OutputPortInterface::OutputPortInterface(std::string const& name)
    : PortInterface(name), cmanager(this) { }

OutputPortInterface::~OutputPortInterface()
{
    cmanager.disconnect();
}

/** Returns true if this port is connected */
bool OutputPortInterface::connected() const
{ return cmanager.connected(); }

void OutputPortInterface::disconnect(PortInterface& port)
{
    cmanager.disconnect(port);
}

void OutputPortInterface::disconnect()
{
    cmanager.disconnect();
}

bool OutputPortInterface::addConnection(PortID* port_id, ChannelElementBase::shared_ptr channel_input, ConnPolicy const& policy)
{
    // this will call us back using connectionAdded().
    return cmanager.addConnection(port_id, channel_input, policy);
}

bool OutputPortInterface::removeConnection(ChannelElementBase::shared_ptr channel)
{
    return cmanager.removeConnection(channel);
}


void OutputPortInterface::write(DataSourceBase::shared_ptr source)
{ throw std::runtime_error("calling default OutputPortInterface::write(datasource) implementation"); }

bool OutputPortInterface::createDataConnection( InputPortInterface& input, int lock_policy )
{ return createConnection( input, ConnPolicy::data(lock_policy) ); }

bool OutputPortInterface::createBufferConnection( InputPortInterface& input, int size, int lock_policy )
{ return createConnection( input, ConnPolicy::buffer(size, lock_policy) ); }

bool OutputPortInterface::createConnection( InputPortInterface& input )
{ return createConnection(input, input.getDefaultPolicy()); }

bool OutputPortInterface::connectTo(PortInterface& other, ConnPolicy const& policy)
{
    InputPortInterface* input  = dynamic_cast<InputPortInterface*>(&other);
    if (! input)
        return false;
    return createConnection(*input, policy);
}

bool OutputPortInterface::connectTo(PortInterface& other)
{
    InputPortInterface* input  = dynamic_cast<InputPortInterface*>(&other);
    if (! input)
        return false;
    return createConnection(*input);
}
