/***************************************************************************
  tag: FMTC  Tue Mar 11 21:49:23 CET 2008  OperationsI.cpp

                        OperationsI.cpp -  description
                           -------------------
    begin                : Tue March 11 2008
    copyright            : (C) 2008 FMTC
    email                : peter.soetens@fmtc.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/


// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:910

#include "OperationsI.h"
#include "../../internal/CommandC.hpp"
#include "AnyDataSource.hpp"
#include "../../scripting/DataSourceCondition.hpp"
#include "../../scripting/CommandDataSource.hpp"
#include "../../Logger.hpp"
#include "../../rtt-detail-fwd.hpp"


using namespace RTT;
using namespace RTT::detail;


// Implementation skeleton constructor
Orocos_CAction_i::Orocos_CAction_i (MethodC* orig, ActionInterface* com, PortableServer::POA_ptr the_poa )
    : morig(*orig), mcom(com), mpoa( PortableServer::POA::_duplicate(the_poa) )
{
}

// Implementation skeleton destructor
Orocos_CAction_i::~Orocos_CAction_i (void)
{
    delete mcom;
}

CORBA::Boolean Orocos_CAction_i::execute (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcom->execute();
}

CORBA::Boolean Orocos_CAction_i::executeAny (
      const ::RTT::corba::CAnyArguments& args
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
	  )) {
      MethodC mgen = morig;
    try {
        for (size_t i =0; i != args.length(); ++i)
            mgen.arg( DataSourceBase::shared_ptr( new RTT::corba::AnyDataSource( new CORBA::Any( args[i] ) )));
        // if not ready, not enough args were given, *guess* a one off error in the exception :-(
        if ( !mgen.ready() )
            throw ::RTT::corba::CWrongNumbArgException( args.length()+1, args.length() );
        delete mcom;
        if ( dynamic_cast< DataSource<bool>* >(mgen.getDataSource().get() ) )
            mcom = new CommandDataSourceBool( dynamic_cast<DataSource<bool>*>(mgen.getDataSource().get() ));
        else
            mcom = new CommandDataSource( mgen.getDataSource() );
        return this->execute();
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch ( wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return false;
  }


void Orocos_CAction_i::reset (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    mcom->reset();
}

// Implementation skeleton constructor
Orocos_CCommand_i::Orocos_CCommand_i ( CommandC& orig, CommandC& comm, PortableServer::POA_ptr the_poa)
    : morig( new CommandC(orig) ), mcomm( new CommandC(comm)), mpoa( PortableServer::POA::_duplicate(the_poa) )
{
}

Orocos_CCommand_i::Orocos_CCommand_i ( DispatchInterface::shared_ptr di, PortableServer::POA_ptr the_poa)
    : morig( new CommandC( di->clone() ) ), mcomm( new CommandC( di )), mpoa( PortableServer::POA::_duplicate(the_poa) )
{
}

// Implementation skeleton destructor
Orocos_CCommand_i::~Orocos_CCommand_i (void)
{
    delete morig;
    delete mcomm;
}

CORBA::Boolean Orocos_CCommand_i::execute (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    //Logger::In in("Orocos_CCommand_i");
    //Logger::log() <<Logger::Debug << "Executing CommandC."<<Logger::endl;
    return mcomm->execute();
}

CORBA::Boolean Orocos_CCommand_i::executeAny (
      const ::RTT::corba::CAnyArguments& args
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
	  )) {
    // if morig is already set, we can not create a new command.
    if ( morig->ready() ) {
        Logger::In in("executeAny()");
        log(Error) << "This CORBA CCommand does not support execution with new arguments." <<endlog();
        return false;
    }
      *mcomm = *morig;
    try {
        for (size_t i =0; i != args.length(); ++i)
            mcomm->arg( DataSourceBase::shared_ptr( new RTT::corba::AnyDataSource( new CORBA::Any( args[i] ) )));
        // if not ready, not enough args were given, *guess* a one off error in the exception :-(
        if ( !mcomm->ready() )
            throw ::RTT::corba::CWrongNumbArgException( args.length()+1, args.length() );
        return this->execute();
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch (wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return false;
  }


RTT::corba::CommandStatus Orocos_CCommand_i::status ( 
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    int status = mcomm->status();
    switch(status)
    {
    case DispatchInterface::NotReady:     return RTT::corba::NotReady;
    case DispatchInterface::Ready:        return RTT::corba::Ready;
    case DispatchInterface::Sent:         return RTT::corba::Sent;
    case DispatchInterface::NotAccepted:  return RTT::corba::NotAccepted;
    case DispatchInterface::Accepted:     return RTT::corba::Accepted;
    case DispatchInterface::Executed:     return RTT::corba::Executed;
    case DispatchInterface::NotValid:     return RTT::corba::NotValid;
    case DispatchInterface::Valid:        return RTT::corba::Valid;
    case DispatchInterface::Done:         return RTT::corba::Done;
    default:
        log(Error) << "wrong status " << status << " returned by command object" << endlog();
        return RTT::corba::NotReady;
    }
}

CORBA::Boolean Orocos_CCommand_i::done (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    //Logger::In in("Orocos_CCommand_i");
    //Logger::log() <<Logger::Debug << "Evaluating CommandC:"<<mcomm->done()<<Logger::endl;
    return mcomm->done();
}

CORBA::Boolean Orocos_CCommand_i::executed (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcomm->executed();
}

CORBA::Boolean Orocos_CCommand_i::sent (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcomm->sent();
}

CORBA::Boolean Orocos_CCommand_i::accepted (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcomm->accepted();
}

CORBA::Boolean Orocos_CCommand_i::valid (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcomm->valid();
}

void Orocos_CCommand_i::reset (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
    return mcomm->reset();
}

void Orocos_CCommand_i::destroyCommand (

  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here

}




