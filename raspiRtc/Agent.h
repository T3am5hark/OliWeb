#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

#include <map>
#include <vector>
#include <string>
#include "FifoReader.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace RaspiController
{

  enum ActionType
  {
      TransitionToState = 1,
      CameraSnapshot = 2,
      SendPulseTrainToPin = 3,
      SetPinState = 4,
      GoTo = 5,
      HaltExecution=6,
      Wait = 7,
      OnPinStateDo=8,
      System=9,
      MessageToAgent=10
  };

  #define MAX_TOKENS 10

  class Action
  {
      public:
      ActionType type;
      vector<string> parameters;
  };

  class Command
  {
      public:
      void addAction(const string &name, const Action &action);
      void run(string *parameters[]);
      vector<Action> actionSequence;
  };

  class State
  {
      public:

      string name;
      void addCommand(string name, Command cmd);
      Command *getCommand(const string &name);

      private:

      map<string,Command> validCommands;

  };

  class Agent
  {
      public:

      string name;
      Agent(const string &agentName);
      Agent();

      void configureFromXmlFile(const string &filename);
      void configureFromXml(XMLElement *agentElement);
      void run();
      void addState(const State &aState);

      private:
      void writeLog(const string &message);
      State *state;
      map<string, State> stateMetaData;
      FifoReader listener;
      void executeAction(const Action &action);
      bool readFifo();
      void handleMessage();
      void parseTokens();
      void clearTokens();
      void executeCommand();
      void sendPulseTrainToPin(const Action &action);
      struct timespec waitStruct;
      string tokens[MAX_TOKENS];
      string message;
      XMLDocument config;
  };

}


#endif
