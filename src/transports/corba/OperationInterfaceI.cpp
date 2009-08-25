/***************************************************************************
  tag: FMTC  Tue Mar 11 21:49:24 CET 2008  OperationInterfaceI.cpp

                        OperationInterfaceI.cpp -  description
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

#include "OperationInterfaceI.h"
#include "OperationsI.h"
#include "../../internal/OperationFactory.hpp"
#include "ExpressionProxy.hpp"
#include "AnyDataSource.hpp"
#include "../../rtt-detail-fwd.hpp"

using namespace RTT;
using namespace RTT::detail;
using namespace std;

// Implementation skeleton constructor
Orocos_CMethodInterface_i::Orocos_CMethodInterface_i (MethodFactory* gmf, PortableServer::POA_ptr the_poa)
    :mfact(gmf), mpoa( PortableServer::POA::_duplicate(the_poa))
{
}

// Implementation skeleton destructor
Orocos_CMethodInterface_i::~Orocos_CMethodInterface_i (void)
{
}

PortableServer::POA_ptr Orocos_CMethodInterface_i::_default_POA()
{
    return PortableServer::POA::_duplicate(mpoa);
}

::RTT::corba::CMethodList * Orocos_CMethodInterface_i::getMethods (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    CMethodList_var rlist = new CMethodList();

    vector<string> flist = mfact->getNames();
    rlist->length( flist.size() );
    for (size_t i=0; i != flist.size(); ++i)
        rlist[i] = CORBA::string_dup( flist[i].c_str() );
    return rlist._retn();
}

::RTT::corba::CDescriptions * Orocos_CMethodInterface_i::getArguments (
      const char* method
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ,::RTT::corba::CNoSuchNameException
      ))
{
    CDescriptions_var ret = new CDescriptions();
    if ( mfact->hasMember( string( method ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( method );
    // method found, convert args:
    MethodFactory::Descriptions args = mfact->getArgumentList( string(method) );
    ret->length( args.size() );
    for (size_t i =0; i != args.size(); ++i) {
        ret[i].name = CORBA::string_dup( args[i].name.c_str() );
        ret[i].description = CORBA::string_dup( args[i].description.c_str() );
        ret[i].type = CORBA::string_dup( args[i].type.c_str() );
    }
    return ret._retn();
}

 char * Orocos_CMethodInterface_i::getResultType (
      const char* method
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ,::RTT::corba::CNoSuchNameException
    ))
{
    if ( mfact->hasMember( string( method ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( method );
    return CORBA::string_dup( mfact->getResultType( string(method) ).c_str() );
}

 char * Orocos_CMethodInterface_i::getDescription (
      const char* method
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ,::RTT::corba::CNoSuchNameException
    ))
{
    if ( mfact->hasMember( string( method ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( method );
    return CORBA::string_dup( mfact->getDescription( string(method) ).c_str() );
}

::RTT::corba::CMethod_ptr Orocos_CMethodInterface_i::createMethod (
    const char * method,
    const ::RTT::corba::CArguments& args
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
  ))
{
    if ( mfact->hasMember( string( method ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( method );
    // convert Corba args to C++ args.
    MethodFactory::Arguments nargs;
    nargs.reserve( args.length() );
    for (size_t i =0; i != args.length(); ++i)
        nargs.push_back( ExpressionProxy::Create( CExpression::_duplicate( args[i] ) ) );
    // create a local data source and a new method servant to serve it.
    try {
        MethodC orig(mfact, method);
        return (::RTT::corba::CMethod_ptr)(mfact->produce(method, nargs )->method(ORO_CORBA_PROTOCOL_ID, &orig, mpoa.in() ));
    } catch ( name_not_found_exception& nnf ) {
        throw ::RTT::corba::CNoSuchNameException( method );
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch (wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return ::RTT::corba::CMethod::_nil();
}

::RTT::corba::CMethod_ptr Orocos_CMethodInterface_i::createMethodAny (
    const char * method,
    const ::RTT::corba::CAnyArguments& args
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
  ))
{
    if ( mfact->hasMember( string( method ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( method );
    // convert Corba args to C++ args.
    MethodFactory::Arguments nargs;
    nargs.reserve( args.length() );
    for (size_t i =0; i != args.length(); ++i)
        nargs.push_back( new AnyDataSource( new CORBA::Any( args[i] ) ) );
    // create a local data source and a new method servant to serve it.
    try {
        MethodC orig(mfact, method);
        return (RTT::corba::CMethod_ptr)(mfact->produce(method, nargs )->method(ORO_CORBA_PROTOCOL_ID, &orig, mpoa.in() ));
    } catch ( name_not_found_exception& nnf ) {
        throw ::RTT::corba::CNoSuchNameException( method );
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch (wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return ::RTT::corba::CMethod::_nil();
}

// Implementation skeleton constructor
Orocos_CCommandInterface_i::Orocos_CCommandInterface_i (CommandFactory* gcf, PortableServer::POA_ptr the_poa)
    :mfact(gcf), mpoa( PortableServer::POA::_duplicate(the_poa) )
{
}

// Implementation skeleton destructor
Orocos_CCommandInterface_i::~Orocos_CCommandInterface_i (void)
{
}

PortableServer::POA_ptr Orocos_CCommandInterface_i::_default_POA()
{
    return PortableServer::POA::_duplicate(mpoa);
}

::RTT::corba::CCommandList * Orocos_CCommandInterface_i::getCommands (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    CCommandList_var rlist = new CCommandList();
    vector<string> flist = mfact->getNames();
    rlist->length( flist.size() );
    for (size_t i=0; i != flist.size(); ++i)
        rlist[i] = CORBA::string_dup( flist[i].c_str() );
    return rlist._retn();
}

::RTT::corba::CDescriptions * Orocos_CCommandInterface_i::getArguments (
      const char* command
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ))
{
    CDescriptions_var ret = new CDescriptions();
    if ( mfact->hasMember( string( command ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( command );
    // method found, convert args:
    CommandFactory::Descriptions args = mfact->getArgumentList( string(command) );
    ret->length( args.size() );
    for (size_t i =0; i != args.size(); ++i) {
        ret[i].name = CORBA::string_dup( args[i].name.c_str() );
        ret[i].description = CORBA::string_dup( args[i].description.c_str() );
        ret[i].type = CORBA::string_dup( args[i].type.c_str() );
    }
    return ret._retn();
}


 char * Orocos_CCommandInterface_i::getResultType (
      const char* command
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ))
{
    if ( mfact->hasMember( string( command ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( command );
    return CORBA::string_dup( mfact->getResultType( string(command) ).c_str() );
}

 char * Orocos_CCommandInterface_i::getDescription (
      const char* command
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ))
{
    if ( mfact->hasMember( string( command ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( command );
    return CORBA::string_dup( mfact->getDescription( string(command) ).c_str() );
}

::RTT::corba::CCommand_ptr Orocos_CCommandInterface_i::createCommand (
    const char * command,
    const ::RTT::corba::CArguments& args
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
  ))
{
  // Add your implementation here
    if ( mfact->hasMember( string( command ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( command );
    // convert Corba args to C++ args.
    // Use CommandC:
    try {
        CommandC comc(mfact, command);
        CommandC orig(mfact, command);
        for (size_t i =0; i != args.length(); ++i)
            comc.arg( DataSourceBase::shared_ptr(ExpressionProxy::Create( CExpression::_duplicate(args[i]) )) );
        // servant uses that object:
        Orocos_CCommand_i* com = new Orocos_CCommand_i( orig, comc, mpoa.in() );
        return com->_this();
    } catch ( name_not_found_exception& nnf ) {
        throw ::RTT::corba::CNoSuchNameException( command );
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch (wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return ::RTT::corba::CCommand::_nil();
}

::RTT::corba::CCommand_ptr Orocos_CCommandInterface_i::createCommandAny (
    const char * command,
    const ::RTT::corba::CAnyArguments& args
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    ,::RTT::corba::CNoSuchNameException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
  ))
{
  // Add your implementation here
    if ( mfact->hasMember( string( command ) ) == false )
        throw ::RTT::corba::CNoSuchNameException( command );
    // convert Corba args to C++ args.
    // Use CommandC:
    try {
        CommandC comc(mfact, string( command ) );
        CommandC orig(mfact, string( command ) );
        for (size_t i =0; i != args.length(); ++i)
            comc.arg( DataSourceBase::shared_ptr( new AnyDataSource( new CORBA::Any( args[i] ) )));
        // servant uses that object:
        Orocos_CCommand_i* com = new Orocos_CCommand_i( orig, comc, mpoa.in() );
        return com->_this();
    } catch ( name_not_found_exception& nnf ) {
        throw ::RTT::corba::CNoSuchNameException( command );
    } catch ( wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch (wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return ::RTT::corba::CCommand::_nil();
}

