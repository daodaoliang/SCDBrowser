#pragma once

#include "SliceSCLModelInterface.h"
#include <fstream>

using namespace SCLModel;
using namespace std;


class GraphicSvr :public SliceGraphicsInterface
{
public:
   virtual bool GetSignalIEDGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& = ::Ice::Current());
   virtual bool GetTwoIEDGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& = ::Ice::Current());
   virtual bool GetIEDIndexGraphic(const ::std::string &IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& = ::Ice::Current());

   virtual bool GetSubnetGraphic(const ::std::string& subNetKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& /* = ::Ice::Current */);
   virtual bool GetMsgFlowGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& /* = ::Ice::Current */);
   virtual bool GetVirtualLoopGraphic(const ::std::string& IEDKeyPri, const ::std::string& IEDKeySec, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& /* = ::Ice::Current */);
   virtual bool GetTerminalGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& /* = ::Ice::Current */);
 
   virtual bool GetGraphicByGraphicName(const ::std::string&, ::SCLModel::ByteSeq&, const ::Ice::Current& = ::Ice::Current());
   virtual bool GetGraphicNamesOfMaintenanceIEDs(const ::SCLModel::NameSeq&, ::SCLModel::GraphicInfoSeq&, const ::Ice::Current& = ::Ice::Current());
   virtual bool GetGraphicNamesOfIED(const ::std::string&, ::SCLModel::GraphicInfoSeq&, const ::Ice::Current& = ::Ice::Current());

   GraphicSvr(void);
   ~GraphicSvr(void);
};

