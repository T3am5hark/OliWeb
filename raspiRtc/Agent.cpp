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

  Agent::Agent(const string &agentName)
  {
      name = agentName;
      listener.fifoName="/tmp/"+name;
      waitStruct.tv_sec = 0;
      waitStruct.tv_nsec = (time_t)(50*1000000); // default 50ms
  }

  void Agent::run()
  {
      FifoReader::createSystemFifo( listener.fifoName, 0777);
      listener.openFifo();
      GpioPwm::setupIo();
      while (1)
      {
          if ( readFifo() ) handleMessage();
          nanosleep(&waitStruct,NULL);
      }
      listener.closeFifo();
  }

  bool Agent::readFifo()
  {
      message = listener.readFifo();
      return (message.length() > 0);
  }

  void Agent::handleMessage()
  {
      parseTokens();
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

      executeCommand();
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
              case ActionType::SetPinState :
                  break;
              case ActionType::GoTo :
                  break;
              case ActionType::HaltExecution :
                  break;
              default:
                  break;
          }
  }

  void Agent::sendPulseTrainToPin(const Action &action)
  {
    int pin = stringTo<int>(action.parameters[0]);
    int periodMicroSeconds = stringTo<int>(action.parameters[1]);
    int nPulses = stringTo<int>(action.parameters[2]);
    double dutyCycle = stringTo<double>(action.parameters[3]);
    GpioPwm::pulse( pin, periodMicroSeconds, nPulses, dutyCycle );
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

  void Agent::configureFromXml(XMLElement *agentElement)
  {
      
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

      /*
      XMLElement *controllerElement = root->FirstChildElement("Controller");
      if ( controllerElement == NULL)
      {
          writeLog("Did not find controller element.");
          return;
      }*/

      //XMLElement *agentElement = controllerElement->FirstChildElement("Agent");
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
          agentElement=agentElement->NextSiblingElement();
      }
      writeLog("Could not find Agent config with name \"" + name + "\"");

  }

  void Agent::writeLog(const string &message)
  {
      cout << message << endl;
  }

}
