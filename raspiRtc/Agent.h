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
      SendPulseTrainToPins = 4,
      SetPinState = 5,
      GoTo = 6,
      HaltExecution=7,
      Wait = 8,
      OnPinStateDo=9,
      System=10,
      MessageToAgent=11,
      Broadcast=12,
      Debug=13
  };

  #define MAX_TOKENS 10

  class Action
  {
      public:
      Action(XMLElement *xml);
      ActionType type;
      vector<string> parameters;
  };

  class Command
  {
      public:

      Command(XMLElement *xml);
      Command();
      void addAction(const string &name, const Action &action);
      void run(string *parameters[]);
      vector<Action> actionSequence;
  };

  class State
  {
      public:

      State(XMLElement *xml);
      State();
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
      State *defaultState;
      map<string, State> stateMetaData;
      FifoReader listener;
      void executeAction(const Action &action);
      bool readFifo();
      void handleMessage();
      void parseTokens();
      void clearTokens();
      void executeCommand();
      void debug();
      void sendPulseTrainToPin(const Action &action);
      void sendPulseTrainToPins(const Action &action);
      struct timespec waitStruct;
      string tokens[MAX_TOKENS];
      string message;
      XMLDocument config;
  };

}

#endif
