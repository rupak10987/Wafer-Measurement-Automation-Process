#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include<deque>
using namespace std;
bool monitorMeasurement = true;//global variable which can be accessed by all classes and will be set by user choice
bool selectYesNo(string prompt)
{
    cout<<prompt;
    char c;
    cin>>c;
    if(c=='y')
        return true;
    else if (c == 'n')
        return false;
    else
        return true;
}

class Wafer
{
private:
    int id;
    string material;
    float allignment;//degree//for robot arm to allign it
    //measurementTool specific data which will be generated with rndaom data and will be read by measurementTools
    float roughness;
    float thickness;
    float filmthickness;
    float stress;
    float resistance;

public:
    Wafer(int id, string material = "silicon"):id(id),material(material) {}
    int getID()
    {
        return this->id;
    }
    string getMaterial()
    {
        return this->material;
    }
    string info()
    {
        return "(" + to_string(id) + ", " + material + ")";
    }

    void setProperties()
    {
        this->roughness =  static_cast<float>(rand()) / RAND_MAX * 100.0f;
        this->thickness =  static_cast<float>(rand()) / RAND_MAX * 100.0f;
        this->filmthickness =  static_cast<float>(rand()) / RAND_MAX * 100.0f;
        this->stress =  static_cast<float>(rand()) / RAND_MAX * 100.0f;
        this->resistance =  static_cast<float>(rand()) / RAND_MAX * 100.0f;
        this->allignment =  static_cast<float>(rand()) / RAND_MAX * 360.0f;
    }
    void setAllignment(float val)
    {
        allignment = val;
    }
    float getAllignment()
    {
        return allignment;
    }
    float getRoughness()
    {
        return roughness;
    }
    float getThickness()
    {
        return thickness;
    }
    float getResistance()
    {
        return resistance;
    }
    float getStress()
    {
        return stress;
    }
    float getFilmThickness()
    {
        return filmthickness;
    }

};

class Cassette
{
private:
    deque<Wafer*> wafers;
    Wafer* getTopWafer() //this will pick up the first wafer from the cassette by robotArm only
    {
        if(wafers.empty()) return nullptr;
        Wafer* w = wafers.front();
        wafers.pop_front();
        return w;
    }
    void addWafer(Wafer* wafer) //for initially populating and also by robotArm
    {
        wafers.push_back(wafer);
    }
public:
    Cassette(int initial_number)//initially have some wafers in the cassette
    {
        for(int i = 1; i <= initial_number; i++)
        {
            Wafer* w = new Wafer(i);
            w->setProperties();//initializing some properties which will be measured by measurement tools
            addWafer(w);
        }
    }
    friend class RobotArm; //only robotArm can access wafers in cassette
};

class ResultStorage
{
private:
    string result;
public:
    void appendResultData(string result_segment)
    {
        result.append(result_segment);
    }
    void saveResult()
    {
        bool save = selectYesNo("Save Result In File(y/n): ");
        if(!save)
            return;
        ofstream file("results.txt", ios::app);
        file << result << endl;
        cout << "Saved result to file " << "results.txt" << endl;
        file.close();
        cout<<"Result Data: "<<result<<endl;
        result.clear();
    }
};

class MeasurementTool
{
protected:
    string name;
    Wafer* heldWafer = nullptr;
    Wafer* giveWaferToRobot()
    {
        if(heldWafer!=nullptr)
        {
            Wafer* temp = heldWafer;
            heldWafer = nullptr;
            return temp;
        }
        return nullptr;
    }
public:
    MeasurementTool(string n):name(n) {}
    virtual void measure(ResultStorage& save) = 0;
    string getName()
    {
        return name;
    }
    friend class RobotArm;
};

class ThicknessTool : public MeasurementTool
{
public:
    ThicknessTool() : MeasurementTool("Thickness Tool") {}
    void measure(ResultStorage& save) override
    {
        if (monitorMeasurement)
        {
            cout << "Measuring Thickness of " << heldWafer->info() << endl;
            cout<< "Thickness: "<<heldWafer->getThickness()<<endl;
        }
        string result_string = heldWafer->info() + " - Thickness:" + to_string(heldWafer->getThickness()) + ", ";
        save.appendResultData(result_string);
    }
};

class RoughnessTool : public MeasurementTool
{
public:
    RoughnessTool() : MeasurementTool("Roughness Tool") {}
    void measure(ResultStorage& save) override
    {
        if (monitorMeasurement)
        {
            cout << "Measuring Roughness of " << heldWafer->info() << endl;
            cout<< "Roughness: "<<heldWafer->getRoughness()<<endl;
        }
        string result_string = heldWafer->info() + " - Roughness:" + to_string(heldWafer->getRoughness()) + ", ";
        save.appendResultData(result_string);
    }
};

class ResistanceTool : public MeasurementTool
{
public:
    ResistanceTool() : MeasurementTool("Resistance Tool") {}
    void measure(ResultStorage& save) override
    {
        if (monitorMeasurement)
        {
            cout << "Measuring Resistance of " << heldWafer->info() << endl;
            cout<< "Resistance: "<<heldWafer->getResistance()<<endl;
        }

        string result_string = heldWafer->info() + " - Resistance:" + to_string(heldWafer->getResistance()) + ", ";
        save.appendResultData(result_string);
    }
};

class FilmThicknessTool : public MeasurementTool
{
public:
    FilmThicknessTool() : MeasurementTool("FilmThickness Tool") {}
    void measure(ResultStorage& save) override
    {
        if (monitorMeasurement)
        {
            cout << "Measuring FilmThickness of " << heldWafer->info() << endl;
            cout<< "Film Thickness: "<<heldWafer->getFilmThickness()<<endl;
        }

        string result_string =heldWafer->info() + " - Film Thickness" + to_string(heldWafer->getFilmThickness()) + ", ";
        save.appendResultData(result_string);
    }
};

class StressTool : public MeasurementTool
{
public:
    StressTool() : MeasurementTool("Stress Tool") {}
    void measure(ResultStorage& save) override
    {
        if (monitorMeasurement)
        {
            cout << "Measuring Stress of " << heldWafer->info() << endl;
            cout<< "Stress: "<<heldWafer->getStress()<<endl;
        }

        string result_string = heldWafer->info() + " - Resistance:" + to_string(heldWafer->getStress()) + ", ";
        save.appendResultData(result_string);
    }
};


class RobotArm
{
    Wafer* heldWafer = nullptr;
public:
    void allignWafer()
    {
        if (heldWafer!=nullptr)
        {
            if (monitorMeasurement)
                cout<<"alligning wafer("<<heldWafer->info()<<")" << " " <<heldWafer->getAllignment()<<" -> ";
            heldWafer->setAllignment(0.0);
            if (monitorMeasurement)
                cout<<heldWafer->getAllignment()<<endl;
        }
    }
    void pickWaferFromTool(MeasurementTool& tool)
    {
        heldWafer = tool.giveWaferToRobot();
        if (monitorMeasurement)
            cout << "Robot picked wafer" << heldWafer->info() <<" from "<<tool.getName()<<endl;
    }
    void pickWaferFromCassette(Cassette& cassette)
    {
        heldWafer = cassette.getTopWafer();
        if (monitorMeasurement)
            cout << "Robot picked wafer" << heldWafer->info() <<" from cassette"<< endl;
    }

    void giveWaferToTool(MeasurementTool &tool)
    {
        if(heldWafer != nullptr)
        {
            if (monitorMeasurement)
                cout << "Robot handing wafer"<<heldWafer->info() <<" to "<< tool.getName()<<endl;
            tool.heldWafer = this -> heldWafer;
            heldWafer = nullptr;
        }
    }

    void returnToCassette(Cassette& cassette)
    {
        if (heldWafer != nullptr)
        {
            Wafer* temp = heldWafer;
            heldWafer = nullptr;
            cassette.addWafer(temp);
            if (monitorMeasurement)
                cout << "Robot returned wafer: "<<temp->info()<< " to cassette"<<endl;
        }
    }
};

class MeasurementManager
{
    Cassette cassette;//automatically populated with wafers via constructor
    RobotArm robot_arm;
    vector<unique_ptr<MeasurementTool>> tools;//unique_ptr cause measurementTools is an abstract class
    vector<int>choices;
    int choice;
    ResultStorage result_storage;
public:
    MeasurementManager():cassette(3)
    {
        tools.push_back(make_unique<ThicknessTool>());//#c1
        tools.push_back(make_unique<StressTool>());//#c2
        tools.push_back(make_unique<RoughnessTool>());//#c3
        tools.push_back(make_unique<ResistanceTool>());//#c4
        tools.push_back(make_unique<FilmThicknessTool>());//#c5
        //populate the cas
    }
    bool promptUserForChoice()
    {
        cout<<"Measurement Menu\n";
        bool notExit = selectYesNo("Start Measurement(y/n)?: ");
        if (!notExit)
            return true;
        cout<<"Enter Measurement Choices(Can Provide Multiple Choices Separated by Space)"<<endl;
        cout<<"0-All, 1-Thickness, 2-Stress, 3-Roughness, 4-Resistance, 5-Film Thickness\n";

        while(true)
        {
            if (choice>=6 || choice<0)
                return true;//exit
            cin>>choice;
            if (cin.peek()=='\n')
            {
                cout<<"\n\n------------Starting Measurements------------\n\n";
            }
            if(choice == 0)//space separated
            {
                monitorMeasurement=selectYesNo("\nMonitor While Measurement Ongoing(y/n)? ");
                if (monitorMeasurement)
                    cout<<"------------------Monitoring-----------------\n";
                doAllMeasurementProcss();
                cout<<"\n--------------Measurement finished--------------\n";
                return false;
            }
            bool flag = false;
            for(int i=0; i<choices.size(); i++)
            {
                if (choices[i]==choice || choice>5)//dont record duplicate or invalid choices
                {
                    flag = true;
                    break;
                }

            }
            if(!flag)
                choices.push_back(choice);
            if(cin.peek() == '\n')
                break;
        }
        monitorMeasurement=selectYesNo("\nMonitor While Measurement Ongoing(y/n)?");
        if (monitorMeasurement)
            cout<<"------------------Monitoring-----------------\n";
        doSelectiveMeasurementProcss();
        cout<<"\n-------------Measurement finished-------------\n";
        return false;
    }

    void doSelectiveMeasurementProcss()
    {
        robot_arm.pickWaferFromCassette(cassette);
        robot_arm.allignWafer();
        for (int i:choices)
        {
            robot_arm.giveWaferToTool(*tools[i-1]);
            (*tools[i-1]).measure(result_storage);
            robot_arm.pickWaferFromTool(*tools[i-1]);
        }
        robot_arm.returnToCassette(cassette);
        result_storage.saveResult();
    }
    void doAllMeasurementProcss()
    {
        robot_arm.pickWaferFromCassette(cassette);
        robot_arm.allignWafer();
        for(int i=0; i<tools.size(); i++)
        {
            robot_arm.giveWaferToTool(*tools[i]);
            (*tools[i]).measure(result_storage);
            robot_arm.pickWaferFromTool(*tools[i]);
        }
        robot_arm.returnToCassette(cassette);
        result_storage.saveResult();
    }

};
int main()
{
    srand(time(nullptr));
    MeasurementManager manager;
    while(true)
    {
        bool can_exit = manager.promptUserForChoice();
        if (can_exit)
            break;
        cout<<endl;
    }
    return 0;
}
