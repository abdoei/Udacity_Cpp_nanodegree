#include "Process.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class ProcessContainer
{
public:
    ProcessContainer()
    {
        this->refreshList();
    }
    void refreshList();
    string printList();
    vector<string> getList();

private:
    vector<Process> _list;
};

void ProcessContainer::refreshList()
{

}

string ProcessContainer::printList()
{

}

vector<string> ProcessContainer::getList()
{

}