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
void accRegistration(string owner,string email,string address);

#endif