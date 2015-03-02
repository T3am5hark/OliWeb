#include "Agent.h"
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "stringTo.h"
#include "GpioPwm.h"

using namespace std;
using namespace tinyxml2;

namespace RaspiController
{

  Agent::Agent()
  {
  }

  /// Agent::Agent(const string &agentName)
  /// Init with name (name is used to configure from XML)
  Agent::Agent(const string &agentName)
  {
      name = agentName;
      listener.fifoName="/tmp/"+name;
      waitStruct.tv_sec = 0;
      waitStruct.tv_nsec = (time_t)(50*1000000); // default 50ms
  }

  void Agent::run()
  {
      writeLog("Creating fifo buffer: " + listener.fifoName);
      FifoReader::createSystemFifo( listener.fifoName, O_CREAT | 0666);
      writeLog("Opening fifo");
      listener.openFifo();
      writeLog("Initializing Gpio.");
      GpioPwm::setupIo();
      writeLog("Running agent.");
      while (1)
      {
          if ( readFifo() ) handleMessage();
          nanosleep(&waitStruct,NULL);
      }
      listener.closeFifo();
  }

  /// Agent::readFifo()
  /// Read from the fifo buffer and populate the message string
  bool Agent::readFifo()
  {
      message = listener.readFifo();
      return (message.length() > 0);
  }

  void Agent::handleMessage()
  {
      parseTokens();
      writeLog("Received command: " + message);
      executeCommand();

  }

  void Agent::parseTokens()
  {
      //clearTokens();
      istringstream str(message);
      for (int i = 0; i < MAX_TOKENS; i++)
      {
          str >> tokens[i];
          //if ( tokens[i].length() == 0 ) break;
      }

  }

  void Agent::executeCommand()
  {
      Command *cmd = state->getCommand(tokens[0]);

      // Should log this as invalid command for this state
      if (cmd == NULL) return;

      int nSteps = cmd->actionSequence.size();

      //for (vector<Action>::iterator it = cmd->actionSequence.begin();
      //     it != cmd->actionSequence.end(); ++it )
      for (int i = 0; i < nSteps; i++)
      {
          Action &action = cmd->actionSequence[i];
          if (action.type == ActionType::HaltExecution) break;
          if (action.type == ActionType::GoTo)
          {
              int stepNumber = max(1,stringTo<int>(action.parameters[0]));
              i = stepNumber-2; // minus two for autoincrement and zero-indexing
          }
          executeAction(action);
      }
  }

  void Agent::executeAction(const Action &action)
  {
          switch (action.type)
          {
              case ActionType::TransitionToState :
                  break;
              case ActionType::CameraSnapshot:
                  break;
              case ActionType::SendPulseTrainToPin :
                   sendPulseTrainToPin(action);
                   break;
              case ActionType::SendPulseTrainToPins :
                   //sendPulseTrainToPins(action);
                   break;
              case ActionType::SetPinState :
                  break;
              case ActionType::GoTo :
                  break;
              case ActionType::HaltExecution :
                  break;
              case ActionType::Debug :
                  debug();
                  break;
              default:
                  break;
          }
  }

  void Agent::debug()
  {
      writeLog("**** DEBUG ****");
      writeLog("Current State: " + state->name);
      writeLog("***************");

  }

  void Agent::sendPulseTrainToPin(const Action &action)
  {
    int pin = stringTo<int>(action.parameters[0]);
    int periodMicroSeconds = stringTo<int>(action.parameters[1]);
    int nPulses = stringTo<int>(action.parameters[2]);
    double dutyCycle = stringTo<double>(action.parameters[3]);
    GpioPwm::pulse( pin, periodMicroSeconds, nPulses, dutyCycle );
  }

  void Agent::sendPulseTrainToPins(const Action &action)
  {

  }

  void Agent::clearTokens()
  {
      for (int i = 0; i < MAX_TOKENS; i++) tokens[i] = "";
  }

  void Agent::addState(const State &aState)
  {
      // Should check this for collisions!
      stateMetaData[aState.name] = aState;
  }

  Command *State::getCommand(const string &name)
  {
      return NULL;
      //map<string,Command>::iterator it = validCommands.find(name);
      //if (it == validCommands.end()) return NULL;
      //return &(it->second);
  }

  Action::Action(XMLElement *xml)
  {
      cout << "Some Action" << endl;
  }

  Command::Command()
  {

  }

  Command::Command(XMLElement *xml)
  {
      XMLElement *actionXml = xml->FirstChildElement("Action");

      while (actionXml)
      {
          cout << "Action!" << endl;
          Action action(actionXml);
          actionXml = actionXml->NextSiblingElement("Action");
      }
  }

  State::State()
  {
  }

  State::State(XMLElement *xml)
  {

      name = xml->Attribute("name");

      XMLElement *cmdXml = xml->FirstChildElement("Command");

      while (cmdXml)
      {
          string cmdName = cmdXml->Attribute("name");
          cout << "Adding command: " << cmdName << endl;
          Command command(cmdXml);
          //validCommands.Add(cmdName, command);
          validCommands[cmdName] = command;
          cmdXml = cmdXml->NextSiblingElement("Command");
      }
  }

  void Agent::configureFromXml(XMLElement *agentElement)
  {
      // Configure states first
      XMLElement *aState = agentElement->FirstChildElement("States")->
                           FirstChildElement("State");
      while (aState)
      {
          string stateName = aState->Attribute("name");
          cout << "Adding state: " << stateName << endl;
          State state(aState);
          //stateMetaData.Add(stateName, state);
          stateMetaData[stateName] = state;
          aState = aState->NextSiblingElement("State");
      }
      defaultState = &(stateMetaData.begin()->second);
      string defaultStateName = agentElement->Attribute("defaultState");
      if ( defaultStateName.length() > 0 )
      {
         map<string, State>::iterator it = stateMetaData.find( defaultStateName );
         if ( it != stateMetaData.end() ) defaultState=&(it->second);
      }
      state = defaultState;
      cout << "InitialState = " + state->name << endl;

  }

  void Agent::configureFromXmlFile(const string &filename)
  {
      writeLog("Opening \"" + filename+ "\"");

      config.LoadFile(filename.c_str());

      writeLog("Errorcode = " + toString(config.ErrorID()));

      if ( config.ErrorID() > 0 )
      {
        string err1 = config.GetErrorStr1();
        string err2 = config.GetErrorStr2();
        //string err2 = config.ErrorDesc();
        writeLog(err1);
        writeLog(err2);

        config.PrintError();
      }

      XMLElement *root = config.RootElement();
      if ( root == NULL )
      {
          writeLog("Error parsing file - no root element found");
          return;
      }

      XMLElement *agentElement = root->FirstChildElement("Agents")->
                                 FirstChildElement("Agent");

      while (agentElement)
      {
          string agentName = agentElement->Attribute("name");
          writeLog(agentName);
          if ( agentName == name )
          {
              writeLog("Found Agent \"" + agentName + "\"");
              configureFromXml(agentElement);
              return;
          }
          agentElement=agentElement->NextSiblingElement("Agent");
      }
      writeLog("Could not find Agent config with name \"" + name + "\"");

  }

  void Agent::writeLog(const string &message)
  {
      cout << message << endl << flush;
  }

}
