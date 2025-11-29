#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Including Libraries
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using json = nlohmann::json;
using namespace std;

//Registrations
void vehicleRegistration(string vehicleNum,string wheels, string owner,string email,string address);
void accRegistration(string owner,string email, string password, string address, int money = 0);

//Bank
bool accExist(const string& email);
bool bankLogin(string email, string password);
void moneyDeposit(string email, int deposit);
void moneyCredit(string email, int credit);

#endif