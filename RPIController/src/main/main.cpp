/****************************************************************************
  FileName     [ main.cpp ]
  PackageName  [ main ]
  Synopsis     [ main() for modCalc ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"
#include "dbJson.h"
using namespace std;

//----------------------------------------------------------------------
//    Global cmd Manager
//----------------------------------------------------------------------
CmdParser* cmdMgr = new CmdParser("");
/*
void sendToRPIClient(const bool& ok, const string& msg) {
    //const string = "{\"OK\":1,\"msg\":\"Start Playing\"}";
    cout << "{\"OK\":" ok << ",\"msg\":\"" << msg << "\"}";
}
 */
extern bool initCommonCmd();
extern bool initRPICmd();
extern RPIMgr rpiMgr;

// global
bool playing = false;
size_t startTime = 0;
//

//*
void sigHandler(int sig) {
    rpiMgr.pause();
    //cout << "startTime: " << rpiMgr.getStartTime() << endl;
}
//*/


static void
usage()
{
   cout << "Usage: modCalc [ -File < doFile > ]" << endl;
}

static void
myexit()
{
   usage();
   exit(-1);
}

int
main(int argc, char** argv)
{
   ifstream dof;
    signal(SIGINT, sigHandler);
   if (argc == 3) {  // -file <doFile>
      if (myStrNCmp("-File", argv[1], 2) == 0) {
         if (!cmdMgr->openDofile(argv[2])) {
            cerr << "Error: cannot open file \"" << argv[2] << "\"!!\n";
            myexit();
         }
      }
      else {
         cerr << "Error: unknown argument \"" << argv[1] << "\"!!\n";
         myexit();
      }
   }
   else if (argc != 1) {
      cerr << "Error: illegal number of argument (" << argc << ")!!\n";
      myexit();
   }

   if (!initCommonCmd() || !initRPICmd())
      return 1;

   CmdExecStatus status = CMD_EXEC_DONE;
    cout << "start success" << endl;
   while (status != CMD_EXEC_QUIT) {  // until "quit" or command error
      status = cmdMgr->execOneCmd();
      cout << endl;  // a blank line between each command
   }

   return 0;
}