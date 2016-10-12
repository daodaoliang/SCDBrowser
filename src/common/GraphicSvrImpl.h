#pragma once

#include <fstream>
#include <vector>

using namespace std;


class GraphicSvrImpl
{
public:
   virtual bool GetSignalIEDGraphic(const ::std::string& IEDKey, std::vector<unsigned char>& fileData);
   virtual bool GetTwoIEDGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, std::vector<unsigned char>& fileData);
   virtual bool GetIEDIndexGraphic(const ::std::string &IEDKey, std::vector<unsigned char>& fileData);

   //niuziming add 2013-06-14
   bool GetMsgFlowGraphic(const ::std::string &IEDKey, std::vector<unsigned char>& fileData);
   bool GetSubnetGraphic(const ::std::string &subnetKey, std::vector<unsigned char>& fileData);
   bool GetTerminalGraphic(const ::std::string &IEDKey, std::vector<unsigned char>& fileData);
   bool GetVirtualLoopGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, int sigType, std::vector<unsigned char>& fileData);

   bool GetMsgFlowDiffGraphic(const ::std::string &IEDKey, const ::std::string &otherIEDKey, std::vector<unsigned char>& fileData);
   bool GetVirtualLoopDiffGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, int sigType, std::vector<unsigned char>& fileData);
   bool GetTerminalDiffGraphic(const ::std::string& IEDKey, const ::std::string &otherIEDKey, std::vector<unsigned char>& fileData);
   GraphicSvrImpl();
   ~GraphicSvrImpl();
};

