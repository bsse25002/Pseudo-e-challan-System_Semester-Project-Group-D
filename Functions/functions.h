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

//DMA for Payment Management
void getSize(string file, int &rows, int &cols);
void toArray(string file, string **arr, int rows, int cols);
class Vector;

//Admin Controls
bool adminExist(string admin);
bool adminLogin(string admin, string password);

//Registrations
void vehicleRegistration(string vehicleNum,int wheels, string owner,string email,string address);
void accRegistration(string owner,string email, string password, string address, int money = 0);
void registeredVehicles();

//Finances
bool accExist(const string& email);
bool bankLogin(string email, string password);
void moneyDeposit(string email, int deposit);
void moneyCredit(string email, int credit);
void payment(string email, string **acc, int rowsAcc, string **challan, int rowsCh);

//Challans
string vehicleExist(string vehicleNum);

#endif