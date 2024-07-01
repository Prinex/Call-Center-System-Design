#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include <random>

/**
 *  classes needed:
 *  -have a call center class which should contain the below
 *  -the call center class should contain a dispatchCall which assigns a call the first available employee
 *  -the dispatchCall should receive a class called Call which will be dispatched
 *  -so a class Call should be created which will contain the following properties: phone number, and type of call as stated below
 *  -an abstract method/interface employee
 *  -the abstract class should have a bool var of whether or not the employee is busy with a call
 *  -the abstract class should have pure virtual function which will be overriden in the child classes
 *  which will simply return the role name, i.e., respondent, manager, director
 *  -child classes of employee: respondent, manager, and director
 *  -the child classes should contain a variable that should tell if the employee is not able to handle it, should be escalated to a director, here should have 3 levels of calls basic, detailed, and classified information being handled with an enum class
 *
 *
*/

enum class CallType { Basic, Detailed, Classified };

enum class EmployeeType { RespondentEmp, ManagerEmployee, DirectorEmployee };

class Employee
{
protected:
    int _id;
    bool _isBusy = false;

public:
    Employee(int id) { _id = id; };

    virtual ~Employee() {};

    // this method should get the type of employe with its id no
    virtual std::string GetEmployee() = 0;

    int GetEmployeeId() const { return _id; };

    bool GetIsBusy() const { return _isBusy; }

    void SetEmployeeBusy(const bool isBusy) { _isBusy = isBusy; }

};

class Respondent : public Employee
{
public:
    static CallType callType;

    Respondent(int id) : Employee(id) {};

    ~Respondent() {};

    std::string GetEmployee() override
    {
        std::stringstream ss;
        ss << _id;
        std::string e = "Respondent with id ";
        ss >> e;
        return e;
    }

};
CallType Respondent::callType = CallType::Basic;

class Manager : public Employee
{
public:
    static CallType callType;

    Manager(int id) : Employee(id) {};

    ~Manager() {};

    std::string GetEmployee() override
    {
        std::stringstream ss;
        ss << _id;
        std::string e = "Manager with id ";
        ss >> e;
        return e;
    }
};
CallType Manager::callType = CallType::Detailed;

class Director : public Employee
{
public:
    static CallType callType;

    Director(int id) : Employee(id) {};

    ~Director() {};

    std::string GetEmployee() override
    {
        std::stringstream ss;
        ss << _id;
        std::string e = "Director with id ";
        ss >> e;
        return e;
    }
};
CallType Director::callType = CallType::Classified;

class Call
{
private:
    std::string _phoneNo;
    CallType _callType;

public:
    Call(const std::string phoneNo, const CallType callType) :
        _phoneNo(phoneNo),
        _callType(callType)
    {};

    ~Call() {};

    std::string GetPhoneNo() const { return _phoneNo; };

    CallType GetCallType() const { return _callType; };
};

// can have an hr class for hiring the employees but will create the instances of employees needed in the main method
// for the sake of time
// also this class can be implemented using a facory class

class HR
{
public:
    static std::vector<Employee*> HireEmployees(int noOfEmployees, EmployeeType type)
    {
        std::vector<Employee*> employees;
        Employee* emp;
        for (int i = 1; i <= noOfEmployees; i++)
        {
            if (type == EmployeeType::RespondentEmp)
            {
                Employee* emp = new Respondent(i);
                employees.push_back(emp);
            }
            if (type == EmployeeType::ManagerEmployee)
            {
                Employee* emp = new Manager(i);
                employees.push_back(emp);
            }
            if (type == EmployeeType::DirectorEmployee)
            {
                Employee* emp = new Director(i);
                employees.push_back(emp);
            }

        }
        return employees;
    };
};

class CallCenter
{
private:
    std::string _name;
    std::vector<Employee*> _respondents;
    std::vector<Employee*> _managers;
    std::vector<Employee*> _directors;

public:
    CallCenter(std::string name, std::vector<Employee*> respondents, std::vector<Employee*> managers, std::vector<Employee*> directors) :
        _name(name), _respondents(respondents), _managers(managers), _directors(directors)
    {}

    void dispatchCall(std::vector<Call*> calls)
    {
        std::string typeOfInfo = "";
        for (size_t i = 0; i < calls.size(); i++)
        {
            if ((*calls[i]).GetCallType() == CallType::Basic)
                typeOfInfo = "basic";
            if ((*calls[i]).GetCallType() == CallType::Detailed)
                typeOfInfo = "detailed";
            if ((*calls[i]).GetCallType() == CallType::Classified)
                typeOfInfo = "classified";

            // first dispatch the call to the employee according to the level of call type
            if ((*calls[i]).GetCallType() == CallType::Basic)
            {
                for (size_t i = 0; i < _respondents.size(); i++)
                {
                    if ((*_respondents[i]).GetIsBusy() == false)
                    {
                        (*_respondents[i]).SetEmployeeBusy(true);
                        std::cout << "Call from " << (*calls[i]).GetPhoneNo() << " with the subject on "
                            << typeOfInfo << " information " << "was assigned to respondant with id "
                            << (*_respondents[i]).GetEmployeeId()
                            << std::endl;
                        break;
                    }
                }
                continue;
            }
            if ((*calls[i]).GetCallType() == CallType::Detailed || (*_respondents[_respondents.size() - 1]).GetIsBusy() == true)
            {
                for (size_t i = 0; i < _managers.size(); i++)
                {
                    if ((*_managers[i]).GetIsBusy() == false)
                    {
                        (*_managers[i]).SetEmployeeBusy(true);
                        std::cout << "Call from " << (*calls[i]).GetPhoneNo() << " with the subject on "
                            << typeOfInfo << " information " << "was escalated to manager with id "
                            << (*_managers[i]).GetEmployeeId()
                            << std::endl;
                        break;
                    }
                }
                continue;
            }
            if ((*calls[i]).GetCallType() == CallType::Classified || (*_managers[_managers.size() - 1]).GetIsBusy() == true)
            {
                for (size_t i = 0; i < _directors.size(); i++)
                {
                    if ((*_directors[i]).GetIsBusy() == false)
                    {
                        (*_directors[i]).SetEmployeeBusy(true);
                        std::cout << "Call from " << (*calls[i]).GetPhoneNo() << " with the subject on "
                            << typeOfInfo << " information " << "was escalated to director with id "
                            << (*_directors[i]).GetEmployeeId()
                            << std::endl;
                        break;
                    }
                }
                continue;
            }
            if ((*_respondents[_respondents.size() - 1]).GetIsBusy() == true && (*_managers[_managers.size() - 1]).GetIsBusy() == true && (*_directors[_directors.size() - 1]).GetIsBusy() == true )
            {
                std::cout << "There are no employees avaialable for call from "
                          << (*calls[i]).GetPhoneNo() << " with the subject on "
                          << typeOfInfo << " information "
                          << (*_respondents[i]).GetEmployeeId()
                          << std::endl;
                return;
            }
            typeOfInfo = "";
        }
    };

};


int main()
{
    HR hire;

    // creating instances of employees
    std::vector<Employee*> respondents = hire.HireEmployees(10, EmployeeType::RespondentEmp);
    std::vector<Employee*> managers = hire.HireEmployees(3, EmployeeType::ManagerEmployee);
    std::vector<Employee*> directors = hire.HireEmployees(2, EmployeeType::DirectorEmployee);

    // call center instance
    CallCenter callCenter("Microsoft Call Center", respondents, managers, directors);

    // creating instances of Call
    std::vector<Call*> calls;

    std::string phoneNo = "";
    char generateNumber = 0;
    std::stringstream ss;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> phoneNoDistribution(48, 57);

    int generateCallType = 0;
    std::uniform_int_distribution<int> callTypeDistribution(0, 2);
    std::vector<CallType> callTypes = { CallType::Basic, CallType::Detailed, CallType::Classified };
    
    Call *call = nullptr;

    for (int i = 0; i < 5; i++)
    {
        ss << "+39";
        for (int j = 0; j < 10; j++)
        {
            generateNumber = phoneNoDistribution(generator);
            ss << generateNumber;   
        }
        ss >> phoneNo;
        ss.clear();
        generateNumber = 0;

        generateCallType = callTypeDistribution(generator);
        call = new Call(phoneNo, callTypes[generateCallType]);
        calls.push_back(call);
        generateCallType = 0;
    }

    callCenter.dispatchCall(calls);

    // implement multithreading to simulate the handling of multiple calls at the same time

    return 0;
}

