#pragma once
#include "ofMain.h"

class sysCommand : public ofThread {
public :
	void callCommand(string command) {
		cmd = command;
		//stopThread();
		startThread();
	}

	////--------------------------------------------------------------
	//sysCommand::sysCommand() {
	//}

	////--------------------------------------------------------------
	//sysCommand::~sysCommand() {
	//	waitForThread(true);
	//}

private:
	void threadedFunction() {
		//string cmd = "start data\\test.bat"; // create the command
		cout << cmd << endl;
		cout << ofSystem(cmd.c_str()) << endl;
		//ofSystem(cmd.c_str());
		//string cmdExit = "exit";
		//ofSystem(cmdExit.c_str());
		//ofLog(OF_LOG_VERBOSE, "call " + cmd);
		//string result = exec((char *)cmd.c_str());
		//ofLog(OF_LOG_VERBOSE, "RESULT " + result);
		//stopThread();
		//ofNotifyEvent(commandComplete, result, this);
	}

	//--------------------------------------------------------------
	void exit() {
		// stop the thread on exit
		waitForThread(true);
	}

	string cmd;
};
