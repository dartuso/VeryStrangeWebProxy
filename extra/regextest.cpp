//
// Created by Daniel on 2020-02-02.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

const string trollUrl = "\"http://pages.cpsc.ucalgary.ca/~carey/CPSC441/trollface.jpg\"";
const string japanStr = " Japan";
const string germanyStr = " Germany";
const string trollyStr = "Trolly";
const string lengthHeader = "Content-Length: ";
int main() {
    string header1 = "HTTP/1.1 200 OK\r\nDate: Sun, 02 Feb 2020 19:42:37 GMT\r\nServer: Apache/2.2.15 (Scientific Linux)\r\nLast-Modified: Fri, 10 Jan 2020 04:34:42 GMT\r\nETag: \"4424780-395-59bc1a4ff73cf\"\r\nAccept-Ranges: bytes\r\nContent-Length: 917\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<html>\n\n<title>\nHTML Test Page 3 about Floppy for CPSC 441 Assignment 1\n</title>\n\n<body>\n\n<h1>HTML Test Page 3 for Floppy from Italy</h1>\n\n<p>\n  This is a rather large and complicated Web page.\n  It tells a simple story about my floppy-eared friend Floppy,\n  who is from Italy. Floppy is a dog, I think.\n</p>\n<p>\n  Here are two pictures of Floppy, which I took when I was\n  in Italy in December, plus a picture of me, which Floppy took:\n</p>\n<p>\n  <img src=\"./Garden-Floppy.jpg\" width=\"200\" height=\"400\">\n  <img src=\"./Potty-Floppy.jpg\" width=\"200\" height=\"400\">\n  <img src=\"./Carey-Italy.jpg\" width=\"400\" height=\"400\">\n</p>\n\n<p>\nWithout your proxy, you should be able to view this page just fine.\n</p>\n<p>\nWith your proxy, this page should look a bit different,\nwith all mentions of my friend Floppy from Italy\nbeing changed into something else.  \nThe page should still be formatted properly.\n</p>\n\n</body>\n\n</html>\n";


    string inputStr = header1;
    string outputStr;
    // Regexes
    regex regflopp ("Floppy");
    regex regfloppUrl ("\".*Floppy.*[jpg|png|gif|jpeg]\"");
    regex regItaly ("Italy(?!\\.)");
    regex regContentLength ("Content-Length: \\d*\r\n");


    //Perform
    outputStr = regex_replace(inputStr, regfloppUrl, trollUrl);
    outputStr = regex_replace(outputStr, regflopp, trollyStr);
    outputStr = regex_replace(outputStr, regItaly, japanStr);

//    string lengthStr =  to_string(outputStr.length());
    string newHeader = lengthHeader + to_string(outputStr.length()) + "\r\n";
    outputStr = regex_replace(outputStr,regContentLength,newHeader);


    cout << outputStr;
    return 0;
}