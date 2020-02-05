//
// Created by dasa9 on 2020-01-27.
//
#include <string>
#include <regex>

using namespace std;
char * modifyWeb(char* input){
    string inputStr (input);
    string outputStr;

    // Regexes
    regex regflopp (" Floppy");
    regex regfloppUrl ("\".*[F|f]loppy.*[jpg|png|gif|jpeg]\"");
    regex regItaly (" Italy");

//    Strings
  const string trollUrl = "\"http://pages.cpsc.ucalgary.ca/~carey/CPSC441/trollface.jpg\"";
//    const string trollUrl = "\"./trollface.jpg\"";
    const string japanStr = " Japan";
    const string germanyStr = " Germany";
    const string trollyStr = " Trolly";
    outputStr = regex_replace(input, regfloppUrl, trollUrl);
    outputStr = regex_replace(outputStr, regflopp, trollyStr);
    outputStr = regex_replace(outputStr, regItaly, germanyStr);
    char outputC[];
    strcpy()
    return outputC;
}

//    string page1 = "This is a basic text file.\n"
//                   "It tells a simple story about my floppy-eared friend Floppy,\n"
//                   "who is from Italy. Floppy is a dog, I think.\n"
//                   "\n"
//                   "Without your proxy, you should be able to view this page just fine.\n"
//                   "\n"
//                   "With your proxy, this page should look a bit different,\n"
//                   "with all mentions of my friend Floppy from Italy\n"
//                   "being changed into something else.  \n"
//                   "The page should still be formatted properly.";
//
//    const string floppy = "Floppy";
//    const string trolly = "Trolly";
//
//    std::size_t pos =  page1.find(floppy);
//    while(pos != string::npos){
//        page1.replace(pos, floppy.length(), trolly);
//        pos =  page1.find(floppy);
//    }
//
//    cout << page1 << endl << endl;
//
//    string page2 = "This is a basic text file.\n"
//                   "It tells a simple story about my floppy-eared friend Floppy,\n"
//                   "who is from Italy. Floppy is a dog, I think.\n"
//                   "\n"
//                   "Without your proxy, you should be able to view this page just fine.\n"
//                   "\n"
//                   "With your proxy, this page should look a bit different,\n"
//                   "with all mentions of my friend Floppy from Italy\n"
//                   "being changed into something else.  \n"
//                   "The page should still be formatted properly.";
//
//    //using regex
//    regex e ("[f|F]loppy");
//    cout << regex_replace(page2,e,"Trolly");